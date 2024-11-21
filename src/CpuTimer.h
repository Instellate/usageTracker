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
#ifdef Q_OS_WIN
    windows::PdhQuery _query;
#endif // Q_OS_WIN

#ifdef Q_OS_LINUX
    struct CoreData {
        CoreData(const CoreData &) = default;
        CoreData(CoreData &&) = default;
        CoreData() = default;

        CoreData &operator=(const CoreData &) = default;

        int64_t idleTime = 0;
        int64_t restTime = 0;
    };

    int64_t _idleTime = 0;
    int64_t _restTime = 0;
    QVector<CoreData> _coreData;

    void updateCoreUsageLinux(QTextStream &in);
#endif // Q_OS_LINUX

    double_t _usage = 0;
    QTimer *_timer;
    QVector<double_t> _perCoreUsage;

    void updateUsage();
};
