//
// Created by instellate on 2024-11-21.
//

#include "PdhQuery.h"

using namespace Windows;

PdhQuery::PdhQuery() {
    _module = LoadLibrary(L"pdh.dll");

    auto openQueryStatus = PdhOpenQueryA(nullptr, 0, &_query);
    if (openQueryStatus != ERROR_SUCCESS) {
        qWarning() << "Couldn't create Phd query, got error: " << formatPdhErrorMessage(openQueryStatus);
        throw std::exception();
    }
}

PdhQuery::~PdhQuery() {
    QWriteLocker lock(&_rwLock);

    PDH_HCOUNTER counter;
    foreach(counter, _counters.values()) {
        PdhRemoveCounter(counter);
    }

    PdhCloseQuery(_query);
}

void PdhQuery::addEnglishCounter(const QString &key, LPCWSTR query) {
    QWriteLocker lock{&_rwLock};
    if (_counters.find(key) != _counters.end()) {
        throw std::exception("Key already exists");
    }

    PDH_HCOUNTER counter;
    // ReSharper disable once CppTooWideScopeInitStatement
    const auto error = PdhAddEnglishCounterW(_query, query, 0, &counter);
    if (error != ERROR_SUCCESS) {
        qWarning() << "Creating pdh counter result in error:" << formatPdhErrorMessage(error);
        throw std::exception("Couldn't create pdh counter");
    }

    _counters.emplace(key, counter);

    lock.unlock();
}

double_t PdhQuery::get(const QString &key) const {
    QReadLocker lock{const_cast<QReadWriteLock *>(&_rwLock)};

    // ReSharper disable once CppTooWideScopeInitStatement
    const auto it = _counters.find(key);
    if (it == _counters.end()) {
        throw std::exception("Key doesn't exist");
    }

    const PDH_HCOUNTER counter = *it;
    PDH_FMT_COUNTERVALUE counterValue;
    // ReSharper disable once CppTooWideScopeInitStatement
    const auto error = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, nullptr, &counterValue);
    if (error != ERROR_SUCCESS) {
        qWarning() << "Formatting counter result in error:" << formatPdhErrorMessage(error);
        throw std::exception("Couldn't format value");
    }

    // ReSharper disable once CppRedundantCastExpression Just in case if compiled on some weird windows version
    return static_cast<double_t>(counterValue.doubleValue);
}

void PdhQuery::refresh() const {
    QReadLocker lock{const_cast<QReadWriteLock *>(&_rwLock)};
    PdhCollectQueryData(_query);
}

QString PdhQuery::formatPdhErrorMessage(const PDH_STATUS status) const {
    if (status == ERROR_SUCCESS) {
        return QStringLiteral("");
    }

    LPWSTR error;
    const auto formatStatus = FormatMessage(
        FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        _module,
        status,
        0,
        reinterpret_cast<LPWSTR>(&error),
        0,
        nullptr);
    if (formatStatus == 0) {
        return QStringLiteral("Failed to format error");
    }

    if (!error) {
        return QStringLiteral("");
    }

    return QString::fromWCharArray(error);
}
