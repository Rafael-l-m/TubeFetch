#include <core/Constants.hpp>
#include <db/DbServer.hpp>


// Public

DbServer::DbServer(QObject* parent) : QObject(parent), port(0) {
    this->manager = new DbManager(this);

    this->handlers[Operation::VerifyState] = [this](const DbRequest& req) {
        auto response = this->manager->isOpenDB();
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::AddInformation] = [this](const DbRequest& req) {
        auto response = this->manager->addDownload(req.params);
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::AddYoutubeAction] = [this](const DbRequest& req) {
        auto response = this->manager->addYoutubeAction(req.params);
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::DeleteInformation] = [this](const DbRequest& req) {
        auto response = this->manager->removeDownload(req.params);
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::DeleteAllInformation] = [this](const DbRequest& req) {
        auto response = this->manager->removeAllDownloads();
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::DeleteUselessActions] = [this](const DbRequest& req) {
        auto response = this->manager->removeUselessActions(req.params);
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::UpdateInformation] = [this](const DbRequest& req) {
        QJsonObject response;

        switch (req.spec) {
            case Specification::Url:
                response = this->manager->updateUrl(req.params);
                break;
            case Specification::Title:
                response = this->manager->updateTitle(req.params);
                break;
            case Specification::AudioCode:
                response = this->manager->updateAudioCode(req.params);
                break;
            case Specification::VideoCode:
                response = this->manager->updateVideoCode(req.params);
                break;
            case Specification::NonDashCode:
                response = this->manager->updateNonDashCode(req.params);
                break;
            case Specification::BestAudio:
                response = this->manager->updateBestAudio(req.params);
                break;
            case Specification::BestVideo:
                response = this->manager->updateBestVideo(req.params);
                break;
            case Specification::SavePath:
                response = this->manager->updateSavePath(req.params);
                break;
            case Specification::Suffix:
                response = this->manager->updateSuffix(req.params);
                break;
            case Specification::Metadata:
                response = this->manager->updateMetadata(req.params);
                break;
            case Specification::Subtitles:
                response = this->manager->updateSubtitles(req.params);
                break;
            case Specification::Progress:
                response = this->manager->updateProgress(req.params);
                break;
            case Specification::DownloadState:
                response = this->manager->updateDownloadStatus(req.params);
                break;
            case Specification::DownloadedBytes:
                response = this->manager->updateDownloadedBytes(req.params);
                break;
            case Specification::TotalBytes:
                response = this->manager->updateTotalBytes(req.params);
                break;
            case Specification::DownloadSpeed:
                response = this->manager->updateDownloadSpeed(req.params);
                break;
            case Specification::Eta:
                response = this->manager->updateEta(req.params);
                break;
            default:
                break;
        }

        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::GetInformation] = [this](const DbRequest& req) {
        auto response = this->manager->readDownload(req.params);
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::GetAllInformation] = [this](const DbRequest& req) {
        auto response = this->manager->readAllDownloads();
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };

    this->handlers[Operation::GetWeights] = [this](const DbRequest& req) {
        auto response = this->manager->getWeights(req.params);
        response[JSON_FORMAT::ID] = req.id;
        this->sendResponse(req, response);
    };
}

bool DbServer::start() {
    connect(&this->server, &QTcpServer::newConnection, this, &DbServer::onNewConnection);
    return this->server.listen(QHostAddress::LocalHost, 0);
}

int DbServer::getPort() const { return this->server.serverPort(); }


// Private Slots

void DbServer::onNewConnection() {
    while (this->server.hasPendingConnections()) {
        const QTcpSocket* socket = this->server.nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, &DbServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }
}

void DbServer::onReadyRead() {
    auto* socket = qobject_cast<QTcpSocket*>(sender());

    if (!socket) { return; }

    while (socket->canReadLine()) {
        QJsonParseError err;

        auto data = socket->readLine();
        auto doc = QJsonDocument::fromJson(data, &err);

        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            this->sendError(
                socket,
                -1,
                tr("Invalid JSON")
                );

            continue;
        }

        auto obj = doc.object();

        const auto id = obj[JSON_FORMAT::ID].toInt();
        const auto op = parseOperation(obj[JSON_FORMAT::OPERATION].toString().trimmed());
        const auto spec = parseSpecification(obj[JSON_FORMAT::SPECIFICATION].toString().trimmed());

        if (op == Operation::Unknown) {
            this->sendError(
                socket,
                id,
                tr("Invalid operation: unknown operation")
                );
        }

        if (op == Operation::UpdateInformation && spec == Specification::Unknown) {
            this->sendError(
                socket,
                id,
                tr("Invalid specification: unknown specification")
                );
        }

        DbRequest req {
            .id = id,
            .op = op,
            .spec = spec,
            .params = obj[JSON_FORMAT::PARAMETERS].toObject(),
            .socket = socket
        };

        if (this->handlers.contains(op)) { this->handlers[op](req); }
    }
}


// Private

void DbServer::sendResponse(const DbRequest& req, const QJsonObject& data) {
    if (!req.socket) { return; }
    data[JSON_FORMAT::ID] = req.id;
    req.socket->write(QJsonDocument(data).toJson(QJsonDocument::Compact) + "\n");
}

void DbServer::sendError(QTcpSocket* socket, const int id, const QString& msg) {
    QJsonObject res;

    res[JSON_FORMAT::ID] = id;
    res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
    res[JSON_FORMAT::MESSAGE] = msg;

    socket->write(QJsonDocument(res).toJson(QJsonDocument::Compact) + "\n");
}
