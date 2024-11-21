#include "util.h"
#include "linux/LinuxPerformanceHandler.h"
#include "windows/WindowsPerformanceHandler.h"

QSharedPointer<IPerformanceHandler> getSharedPerformanceHandler() {
#ifdef Q_OS_LINUX
    static QSharedPointer<Linux::LinuxPerformanceHandler> linuxPtr{new Linux::LinuxPerformanceHandler};
    return linuxPtr;
#endif // Q_OS_LINUX

#ifdef Q_OS_WIN
    static QSharedPointer<Windows::WindowsPerformanceHandler> linuxPtr{new Windows::WindowsPerformanceHandler};
    return linuxPtr;
#endif // Q_OS_WIN
}

