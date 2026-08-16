#pragma once

#include "Constants.hpp"
#include <QDirIterator>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryFile>

namespace EXPORT_IMPORT_DATA {
    template<typename T>
    inline bool exportData(const T& obj, const QString& filePath) {
        const auto fp = filePath.trimmed();

        if (fp.isEmpty()) { return false; }

        QByteArray raw;

        {
            QDataStream out(&raw, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_11);
            out << obj;

            if (out.status() != QDataStream::Ok) { return false; }
        }

        QFile f(fp);

        if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return false; }

        const qint64 written = f.write(raw);

        if (written != raw.size()) { return false; }

        return true;
    }

    template<typename T>
    inline bool importData(T& obj, const QString& filePath) {
        const auto fp = filePath.trimmed();

        if (fp.isEmpty()) { return false; }

        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly)) { return false; }

        const auto raw = f.readAll();
        if (raw.isEmpty()) { return false; }

        QDataStream in(raw);
        in >> obj;

        return in.status() == QDataStream::Ok;
    }
}

namespace SYS_INFO {
    inline bool checkPathWritable(const QString& path) {
        if (path.trimmed().isEmpty()) { return false; }

        const auto dir = QFileInfo(path).dir();

        QTemporaryFile file(dir.filePath("XXXXXX.tmp"));
        file.setAutoRemove(true);

        return file.open();
    }

    inline QString findToolPath(const QString& program) {
        const auto _prog = program.trimmed();

        if (_prog.isEmpty()) { return {}; }

        const auto path = QStandardPaths::findExecutable(_prog);

        if (!path.isEmpty()) { return path; }

        const auto searchPaths = SYS_PATHS::SEARCH_PATHS();

        for (const auto& dirPath : searchPaths) {
            QDirIterator it(dirPath, QDir::Files | QDir::Executable, QDirIterator::Subdirectories);

            while (it.hasNext()) {
                it.next();

                QFileInfo fi(it.filePath());

                if (fi.fileName().compare(program, Qt::CaseInsensitive) == 0) {
                    return fi.absoluteFilePath();
                }
            }
        }

        return {};
    }

    namespace CONFIG {
        inline bool writeConfig(
            const QMap<QString, QVariant>& map,
            const QString& organization = "rafael",
            const QString& application = "YoutubeMediaDownloader")
        {
            QSettings settings(QSettings::UserScope, organization, application);

            for (auto it = map.cbegin(); it != map.cend(); ++it) {
                settings.setValue(it.key(), it.value());
            }

            settings.sync();

            return settings.status() == QSettings::NoError;
        }

        template<typename T>
        inline T readConfig(
            const QString& key,
            const T& defaultValue = T{},
            const QString& organization = "rafael",
            const QString& application = "YoutubeMediaDownloader")
        {
            QSettings settings(QSettings::UserScope, organization, application);
            return settings.value(key, QVariant::fromValue(defaultValue)).template value<T>();
        }
    }
}
