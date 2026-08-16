#include "core/Constants.hpp"
#include "db/DbClient.hpp"
#include <QTimer>


// Public

DbClient::DbClient(const quint16 port, QObject* parent) : QObject(parent), port(port) {
    this->socket = new QTcpSocket(this);

    connect(this->socket, &QTcpSocket::readyRead,    this, &DbClient::onReadyRead );
    connect(this->socket, &QTcpSocket::disconnected, this, &DbClient::reconnect   );
    connect(this->socket, &QTcpSocket::connected,    this, &DbClient::flushPending);

    this->connectToServer();
}

void DbClient::verifyState(std::function<void(QJsonObject)> cb) {
    this->sendRequest(
        Operation::VerifyState,
        Specification::Unknown,
        {},
        std::move(cb)
    );
}

void DbClient::addDownload(const Download& download, std::function<void(QJsonObject)> cb) {
    this->sendRequest(
        Operation::AddInformation,
        Specification::Unknown,
        download.toJson(),
        std::move(cb)
    );
}

void DbClient::removeDownload(const qint64 internalId, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;

    this->sendRequest(
        Operation::DeleteInformation,
        Specification::Unknown,
        params,
        std::move(cb)
    );
}

void DbClient::removeAllDownloads(std::function<void(QJsonObject)> cb) {
    this->sendRequest(
        Operation::DeleteAllInformation,
        Specification::Unknown,
        {},
        std::move(cb)
    );
}

void DbClient::updateUrl(const qint64 internalId, const QString& url, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::URL] = url;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::Url,
        params,
        std::move(cb)
    );
}

void DbClient::updateTitle(const qint64 internalId, const QString& title, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::TITLE] = title;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::Title,
        params,
        std::move(cb)
    );
}

void DbClient::updateAudioCode(const qint64 internalId, const QString& audioCode, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::AUDIO_CODE] = audioCode;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::AudioCode,
        params,
        std::move(cb)
    );
}

void DbClient::updateVideoCode(const qint64 internalId, const QString& videoCode, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::VIDEO_CODE] = videoCode;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::VideoCode,
        params,
        std::move(cb)
    );
}

void DbClient::updateNonDashCode(const qint64 internalId, const QString& nonDashCode, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::NON_DASH_CODE] = nonDashCode;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::NonDashCode,
        params,
        std::move(cb)
    );
}

void DbClient::updateBestAudio(const qint64 internalId, const bool bestAudio, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::BEST_AUDIO] = bestAudio;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::BestAudio,
        params,
        std::move(cb)
    );
}

void DbClient::updateBestVideo(const qint64 internalId, const bool bestVideo, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::BEST_VIDEO] = bestVideo;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::BestVideo,
        params,
        std::move(cb)
    );
}

void DbClient::updateSavePath(const qint64 internalId, const QString& savePath, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::SAVE_PATH] = savePath;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::SavePath,
        params,
        std::move(cb)
    );
}

void DbClient::updateSuffix(const qint64 internalId, const QString& suffix, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::SUFFIX] = suffix;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::Suffix,
        params,
        std::move(cb)
    );
}

void DbClient::updateMetadata(const qint64 internalId, const bool metadata, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::METADATA] = metadata;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::Metadata,
        params,
        std::move(cb)
    );
}

void DbClient::updateSubtitles(const qint64 internalId, const bool subtitles, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::SUBTITLES] = subtitles;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::Subtitles,
        params,
        std::move(cb)
    );
}

void DbClient::updateProgress(const qint64 internalId, const double progress, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::PROGRESS] = progress;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::Progress,
        params,
        std::move(cb)
    );
}

void DbClient::updateDownloadStatus(const qint64 internalId, const DownloadStatus& downloadStatus, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::DOWNLOAD_STATUS] = downloadStatusToInt(downloadStatus);

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::DownloadState,
        params,
        std::move(cb)
    );
}

void DbClient::updateDownloadedBytes(const qint64 internalId, const double downloadedBytes, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::DOWNLOADED_BYTES] = downloadedBytes;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::DownloadedBytes,
        params,
        std::move(cb)
    );
}

void DbClient::updateTotalBytes(const qint64 internalId, const double totalBytes, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::TOTAL_BYTES] = totalBytes;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::TotalBytes,
        params,
        std::move(cb)
    );
}

void DbClient::updateDownloadSpeed(const qint64 internalId, const double downloadSpeed, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::DOWNLOAD_SPEED] = downloadSpeed;

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::DownloadSpeed,
        params,
        std::move(cb)
    );
}

void DbClient::updateEta(const qint64 internalId, const QString& eta, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;
    params[JSON_FORMAT::ETA] = eta.trimmed();

    this->sendRequest(
        Operation::UpdateInformation,
        Specification::Eta,
        params,
        std::move(cb)
    );
}

void DbClient::readDownload(const qint64 internalId, std::function<void(QJsonObject)> cb) {
    QJsonObject params;

    params[JSON_FORMAT::INTERNAL_ID] = internalId;

    this->sendRequest(
        Operation::GetInformation,
        Specification::Unknown,
        params,
        std::move(cb)
    );
}

void DbClient::readAllDownloads(std::function<void(QJsonObject)> cb) {
    this->sendRequest(
        Operation::GetAllInformation,
        Specification::Unknown,
        {},
        std::move(cb)
    );
}

DbClient::~DbClient() {
    if (this->socket) {
        this->socket->disconnectFromHost();
        this->socket->abort();
    }
}


// Private Slots

void DbClient::onReadyRead() {
    while (this->socket->canReadLine()) {
        auto doc = QJsonDocument::fromJson(this->socket->readLine());

        if (!doc.isObject()) { continue; }

        auto obj = doc.object();

        const auto id = obj[JSON_FORMAT::ID].toInt();

        if (this->callbacks.contains(id)) {
            auto cb = this->callbacks.take(id);
            cb(obj);
        }
    }
}


// Private

void DbClient::connectToServer() const {
    if (this->socket->state() == QAbstractSocket::ConnectedState) { return; }
    this->socket->connectToHost(QHostAddress::LocalHost, this->port);
}

void DbClient::reconnect() const { QTimer::singleShot(1000, this, [this]() { this->connectToServer(); }); }

void DbClient::flushPending() {
    while (!this->pending.empty()) {
        auto [op, spec, params, cb] = this->pending.dequeue();
        this->sendRequest(op, spec, params, cb);
    }
}

void DbClient::sendRequest(
    const Operation& op,
    const Specification& spec,
    const QJsonObject& params,
    std::function<void(QJsonObject)> cb)
{
    if (this->socket->state() != QAbstractSocket::ConnectedState) {
        this->pending.enqueue({op, spec, params, cb});
        return;
    }

    const auto id = nextId++;
    this->callbacks[id] = std::move(cb);

    QJsonObject obj;

    obj[JSON_FORMAT::ID] = id;
    obj[JSON_FORMAT::OPERATION] = opToString(op);
    obj[JSON_FORMAT::SPECIFICATION] = specToString(spec);
    obj[JSON_FORMAT::PARAMETERS] = params;

    this->socket->write(QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n");

    QTimer::singleShot(5000, this, [this, id]() {this->callbacks.remove(id);});
}
