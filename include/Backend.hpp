#pragma once

#include <db/DbClient.hpp>
#include <db/DbServer.hpp>
#include <download/DownloadManager.hpp>
#include <download/DownloadModel.hpp>
#include <provider/ProviderManager.hpp>
#include <tools/ToolsManager.hpp>
#include <update/UpdateChecker.hpp>
#include <url/URLInfoManager.hpp>


class Backend final : public QObject {
    Q_OBJECT

public:
    explicit Backend(QObject* parent = nullptr);

    inline static qint64 nextInternalId = 1;

    Q_INVOKABLE static double doubleRound2(double value);

    // Import / Export Data
    Q_INVOKABLE void exportData(const QUrl& filePath);
    Q_INVOKABLE void importData(const QUrl& filePath);
    Q_INVOKABLE void exportOutputs(const QUrl& filePath, const QString& text);

    // Load Data
    void setDownloadModel(DownloadModel* dm);
    void loadDataBase();

    // Tools
    Q_INVOKABLE void findToolsPath();
    Q_INVOKABLE void loadTools();
    Q_INVOKABLE void checkTools();
    Q_INVOKABLE void checkFFmpeg(const QUrl& ffmpegPath);
    Q_INVOKABLE void checkNode(const QUrl& nodePath);
    Q_INVOKABLE void downloadYtDlp();
    Q_INVOKABLE void downloadPoTokenProvider();
    Q_INVOKABLE void startPoTokenProvider(const QString& poTokenProviderPath = TOOLS::poTokenProviderPath());
    Q_INVOKABLE void updateYtDlp();
    Q_INVOKABLE void updateYtDlpToNightly();
    Q_INVOKABLE void saveYtDlpPath(const QString& ytDlpPath = TOOLS::ytDlpFilePath());    
    Q_INVOKABLE void saveFFmpegPath(const QString& ffmpegPath);
    Q_INVOKABLE void saveNodePath(const QString& nodePath);

    // Add Download
    Q_INVOKABLE void showCodecs(const QString& url, const QString& ytDlpPath = "", const QString& ffmpegPath = "", const QString& nodePath = "");
    Q_INVOKABLE void askAudioFilters();
    Q_INVOKABLE void askVideoFilters();
    Q_INVOKABLE void checkAvailablePath(const QUrl& path);
    Q_INVOKABLE void addNewDownload(
        const QString& url,
        const QString& title,
        const QString& audioCode,
        const QString& videoCode,
        const QString& nonDashCode,
        bool bestAudio,
        bool bestVideo,
        const QString& savePath,
        const QString& suffix,
        bool metadata,
        bool subtitles,
        double progress = 0.0,
        const DownloadStatus& downloadState = DownloadStatus::WAITING,
        double downloadedBytes = 0.0,
        double totalBytes = 0.0,
        double downloadSpeed = 0.0,
        const QString& eta = "00:00"
    );

    // Remove Download
    Q_INVOKABLE void removeDownload(qint64 internalId);
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
        bool bestAudio,
        bool bestVideo,
        const QString& savePath,
        const QString& suffix,
        bool metadata,
        bool subtitles,
        double progress = 0.0,
        const DownloadStatus& downloadState = DownloadStatus::WAITING,
        double downloadedBytes = 0.0,
        double totalBytes = 0.0,
        double downloadSpeed = 0.0,
        const QString& eta = "00:00"
    );

    // Start / Stop Download
    Q_INVOKABLE void startDownload(qint64 internalId);
    Q_INVOKABLE void startDownload();
    Q_INVOKABLE void stopDownload(qint64 internalId);
    Q_INVOKABLE void stopDownload();

    // URL Manager
    Q_INVOKABLE void isValidUrl(const QString& url);
    Q_INVOKABLE void getUrlInfo(const QString& url);

    // Update Checker
    Q_INVOKABLE static void visitRepo();
    Q_INVOKABLE static void helpDoc();
    Q_INVOKABLE static void reportIssues();
    Q_INVOKABLE void checkUpdate() const;

    // Youtube Rate Limiter
    static const int maxPerHour {55};
    static const int maxPerDay {180};

    Q_INVOKABLE void consume(qint64 internalId, int type, int weight, qint64 createdAt = QDateTime::currentSecsSinceEpoch());
    Q_INVOKABLE void cleanUp();

    // Clear Status
    Q_INVOKABLE void clearAllDownloadStatus();

    // Settings
    Q_INVOKABLE void loadSettings();
    Q_INVOKABLE void writeDefaultSettings();
    Q_INVOKABLE void updateLightMode(bool automatic, bool light);
    Q_INVOKABLE void updateSelfCheck(bool ok);
    Q_INVOKABLE void updateRateLimit(bool ok);
    Q_INVOKABLE void updateMessageBeforeSaving(bool ok);
    Q_INVOKABLE void updateFFmpegPath(const QString& filePath);
    Q_INVOKABLE void updateNodePath(const QString& filePath);

    // Quit App
    Q_INVOKABLE void requestQuitApp();

