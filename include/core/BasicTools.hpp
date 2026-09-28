#pragma once

#include <QDirIterator>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryFile>

namespace CONFIG {
    inline bool writeConfig(
        const QMap<QString, QVariant>& map,
        const QString& organization = "rafael",
        const QString& application = "TubeFetch")
    {
        QSettings settings(QSettings::UserScope, organization, application);

        for (auto it = map.cbegin(); it != map.cend(); ++it) { settings.setValue(it.key(), it.value()); }

        settings.sync();

        return settings.status() == QSettings::NoError;
    }

    template<typename T>
    T readConfig(
        const QString& key,
        const T& defaultValue = T{},
        const QString& organization = "rafael",
        const QString& application = "TubeFetch")
    {
        QSettings settings(QSettings::UserScope, organization, application);
        return settings.value(key, QVariant::fromValue(defaultValue)).template value<T>();
    }
}

namespace EXPORT_IMPORT_DATA {
    template<typename T>
    bool exportData(const T& obj, const QString& filePath) {
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
    bool importData(T& obj, const QString& filePath) {
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

inline bool clearDirectory(const QString& dirPath) {
    QDir dir(dirPath.trimmed());

    if (!dir.exists()) { return false; }

    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    for (const auto& info : entries) {
        if (info.isDir()) {
            if (!clearDirectory(info.absoluteFilePath())) { return false; }
            if (!dir.rmdir(info.fileName())) { return false; }
        }

        else {
            if (!dir.remove(info.fileName())) { return false; }
        }
    }

    return true;
}

inline bool writeText(const QString& filePath, const QString& text) {
    const auto filePathTrimmed = filePath.trimmed();
    const auto textTrimmed = text.trimmed();

    if (filePathTrimmed.isEmpty()) { return false; }

    if (textTrimmed.isEmpty()) { return false; }

    QFile file(filePathTrimmed);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { return false; }

    QTextStream out(&file);
    out << text;

    file.close();

    return true;
}
