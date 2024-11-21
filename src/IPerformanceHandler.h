//
// Created by instellate on 2024-11-21.
//

#pragma once

class IPerformanceHandler {
public:
    virtual ~IPerformanceHandler() = default;

    [[nodiscard]] virtual double_t getCpuUsage() const = 0;

    [[nodiscard]] virtual QVector<double_t> getCoresUsage() const = 0;

    virtual void refreshValues() = 0;

};
