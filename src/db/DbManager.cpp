#include <core/Constants.hpp>
#include <db/DbManager.hpp>


// Public

namespace C {
    qint64 _getInternalId(const QJsonObject& obj) {
        return obj[JSON_FORMAT::INTERNAL_ID]
            .toVariant()
            .toLongLong();
    }
}

DbManager::DbManager(QObject* parent) : DataBase(parent) { DataBase::init(); }

QJsonObject DbManager::isOpenDB() const {
    QJsonObject obj;
    obj[JSON_FORMAT::STATUS] = DataBase::isOpen() ? JSON_FORMAT::STATUS_ACCEPTED : JSON_FORMAT::STATUS_REFUSED;
    return obj;
}

QJsonObject DbManager::addDownload(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id already exists");
    }

    else {
        const auto d = Download::fromJson(obj);

        if (!DataBase::add(d)) {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to add download: Operation rejected by database");
        }

        else { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }
    }

    return res;
}

QJsonObject DbManager::addYoutubeAction(const QJsonObject& obj) const {
    QJsonObject res;

    const auto d = YoutubeActions::fromJson(obj);

    if (!DataBase::addYoutubeAction(d)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Failed to add youtube action: Operation rejected by database");
    }

    else { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

    return res;
}

QJsonObject DbManager::removeDownload(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        if (DataBase::remove(internalId)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Database is empty");
        }
    }

    return res;
}

QJsonObject DbManager::removeAllDownloads() const {
    QJsonObject res;

    if (DataBase::removeAll()) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

    else {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Database is empty");
    }

    return res;
}

QJsonObject DbManager::removeUselessActions(const QJsonObject& obj) const {
    QJsonObject res;

    const auto tl = obj[JSON_FORMAT::TIME_LEFT].toVariant().toLongLong();

    if (DataBase::removeUselessActions(tl)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

    else {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("No items match the deletion criteria");
    }

    return res;
}

QJsonObject DbManager::updateUrl(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newUrl = obj[JSON_FORMAT::URL].toString().trimmed();

        if (DataBase::updateUrl(internalId, newUrl)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update url: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateTitle(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newTitle = obj[JSON_FORMAT::TITLE].toString().trimmed();

        if (DataBase::updateTitle(internalId, newTitle)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update title: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateAudioCode(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newAudioCode = obj[JSON_FORMAT::AUDIO_CODE].toString().trimmed();

        if (DataBase::updateAudioCode(internalId, newAudioCode)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update audio code: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateVideoCode(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newVideoCode = obj[JSON_FORMAT::VIDEO_CODE].toString().trimmed();

        if (DataBase::updateVideoCode(internalId, newVideoCode)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update video code: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateNonDashCode(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newNonDashCode = obj[JSON_FORMAT::NON_DASH_CODE].toString().trimmed();

        if (DataBase::updateNonDashCode(internalId, newNonDashCode)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update non dash code: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateBestAudio(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newBestAudio = obj[JSON_FORMAT::BEST_AUDIO].toBool();

        if (DataBase::updateBestAudio(internalId, newBestAudio)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update bestAudio: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateBestVideo(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newBestVideo = obj[JSON_FORMAT::BEST_VIDEO].toBool();

        if (DataBase::updateBestVideo(internalId, newBestVideo)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update bestVideo: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateSavePath(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newSavePath = obj[JSON_FORMAT::SAVE_PATH].toString().trimmed();

        if (DataBase::updateSavePath(internalId, newSavePath)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update save path: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateSuffix(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newSuffix = obj[JSON_FORMAT::SUFFIX].toString().trimmed();

        if (DataBase::updateSuffix(internalId, newSuffix)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update suffix: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateMetadata(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto met = obj[JSON_FORMAT::METADATA].toBool();

        if (DataBase::updateMetadata(internalId, met)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update metadata: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateSubtitles(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto subt = obj[JSON_FORMAT::SUBTITLES].toBool();

        if (DataBase::updateSubtitles(internalId, subt)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update subtitles: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateProgress(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newProgress = obj[JSON_FORMAT::PROGRESS].toDouble();

        if (DataBase::updateProgress(internalId, newProgress)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update progress: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateDownloadStatus(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newDownloadStatus = parseDownloadStatus(obj[JSON_FORMAT::DOWNLOAD_STATUS].toInt());

        if (DataBase::updateDownloadStatus(internalId, newDownloadStatus)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update download status: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateDownloadedBytes(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newDownloadedBytes = obj[JSON_FORMAT::DOWNLOADED_BYTES].toDouble();

        if (DataBase::updateDownloadedBytes(internalId, newDownloadedBytes)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update downloaded bytes: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateTotalBytes(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newTotalBytes = obj[JSON_FORMAT::TOTAL_BYTES].toDouble();

        if (DataBase::updateTotalBytes(internalId, newTotalBytes)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update total bytes: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateDownloadSpeed(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newDownloadSpeed = obj[JSON_FORMAT::DOWNLOAD_SPEED].toDouble();

        if (DataBase::updateDownloadSpeed(internalId, newDownloadSpeed)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update download speed: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::updateEta(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        const auto newEta = obj[JSON_FORMAT::ETA].toString().trimmed();

        if (DataBase::updateEta(internalId, newEta)) { res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED; }

        else {
            res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
            res[JSON_FORMAT::MESSAGE] = tr("Failed to update eta: Operation rejected by database");
        }
    }

    return res;
}

QJsonObject DbManager::readDownload(const QJsonObject& obj) const {
    QJsonObject res;

    const auto internalId = C::_getInternalId(obj);

    if (!DataBase::ifExists(internalId)) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Critical Error: internal id not found");
    }

    else {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED;
        res[JSON_FORMAT::PARAMETERS] = read(internalId).toJson();
    }

    return res;
}

QJsonObject DbManager::readAllDownloads() const {
    QJsonObject res;

    if (DataBase::isEmpty()) {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_REFUSED;
        res[JSON_FORMAT::MESSAGE] = tr("Database is empty");
    }

    else {
        res[JSON_FORMAT::STATUS] = JSON_FORMAT::STATUS_ACCEPTED;
        res[JSON_FORMAT::PARAMETERS] = Download::toJsonList(readAll());
    }

    return res;
}

QJsonObject DbManager::getWeights(const QJsonObject& obj) const {
    QJsonObject res;

    const auto tl = obj[JSON_FORMAT::TIME_LEFT].toVariant().toLongLong();
    const auto weights = DataBase::getWeight(tl);

    res[JSON_FORMAT::WEIGHTS] = weights;

    return res;
}
