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

public:
    explicit CpuTimer(QObject *parent = nullptr);

    [[nodiscard]] double_t usage() const;

signals:
    void usageChanged();

private:
#ifdef Q_OS_WIN
    windows::PdhQuery _query;
#endif

    int64_t _idleSum = 0;
    int64_t _restSum = 0;

    double_t _usage = 0;
    QTimer *_timer;

    void updateUsage();
};
