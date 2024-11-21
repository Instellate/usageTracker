//
// Created by instellate on 2024-11-19.
//

#pragma once

#include <QtQmlIntegration>

#ifdef Q_OS_WIN
#include "windows/PdhQuery.h"
#endif

class CpuTimer : public QObject {
    Q_OBJECT

    QML_ELEMENT
    Q_PROPERTY(double_t usage READ usage NOTIFY usageChanged)
    Q_PROPERTY(int32_t refreshRate READ refreshRate WRITE setRefreshRate NOTIFY refreshRateChanged)

public:
    explicit CpuTimer(QObject *parent = nullptr);

    [[nodiscard]] double_t usage() const;

    [[nodiscard]] int32_t refreshRate() const;

    void setRefreshRate(int32_t refreshRate);

signals:
    void usageChanged();

    void refreshRateChanged();

private:
#ifdef Q_OS_WIN
    windows::PdhQuery _query;
#endif // Q_OS_WIN

#ifdef Q_OS_LINUX
    int64_t _idleSum = 0;
    int64_t _restSum = 0;
#endif // Q_OS_LINUX

    double_t _usage = 0;
    QTimer *_timer;

    void updateUsage();
};
