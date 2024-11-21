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
    Q_PROPERTY(QVector<double_t> perCoreUsage READ perCoreUsage NOTIFY perCoreUsageChanged)

public:
    explicit CpuTimer(QObject *parent = nullptr);

    [[nodiscard]] double_t usage() const;

    [[nodiscard]] int32_t refreshRate() const;

    [[nodiscard]] QVector<double_t> perCoreUsage() const;

    void setRefreshRate(int32_t refreshRate);

signals:
    void usageChanged();

    void refreshRateChanged();

    void perCoreUsageChanged();

private:
    QTimer *_timer;
    QVector<double_t> _perCoreUsage;

    void updateUsage();
};
