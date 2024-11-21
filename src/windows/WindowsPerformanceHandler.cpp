//
// Created by instellate on 2024-11-21.
//

#include "WindowsPerformanceHandler.h"
#include <sysinfoapi.h>
#include <ChString.h>

using namespace Windows;

WindowsPerformanceHandler::WindowsPerformanceHandler() {
    _query.addEnglishCounter("total{T}Idle", L"\\Processor(_Total)\\% Idle Time");

    SYSTEM_INFO info;
    GetSystemInfo(&info);
    for (DWORD i = 0; i < info.dwNumberOfProcessors; ++i) {
        std::wstringstream stream;
        stream << "\\Processor(" << i << ")\\% Idle Time";
        _query.addEnglishCounter("total{" + QString::number(i) + "}Idle", stream.str().c_str());
    }

    _coreAmount = static_cast<int32_t>(info.dwNumberOfProcessors);
    _query.refresh();
}

double_t WindowsPerformanceHandler::getCpuUsage() const {
    return (100.0 - _query.get("total{T}Idle")) / 100.0;
}

QVector<double_t> WindowsPerformanceHandler::getCoresUsage() const {
    QVector<double_t> vec;
    vec.reserve(_coreAmount);
    for (int32_t i = 0; i < _coreAmount; ++i) {
        vec.emplace_back((100.0 - _query.get("total{" + QString::number(i) + "}Idle")) / 100);
    }

    return vec;
}

void WindowsPerformanceHandler::refreshValues() {
    _query.refresh();
}