signals:
    // Import / Export Data
    void dataExported(bool ok, const QString& message);
    void dataImported(bool ok, const QString& message);
    void outputExported(bool ok, const QString& message);

    // Tools
    void toolsPathFound(const QString& ffmpegPath, const QString& nodePath);
    void toolsChecked(bool needYtDlp, bool needFFmpeg, bool needNode, const QString& ydp, const QString& ffp, const QString& ndp);
    void ffmpegChecked(bool isExecutable, bool isFFmpeg, const QString& filePath);
    void nodeChecked(bool isExecutable, bool isNode, const QString& filePath);
    void ytDlpDownloaded(bool ok);
    void ytDlpDownloadedProgress(qint64 bytesReceived, qint64 bytesTotal);
    void poTokenProviderDownloaded(bool ok);
    void poTokenProviderDownloadedProgress(qint64 bytesReceived, qint64 bytesTotal);
    void poTokenProviderStarted(bool ok);
    void ytDlpUpdated(bool ok);
    void ytDlpUpdatedToNightly(bool ok);

    // Add Download
    void codecsShown(const QString& message);
    void filtersAsked(const QStringList& mediaFilters);
    void availablePathChecked(bool ok, const QString& path);
    void newDownloadAdded(bool accepted, const QString& message);

    // Remove Download
    void downloadRemoved(bool ok, const QString& message);
    void downloadsRemoved(bool ok, const QString& message);

    // Edit Download
    void editInformationRequest(const QJsonObject& obj, const QString& message);

    // Start / Stop Download
    void isAlreadyDownloading(qint64 internalId, const QString& message);
    void isNotDownloading(qint64 internalId, const QString& message);
    void downloadFinished(qint64 internalId);
    void downloadStopped(qint64 internalId, bool ok, const QString& message);
    void downloadErrorOccurred(qint64 internalId, const QString& message);
    void subprocessErrorOccurred(qint64 internalId, const QString& message);
    void failedAtStart(qint64 internalId);
    void allDownloadFinished();
    void allDownloadStopped();

    // URL Manager
    void urlValid(const QString& url, bool ok);
    void urlCut(const QString& url);
    void jsonReady(const QJsonObject& obj);
    void jsonError(const QString& message);

    // Update Checker
    void updateChecked(int updateStatus, const QString& latestVersion, const QString& notes, const QString& downloadUrl, const QString& message);

    // Youtube Rate Limiter
    void possiblePerform(bool ok);

    // Settings
    void settingsLoaded(const QJsonObject& obj);
    void lightModeUpdated(bool ok);
    void selfCheckUpdated(bool ok);
    void rateLimitUpdated(bool ok);
    void messageBeforeSavingUpdated(bool ok);

private:
    void usageSince(qint64 timestamp, std::function<void(int)> cb);
    void usageLastHour(std::function<void(int)> cb);
    void usageLastDay(std::function<void(int)> cb);
    void canPerform(int weight, std::function<void(bool)> cb);

private:
    mutable QMutex m_mutex;

    bool enableRateLimit = true;

    int serverPort;
    DbServer* m_server = nullptr;
    DbClient* m_client = nullptr;

    DownloadManager* m_downloadManager = nullptr;
    DownloadModel* m_downloadModel = nullptr;
    DownloadWorker* m_downloadWorker = nullptr;
    ProviderManager* m_providerManager = nullptr;
    ToolsManager* m_toolsManager = nullptr;
    UpdateChecker* m_updateChecker = nullptr;
    URLInfoManager* m_urlInfoManager = nullptr;

    QSet<qint64> downloadingIds;
};
