//
// Created by instellate on 2024-11-21.
//


#pragma once

#include <QDateTime>
#include <QReadWriteLock>

#include "IPerformanceHandler.h"

namespace Linux {
#ifdef Q_OS_LINUX
    class LinuxPerformanceHandler final : public IPerformanceHandler {
    public:
        LinuxPerformanceHandler();

        [[nodiscard]] double_t getCpuUsage() const override;

        [[nodiscard]] QVector<double_t> getCoresUsage() const override;

        void refreshValues() override;

    private:
        struct CoreData {
            CoreData(const CoreData &) = default;

            CoreData(CoreData &&) = default;

            CoreData() = default;

            CoreData &operator=(const CoreData &) = default;

            int64_t idleTime = 0;
            int64_t restTime = 0;
        };

        QReadWriteLock _rwLock;
        QDateTime _lastFetch;
        CoreData _lastTotalCpuData;
        double_t _lastTotalCpuUsage = 0;
        QVector<double_t> _lastPerCoreUsage;
        QVector<CoreData> _lastCoreData;

        void updatePerCoreUsage(QTextStream &in);

        void updateTotalCpuUsage(QTextStream &in);
    };
#endif //Q_OS_LINUX
}
