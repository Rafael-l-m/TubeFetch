#pragma once

#include <db/Operation.hpp>
#include <db/Specification.hpp>
#include <download/Download.hpp>
#include <download/YoutubeActions.hpp>
#include <QQueue>
#include <QTcpSocket>

class DbClient final : public QObject {
    Q_OBJECT

public:
    explicit DbClient(quint16 port, QObject* parent = nullptr);

    void verifyState(std::function<void(QJsonObject)> cb);

    void addDownload(const Download& download, std::function<void(QJsonObject)> cb);
    void addYoutubeAction(const YoutubeActions& youtubeAction, std::function<void(QJsonObject)> cb);
    void removeDownload(qint64 internalId, std::function<void(QJsonObject)> cb);
    void removeAllDownloads(std::function<void(QJsonObject)> cb);
    void removeUselessActions(qint64 timeLeft, std::function<void(QJsonObject)> cb);

    void updateUrl(qint64 internalId, const QString& url, std::function<void(QJsonObject)> cb);
    void updateTitle(qint64 internalId, const QString& title, std::function<void(QJsonObject)> cb);
    void updateAudioCode(qint64 internalId, const QString& audioCode, std::function<void(QJsonObject)> cb);
    void updateVideoCode(qint64 internalId, const QString& videoCode, std::function<void(QJsonObject)> cb);
    void updateNonDashCode(qint64 internalId, const QString& nonDashCode, std::function<void(QJsonObject)> cb);
    void updateBestAudio(qint64 internalId, bool bestAudio, std::function<void(QJsonObject)> cb);
    void updateBestVideo(qint64 internalId, bool bestVideo, std::function<void(QJsonObject)> cb);
    void updateSavePath(qint64 internalId, const QString& savePath, std::function<void(QJsonObject)> cb);
    void updateSuffix(qint64 internalId, const QString& suffix, std::function<void(QJsonObject)> cb);
    void updateMetadata(qint64 internalId, bool metadata, std::function<void(QJsonObject)> cb);
    void updateSubtitles(qint64 internalId, bool subtitles, std::function<void(QJsonObject)> cb);
    void updateProgress(qint64 internalId, double progress, std::function<void(QJsonObject)> cb);
    void updateDownloadStatus(qint64 internalId, const DownloadStatus& downloadStatus, std::function<void(QJsonObject)> cb);
    void updateDownloadedBytes(qint64 internalId, double downloadedBytes, std::function<void(QJsonObject)> cb);
    void updateTotalBytes(qint64 internalId, double totalBytes, std::function<void(QJsonObject)> cb);
    void updateDownloadSpeed(qint64 internalId, double downloadSpeed, std::function<void(QJsonObject)> cb);
    void updateEta(qint64 internalId, const QString& eta, std::function<void(QJsonObject)> cb);

    void readDownload(qint64 internalId, std::function<void(QJsonObject)> cb);
    void readAllDownloads(std::function<void(QJsonObject)> cb);
    void getWeights(qint64 timeLeft, std::function<void(QJsonObject)> cb);

    ~DbClient() override;

private slots:
    void onReadyRead();

private:
    struct Pending {
        Operation op;
        Specification spec;
        QJsonObject params;
        std::function<void(QJsonObject)> cb;
    };

    void sendRequest(
        const Operation& op,
        const Specification& spec,
        const QJsonObject& params,
        std::function<void(QJsonObject)> cb
    );

    void connectToServer() const;
    void reconnect() const;
    void flushPending();

private:
    inline static int nextId = 1;
    quint16 port;
    QTcpSocket* socket;
    QQueue<Pending> pending;
    QMap<int, std::function<void(QJsonObject)>> callbacks;
};
