#pragma once

#include <download/Download.hpp>
#include <download/YoutubeActions.hpp>
#include <QSqlQuery>

class DataBase : public QObject {
    Q_OBJECT

public:
    explicit DataBase(QObject* parent = nullptr);

    void initDataBase();

protected:
    bool init();
    bool isOpen() const;
    bool isEmpty() const;
    bool ifExists(qint64 id) const;
    QString getDataBaseDir() const;

    bool add(const Download& d) const;
    bool addYoutubeAction(const YoutubeActions& y) const;
    bool remove(qint64 id) const;
    bool removeAll() const;
    bool removeUselessActions(qint64 timeLeft) const;

    bool updateUrl(qint64 id, const QString& newUrl) const;
    bool updateTitle(qint64 id, const QString& newTitle) const;
    bool updateAudioCode(qint64 id, const QString& newAudioCode) const;
    bool updateVideoCode(qint64 id, const QString& newVideoCode) const;
    bool updateNonDashCode(qint64 id, const QString& newNonDashCode) const;
    bool updateBestAudio(qint64 id, bool newBestAudio) const;
    bool updateBestVideo(qint64 id, bool newBestVideo) const;
    bool updateSavePath(qint64 id, const QString& newSavePath) const;
    bool updateSuffix(qint64 id, const QString& newSuffix) const;
    bool updateMetadata(qint64 id, bool metadata) const;
    bool updateSubtitles(qint64 id, bool subtitles) const;
    bool updateProgress(qint64 id, double progress) const;
    bool updateDownloadStatus(qint64 id, const DownloadStatus& downloadStatus) const;
    bool updateDownloadedBytes(qint64 id, double newDownloadedBytes) const;
    bool updateTotalBytes(qint64 id, double newTotalBytes) const;
    bool updateDownloadSpeed(qint64 id, double newDownloadSpeed) const;
    bool updateEta(qint64 id, const QString& newEta) const;

    Download read(qint64 id) const;
    QList<Download> readAll() const;

    int getWeight(qint64 time) const;

private:
    bool createMainTable() const;
    //bool createYoutubeActionsTable() const;

private:
    QString dataBaseDir;
    QSqlDatabase db;
};
