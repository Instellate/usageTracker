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

int32_t CpuTimer::refreshRate() const {
    return _timer->interval();
}

QVector<double_t> CpuTimer::perCoreUsage() const {
    return _perCoreUsage;
}

void CpuTimer::setRefreshRate(const int32_t refreshRate) {
    _timer->setInterval(refreshRate);
    emit refreshRateChanged();
}

void CpuTimer::updateUsage() {
#ifdef Q_OS_LINUX

    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open /proc/stat";
        return;
    }

    QTextStream in(&file);

    QString str;
    in.readLineInto(&str);
    if (!str.startsWith("cpu ")) {
        return;
    }

    auto spaces = str.split(' ');

    int64_t idleTime = spaces[4].toLong() + spaces[5].toLong();
    int64_t restTime =
            spaces[1].toLong() + spaces[2].toLong() + spaces[3].toLong() + spaces[6].toLong() + spaces[7].toLong();

    auto totalDifference = _idleTime + _restTime - (idleTime + restTime);
    auto idleDifference = _idleTime - idleTime;

    _usage = (static_cast<double_t>(totalDifference) - static_cast<double_t>(idleDifference)) /
             static_cast<double_t>(totalDifference);
    _idleTime = idleTime;
    _restTime = restTime;
    emit usageChanged();

    updateCoreUsageLinux(in);
#endif // Q_OS_LINUX

#ifdef Q_OS_WIN
    _query.refresh();

    const double_t usageVal = _query.get("totalIdle");
    _usage = (100.0 - usageVal) / 100;
    emit usageChanged();
#endif // Q_OS_WIN
}

#ifdef Q_OS_UNIX
void CpuTimer::updateCoreUsageLinux(QTextStream &in) {
    QString str;

    QVector<double_t> vector;
    vector.reserve(_perCoreUsage.length());
    qsizetype i = 0;
    while (in.readLineInto(&str)) {
        if (!str.startsWith("cpu")) {
            break;
        }

        CoreData oldData;
        if (_coreData.length() > i) {
            oldData = _coreData[i];
        }

        auto spaces = str.split(' ');

        int64_t idleTime = spaces[4].toLong() + spaces[5].toLong();
        int64_t restTime =
                spaces[1].toLong() + spaces[2].toLong() + spaces[3].toLong() + spaces[6].toLong() + spaces[7].toLong();

        auto totalDifference = oldData.idleTime + oldData.restTime - (idleTime + restTime);
        auto idleDifference = oldData.idleTime - idleTime;

        double_t usage = (static_cast<double_t>(totalDifference) - static_cast<double_t>(idleDifference)) /
         static_cast<double_t>(totalDifference);
        vector.emplace_back(usage);

        CoreData data{};
        data.idleTime = idleTime;
        data.restTime = restTime;
        if (_coreData.length() > i) {
            _coreData[i] = data;
        } else {
            _coreData.emplace_back(data);
        }

        ++i;
    }

    _perCoreUsage = vector;
    emit perCoreUsageChanged();
}
#endif
