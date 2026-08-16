
#ifndef YOUTUBEMEDIADOWNLOADER_DOWNLOADMANAGER_H
#define YOUTUBEMEDIADOWNLOADER_DOWNLOADMANAGER_H

#include "DataBaseManager.h"
#include "DownloadInfoWidget.h"
#include "DownloadItemWidget.h"
#include <QQueue>

class DownloadManager final : public QObject {
    Q_OBJECT
public:
    explicit DownloadManager(QString ytDlpPath, QString ffmpegPath, QString nodePath, const QString& databasePath, int max_concurrent_downloads = 2, int retries = 5, int fragment_retries = 5, int concurrent_fragments = 4);

    bool addDownload(int id, const DownloadItem *info, DownloadItemWidget* wg) const;

    [[nodiscard]] bool removeAllDownloads() const;

    [[nodiscard]] bool removeDownload(int id) const;

    [[nodiscard]] QPair<int, QList<DownloadItemWidget *> > restoreData() const;

    [[nodiscard]] bool haveAvailableDownload() const;

    [[nodiscard]] bool haveAvailableWidget() const;

    [[nodiscard]] bool haveActiveProcesses() const;

    bool setConcurrentDownload(int cd);

    void startNextDownload();

    void stopAll();

    static QString upgradeYtDlp(const QString& ytDlpPath);


    // Database
    [[nodiscard]] bool updateNewTitle(int id, const QString& newTitle) const;
    [[nodiscard]] bool updateAudioCode(int id, int audio_code) const;
    [[nodiscard]] bool updateVideoCode(int id, int video_code) const;
    [[nodiscard]] bool updateIfMetadata(int id, bool withMetadata) const;
    [[nodiscard]] bool updateFilePath(int id, const QString& filePath) const;
    [[nodiscard]] bool updateSuffix(int id, const QString& suffix) const;
    [[nodiscard]] DownloadItem* readDownload(int id) const;

signals:
    void renewWidgets(QPair<int, QList<DownloadItemWidget *> >);
    void finishedDownload();

private:
    static constexpr int MIN_CONCURRENT_DOWNLOADS = 1;
    static constexpr int MAX_CONCURRENT_DOWNLOADS = 5;
    static constexpr int MIN_WAIT_TIME = 3000;
    static constexpr int MAX_WAIT_TIME = 7000;
    static constexpr int MIN_RATE = 330;
    static constexpr int MAX_RATE = 730;

    void download(int id);

    QString yt_dlp_path;
    QString ffmpeg_path;
    QString node_js_path;

    int activeCount;
    int maxConcurrentDownloads;
    QQueue<int>* taskQueue;
    DataBaseManager* database;
    QMap<int, DownloadItemWidget*>* downloads;
    QMap<int, QProcess*>* activeProcesses;

    int retries;
    int fragment_retries;
    int concurrent_fragments;
    QSet<QString> supportedAudioTypes;
    QSet<QString> supportedVideoTypes;

    const QStringList userAgents = {
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Edg/123.0.0.0 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:122.0) Gecko/20100101 Firefox/122.0",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36",
        "Mozilla/5.0 (Linux; Android 12; Pixel 6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Mobile Safari/537.36"
    };
};

#endif //YOUTUBEMEDIADOWNLOADER_DOWNLOADMANAGER_H
