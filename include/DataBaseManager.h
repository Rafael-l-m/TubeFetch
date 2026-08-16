
#ifndef YOUTUBEMEDIADOWNLOADER_DATABASEMANAGER_H
#define YOUTUBEMEDIADOWNLOADER_DATABASEMANAGER_H

#include <QSqlQuery>

struct DownloadItem {
    int id;
    QString url;
    int audio_code;
    int video_code;
    QString file_path;
    QString suffix;
    QString oldTitle;
    QString newTitle;
    int progress;
    bool isDownload;
    bool withMetadata;
};

class DataBaseManager final : public QObject {
    Q_OBJECT
public:
    explicit DataBaseManager(const QString& dbPath, QObject* parent = nullptr);

    [[nodiscard]] bool isOpen() const;

    [[nodiscard]] bool addDownload(int id, const QString& url, int audioCode, int videoCode, const QString& filePath, const QString& suffix, const QString& oldTitle, const QString& newTitle, int progress = 0, bool isDownload = false, bool withMetadata = false) const;

    [[nodiscard]] DownloadItem* readDownload(int id) const;

    [[nodiscard]] QList<DownloadItem *>* readAllDownloads() const;

    [[nodiscard]] bool updateAudioCode(int id, int newAudioCode) const;

    [[nodiscard]] bool updateVideoCode(int id, int newVideoCode) const;

    [[nodiscard]] bool updateNewTitle(int id, const QString& newTitle) const;

    [[nodiscard]] bool updateFilePath(int id, const QString& filePath) const;

    [[nodiscard]] bool updateSuffix(int id, const QString& suffix) const;

    [[nodiscard]] bool updateProgress(int id, int newProgress) const;

    [[nodiscard]] bool updateDownloaded(int id, bool downloaded) const;

    [[nodiscard]] bool updateIfMetadata(int id, bool ifM) const;

    [[nodiscard]] bool removeDownloaded(int id) const;

    [[nodiscard]] bool removeAllDownloads() const;

private:
    [[nodiscard]] bool createTable() const;

    QSqlDatabase db;
};

#endif //YOUTUBEMEDIADOWNLOADER_DATABASEMANAGER_H
