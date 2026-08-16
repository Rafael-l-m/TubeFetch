#pragma once

#include "download/Download.hpp"
#include <QSqlQuery>

class DataBase : public QObject {
    Q_OBJECT

protected:
    explicit DataBase(QObject* parent = nullptr);

    bool init();
    bool isOpen() const;
    bool isEmpty() const;
    bool ifExists(const qint64 id) const;
    QString getDataBaseDir() const;

    bool add(const Download& d) const;
    bool remove(const qint64 id) const;
    bool removeAll() const;

    bool updateUrl(const qint64 id, const QString& newUrl) const;
    bool updateTitle(const qint64 id, const QString& newTitle) const;
    bool updateAudioCode(const qint64 id, const QString& newAudioCode) const;
    bool updateVideoCode(const qint64 id, const QString& newVideoCode) const;
    bool updateNonDashCode(const qint64 id, const QString& newNonDashCode) const;
    bool updateBestAudio(const qint64 id, const bool newBestAudio) const;
    bool updateBestVideo(const qint64 id, const bool newBestVideo) const;
    bool updateSavePath(const qint64 id, const QString& newSavePath) const;
    bool updateSuffix(const qint64 id, const QString& newSuffix) const;
    bool updateMetadata(const qint64 id, const bool metadata) const;
    bool updateSubtitles(const qint64 id, const bool subtitles) const;
    bool updateProgress(const qint64 id, const double progress) const;
    bool updateDownloadStatus(const qint64 id, const DownloadStatus& downloadStatus) const;
    bool updateDownloadedBytes(const qint64 id, const double newDownloadedBytes) const;
    bool updateTotalBytes(const qint64 id, const double newTotalBytes) const;
    bool updateDownloadSpeed(const qint64 id, const double newDownloadSpeed) const;
    bool updateEta(const qint64 id, const QString& newEta) const;

    Download read(const qint64 id) const;
    QList<Download> readAll() const;

private:
    bool createTable() const;

private:
    QString dataBaseDir;
    QSqlDatabase db;
};
