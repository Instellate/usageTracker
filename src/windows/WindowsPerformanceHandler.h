//
// Created by instellate on 2024-11-21.
//


#pragma once

#include <QVector>
#include <QDateTime>
#include <QReadWriteLock>
#include "IPerformanceHandler.h"
#include "PdhQuery.h"

namespace Windows {
#ifdef Q_OS_WIN
    class WindowsPerformanceHandler final : public IPerformanceHandler {
    public:
        WindowsPerformanceHandler();

        [[nodiscard]] double_t getCpuUsage() const override;

        [[nodiscard]] QVector<double_t> getCoresUsage() const override;

        void refreshValues() override;

    private:
        PdhQuery _query;
        int32_t _coreAmount;
    };
#endif // Q_OS_WIN
}
