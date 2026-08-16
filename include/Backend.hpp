#pragma once

#include "db/DbClient.hpp"
#include "db/DbServer.hpp"
#include "download/DownloadManager.hpp"
#include "download/DownloadModel.hpp"

class Backend final : public QObject {
    Q_OBJECT

public:
    explicit Backend(QObject* parent = nullptr);

    inline static qint64 nextInternalId = 1;


    // Round

    Q_INVOKABLE static double doubleRound2(const double value);


    // Load Database

    void loadDatabase();


    // Download Model

    void setDownloadModel(DownloadModel* m);


    // Import / Export Data

    Q_INVOKABLE void exportData(const QUrl& filePath);

    Q_INVOKABLE void importData(const QUrl& filePath);


    // Tools Path

    Q_INVOKABLE bool needConfig();

    Q_INVOKABLE void findToolsPath();

    Q_INVOKABLE void checkYtDlp(const QUrl& filePath);

    Q_INVOKABLE void checkFFmpeg(const QUrl& filePath);

    Q_INVOKABLE void checkNode(const QUrl& filePath);

    Q_INVOKABLE void saveToolsPath(
        const QString& yt_dlp_path,
        const QString& ffmpeg_path,
        const QString& node_path
    );


    // Add Download

    Q_INVOKABLE void showItags(
        const QString& url,
        const QString& yt_dlp_path = "",
        const QString& ffmpeg_path = "",
        const QString& node_path = ""
    );

    Q_INVOKABLE void askAudioFilters();

    Q_INVOKABLE void askVideoFilters();

    Q_INVOKABLE void checkAvailablePath(const QUrl& path);

    Q_INVOKABLE void addNewDownload(
        const QString& url,
        const QString& title,
        const QString& audioCode,
        const QString& videoCode,
        const QString& nonDashCode,
        const bool bestAudio,
        const bool bestVideo,
        const QString& savePath,
        const QString& suffix,
        const bool metadata,
        const bool subtitles,
        const double progress = 0.0,
        const DownloadStatus downloadState = DownloadStatus::WAITING,
        const double downloadedBytes = 0.0,
        const double totalBytes = 0.0,
        const double downloadSpeed = 0.0,
        const QString& eta = "00:00"
    );


    // Remove Download

    Q_INVOKABLE void removeDownload(const qint64 internalId);

    Q_INVOKABLE void removeAllDownloads();


    // Edit Download

    Q_INVOKABLE void requestEditInformation(const qint64 internalId);

    Q_INVOKABLE void editDownload(
        const qint64 internalId,
        const QString& url,
        const QString& title,
        const QString& audioCode,
        const QString& videoCode,
        const QString& nonDashCode,
        const bool bestAudio,
        const bool bestVideo,
        const QString& savePath,
        const QString& suffix,
        const bool metadata,
        const bool subtitles,
        const double progress = 0.0,
        const DownloadStatus downloadState = DownloadStatus::WAITING,
        const double downloadedBytes = 0.0,
        const double totalBytes = 0.0,
        const double downloadSpeed = 0.0,
        const QString& eta = "00:00"
    );


    // Start Download

    Q_INVOKABLE void startDownload(const qint64 internalId);

    Q_INVOKABLE void startDownloads();


    // Stop Download

    Q_INVOKABLE void stopDownloads();


    // Clear Download Status

    Q_INVOKABLE void clearAllStatus();


    // Quit App

    Q_INVOKABLE void requestQuitApp();

signals:
    // Import / Export Data
    void dataExported(const bool ok, const QString& message);
    void dataImported(const bool ok, const QString& message);

    // Tools Path
    void toolsPathFound(const QJsonObject& obj);
    void ytDlpChecked(const bool isExecutable, const bool isYtDlp, const QString& path);
    void ffmpegChecked(const bool isExecutable, const bool isFFmpeg, const QString& path);
    void nodeChecked(const bool isExecutable, const bool isNode, const QString& path);
    void toolsPathSaved(const bool ok, const QString& error);

    // Add Download
    void itagsShown(const QString& content);
    void filtersAsked(const QStringList& mediaFilters);
    void availablePathChecked(const bool ok, const QString& path);
    void newDownloadAdded(const bool accepted, const QString& message);

    // Remove Download
    void downloadRemoved(const bool ok, const QString& message);
    void downloadsRemoved(const bool ok, const QString& message);

    // Edit Download
    void editInformationRequest(const QJsonObject& obj, const QString& message);

    // Start Download
    void isAlreadyDownloading();
    void noAvailableDownloads();
    void downloadFailed(const qint64 internalId, const QString& message);
    void downloadAllFinished();

    // Stop Download
    void noDownloading();

private:
    bool checkExecutable(const QUrl& filePath);

private:
    mutable QMutex m_mutex;

    DownloadManager* m_downloadManager;

    DbServer* dbServer;
    DbClient* dbClient;
    int serverPort;

    DownloadModel* dm = nullptr;
};
