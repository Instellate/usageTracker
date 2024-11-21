//
// Created by instellate on 2024-11-21.
//

#include "LinuxPerformanceHandler.h"

#include <qfile.h>
#include <QList>
#include <QReadWriteLock>

using namespace Linux;

LinuxPerformanceHandler::LinuxPerformanceHandler() {
    _lastFetch = QDateTime::fromMSecsSinceEpoch(0);
}

double_t LinuxPerformanceHandler::getCpuUsage() const {
    QReadLocker lock{const_cast<QReadWriteLock *>(&_rwLock)};
    return _lastTotalCpuUsage;
}

QVector<double_t> LinuxPerformanceHandler::getCoresUsage() const {
    QReadLocker lock{const_cast<QReadWriteLock *>(&_rwLock)};
    return _lastPerCoreUsage;
}

void LinuxPerformanceHandler::refreshValues() {
    QWriteLocker lock{&_rwLock};
    auto timeDifference = QDateTime::currentDateTimeUtc() - _lastFetch;
    if (timeDifference > std::chrono::milliseconds(500)) {
        return;
    }

    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open /proc/stat";
        return;
    }

    QTextStream in(&file);
    updateTotalCpuUsage(in);
    updatePerCoreUsage(in);
}

void LinuxPerformanceHandler::updatePerCoreUsage(QTextStream &in) {
    QString str;

    QVector<double_t> vector;
    vector.reserve(_lastPerCoreUsage.length());
    qsizetype i = 0;
    while (in.readLineInto(&str)) {
        if (!str.startsWith("cpu")) {
            break;
        }

        CoreData oldData;
        if (_lastCoreData.length() > i) {
            oldData = _lastCoreData[i];
        }

        auto spaces = str.split(' ');

        const int64_t idleTime = spaces[4].toLong() + spaces[5].toLong();
        const int64_t restTime =
                spaces[1].toLong() + spaces[2].toLong() + spaces[3].toLong() + spaces[6].toLong() + spaces[7].toLong();

        const auto totalDifference = oldData.idleTime + oldData.restTime - (idleTime + restTime);
        const auto idleDifference = oldData.idleTime - idleTime;

        double_t usage = (static_cast<double_t>(totalDifference) - static_cast<double_t>(idleDifference)) /
                         static_cast<double_t>(totalDifference);
        vector.emplace_back(usage);

        CoreData data{};
        data.idleTime = idleTime;
        data.restTime = restTime;
        if (_lastCoreData.length() > i) {
            _lastCoreData[i] = data;
        } else {
            _lastCoreData.emplace_back(data);
        }

        ++i;
    }

    _lastPerCoreUsage = vector;
}

void LinuxPerformanceHandler::updateTotalCpuUsage(QTextStream &in) {
    QString str;
    in.readLineInto(&str);
    if (!str.startsWith("cpu ")) {
        return;
    }

    auto spaces = str.split(' ');

    const int64_t idleTime = spaces[4].toLong() + spaces[5].toLong();
    const int64_t restTime =
            spaces[1].toLong() + spaces[2].toLong() + spaces[3].toLong() + spaces[6].toLong() + spaces[7].toLong();

    const auto totalDifference = _lastTotalCpuData.idleTime + _lastTotalCpuData.restTime - (idleTime + restTime);
    const auto idleDifference = _lastTotalCpuData.idleTime - idleTime;

    _lastTotalCpuUsage = (static_cast<double_t>(totalDifference) - static_cast<double_t>(idleDifference)) /
                             static_cast<double_t>(totalDifference);

    _lastTotalCpuData.idleTime = idleTime;
    _lastTotalCpuData.restTime = restTime;
}
