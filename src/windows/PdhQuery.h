//
// Created by instellate on 2024-11-21.
//

#pragma once

#include <QtCore>

#ifdef Q_OS_WIN
#include <pdh.h>
#endif

namespace Windows {
#ifdef Q_OS_WIN
    class PdhQuery {
    public:
        PdhQuery();

        virtual ~PdhQuery();

        void addEnglishCounter(const QString &key, LPCWSTR query);

        [[nodiscard]] double_t get(const QString &key) const;

        void refresh() const;

    private:
        QReadWriteLock _rwLock;
        PDH_HQUERY _query = nullptr;
        QHash<QString, PDH_HCOUNTER> _counters;

        HMODULE _module = nullptr; // For error messages

        QString formatPdhErrorMessage(PDH_STATUS status) const;
    };
#endif // Q_OS_WIN
}
