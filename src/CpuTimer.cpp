//
// Created by instellate on 2024-11-19.
//

#include <fstream>
#include "CpuTimer.h"

CpuTimer::CpuTimer(QObject *parent) : QObject(parent) {
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &CpuTimer::updateUsage);
    updateUsage();
    _timer->start(500);
}

double_t CpuTimer::usage() const {
    return _usage;
}

#ifdef Q_OS_UNIX

void CpuTimer::updateUsage() {
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open /proc/stat";
        return;
    }

    int64_t idleSum = 0;
    int64_t restSum = 0;

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

    auto totalDifference = (_idleSum + _restSum) - (idleSum + restSum);
    auto idleDifference = _idleSum - idleSum;

     _usage = (static_cast<double_t>(totalDifference) - static_cast<double_t>(idleDifference)) /
                     static_cast<double_t>(totalDifference);
     _idleSum = idleSum;
     _restSum = restSum;
     emit usageChanged();
}

#endif