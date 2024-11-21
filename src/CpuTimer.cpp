//
// Created by instellate on 2024-11-19.
//

#include "CpuTimer.h"
#include "util.h"

CpuTimer::CpuTimer(QObject *parent) : QObject(parent) {
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &CpuTimer::updateUsage);
    updateUsage();
    _timer->start(500);
}

double_t CpuTimer::usage() const {
    return getSharedPerformanceHandler()->getCpuUsage();
}

int32_t CpuTimer::refreshRate() const {
    return _timer->interval();
}

QVector<double_t> CpuTimer::perCoreUsage() const {
    return getSharedPerformanceHandler()->getCoresUsage();
}

void CpuTimer::setRefreshRate(const int32_t refreshRate) {
    _timer->setInterval(refreshRate);
    emit refreshRateChanged();
}

void CpuTimer::updateUsage() {
    getSharedPerformanceHandler()->refreshValues();
    emit usageChanged();
    emit perCoreUsageChanged();
}

