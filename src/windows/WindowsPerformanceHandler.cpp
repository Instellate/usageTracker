//
// Created by instellate on 2024-11-21.
//

#include "WindowsPerformanceHandler.h"

using namespace Windows;

WindowsPerformanceHandler::WindowsPerformanceHandler() {
    _query.addEnglishCounter("totalIdle", L"\\Processor(_Total)\\% Idle Time");
    _query.refresh();
}

double_t WindowsPerformanceHandler::getCpuUsage() const {
}

QVector<double_t> WindowsPerformanceHandler::getCoresUsage() const {
}

void WindowsPerformanceHandler::refreshValues() {
    QWriteLocker lock{&_rwLock};
    auto timeDifference = QDateTime::currentDateTimeUtc() - _lastFetch;
    if (timeDifference > std::chrono::milliseconds(500)) {
        return;
    }

}
