#pragma once

#include "db/Operation.hpp"
#include "db/Specification.hpp"
#include "download/Download.hpp"
#include <QQueue>
#include <QTcpSocket>

class DbClient final : public QObject {
    Q_OBJECT

public:
    explicit DbClient(const quint16 port, QObject* parent = nullptr);

    void verifyState(std::function<void(QJsonObject)> cb);
    void addDownload(const Download& download, std::function<void(QJsonObject)> cb);

    void removeDownload(const qint64 internalId, std::function<void(QJsonObject)> cb);
    void removeAllDownloads(std::function<void(QJsonObject)> cb);

    void updateUrl(const qint64 internalId, const QString& url, std::function<void(QJsonObject)> cb);
    void updateTitle(const qint64 internalId, const QString& title, std::function<void(QJsonObject)> cb);
    void updateAudioCode(const qint64 internalId, const QString& audioCode, std::function<void(QJsonObject)> cb);
    void updateVideoCode(const qint64 internalId, const QString& videoCode, std::function<void(QJsonObject)> cb);
    void updateNonDashCode(const qint64 internalId, const QString& nonDashCode, std::function<void(QJsonObject)> cb);
    void updateBestAudio(const qint64 internalId, const bool bestAudio, std::function<void(QJsonObject)> cb);
    void updateBestVideo(const qint64 internalId, const bool bestVideo, std::function<void(QJsonObject)> cb);
    void updateSavePath(const qint64 internalId, const QString& savePath, std::function<void(QJsonObject)> cb);
    void updateSuffix(const qint64 internalId, const QString& suffix, std::function<void(QJsonObject)> cb);
    void updateMetadata(const qint64 internalId, const bool metadata, std::function<void(QJsonObject)> cb);
    void updateSubtitles(const qint64 internalId, const bool subtitles, std::function<void(QJsonObject)> cb);
    void updateProgress(const qint64 internalId, const double progress, std::function<void(QJsonObject)> cb);
    void updateDownloadStatus(const qint64 internalId, const DownloadStatus& downloadStatus, std::function<void(QJsonObject)> cb);
    void updateDownloadedBytes(const qint64 internalId, const double downloadedBytes, std::function<void(QJsonObject)> cb);
    void updateTotalBytes(const qint64 internalId, const double totalBytes, std::function<void(QJsonObject)> cb);
    void updateDownloadSpeed(const qint64 internalId, const double downloadSpeed, std::function<void(QJsonObject)> cb);
    void updateEta(const qint64 internalId, const QString& eta, std::function<void(QJsonObject)> cb);

    void readDownload(const qint64 internalId, std::function<void(QJsonObject)> cb);
    void readAllDownloads(std::function<void(QJsonObject)> cb);

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
