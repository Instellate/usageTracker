//
// Created by instellate on 2024-11-19.
//

#include "CpuTimer.h"

CpuTimer::CpuTimer(QObject *parent) : QObject(parent) {
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &CpuTimer::updateUsage);

#ifdef Q_OS_WIN
    _query.addEnglishCounter("totalIdle", L"\\Processor(_Total)\\% Idle Time");
    _query.refresh();
#endif

    updateUsage();
    _timer->start(500);
}

double_t CpuTimer::usage() const {
    return _usage;
}

void CpuTimer::updateUsage() {
#ifdef Q_OS_UNIX
    int64_t idleSum = 0;
    int64_t restSum = 0;

    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open /proc/stat";
        return;
    }

    QTextStream in(&file);

    QString str;
    while (in.readLineInto(&str)) {
        if (!str.startsWith("cpu")) {
            break;
        }

        auto spaces = str.split(' ');

        idleSum += spaces[4].toLong() + spaces[5].toLong();
        restSum +=
                spaces[1].toLong() + spaces[2].toLong() + spaces[3].toLong() + spaces[6].toLong() + spaces[7].toLong();
    }

    auto totalDifference = _idleSum + _restSum - (idleSum + restSum);
    auto idleDifference = _idleSum - idleSum;

    _usage = (static_cast<double_t>(totalDifference) - static_cast<double_t>(idleDifference)) /
             static_cast<double_t>(totalDifference);
    _idleSum = idleSum;
    _restSum = restSum;
    emit usageChanged();
#endif // Q_OS_UNIX

#ifdef Q_OS_WIN
    _query.refresh();

    const double_t usageVal = _query.get("totalIdle");
    _usage = (100.0 - usageVal) / 100;
    emit usageChanged();
#endif // Q_OS_WIN
}
