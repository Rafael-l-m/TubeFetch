#include "Backend.hpp"
#include "core/APP.hpp"
#include "core/BasicTools.hpp"
#include "SharedStorage.hpp"


// Public

Backend::Backend(QObject* parent) : QObject(parent) {
    this->dbServer = new DbServer(this);

    const auto serverStarted = this->dbServer->start();

    if (serverStarted) {
        this->serverPort = this->dbServer->getPort();
        this->dbClient = new DbClient(this->serverPort, this);

        this->m_downloadManager = new DownloadManager(3, this);

        connect(
            this->m_downloadManager,
            &DownloadManager::downloadTaskChanged,
            this,
            [this](const QSharedPointer<Download>& dt) {
                this->dm->refresh(dt);

                const auto _internalId = dt->getInternalId();

                this->dbClient->updateDownloadStatus(_internalId, dt->getDownloadStatus(), [](const QJsonObject&) {});

                this->dbClient->updateProgress(_internalId, dt->getProgress(), [](const QJsonObject&) {});

                this->dbClient->updateDownloadedBytes(_internalId, dt->getDownloadedBytes(), [](const QJsonObject&) {});

                this->dbClient->updateTotalBytes(_internalId, dt->getTotalBytes(), [](const QJsonObject&j) {});

                this->dbClient->updateDownloadSpeed(_internalId, dt->getDownloadSpeed(), [](const QJsonObject&) {});

                this->dbClient->updateEta(_internalId, dt->getEta().trimmed(), [](const QJsonObject&) {});
            }
        );

        connect(
            this->m_downloadManager,
            &DownloadManager::downloadNormalFinished,
            this,
            [this](const qint64 internalId){       
                APP::messageCenter()->sendDebug(
                    QString("Download finished normally with internalId: %1")
                        .arg(internalId)
                );

                this->dbClient->updateDownloadStatus(internalId, DownloadStatus::FINISHED, [](const QJsonObject&){});
            }
        );

        connect(
            this->m_downloadManager,
            &DownloadManager::downloadUnnormalFinished,
            this,
            [this](const qint64 internalId){
                APP::messageCenter()->sendWarning(
                    QString("Download finished unnormally with internalId: %1")
                        .arg(internalId)
                );

                this->dbClient->updateDownloadStatus(internalId, DownloadStatus::ERROR, [](const QJsonObject&){});
            }
        );

        connect(
            this->m_downloadManager,
            &DownloadManager::downloadErrorOccurred,
            this,
            [this](const qint64 internalId, const QString& errorMessage){
                APP::messageCenter()->sendError(
                    QString("Error occurred with internalId: %1")
                        .arg(internalId)
                );

                APP::messageCenter()->sendError(errorMessage);

                this->dbClient->updateDownloadStatus(internalId, DownloadStatus::ERROR, [this](const QJsonObject&){});

                emit downloadFailed(internalId, errorMessage);
            }
        );

        connect(
            this->m_downloadManager,
            &DownloadManager::downloadAllFinished,
            this,
            [](){
                APP::messageCenter()->sendDebug("All downloads have done");
            }
        );
    }

    else { APP::messageCenter()->sendError("DbServer failed started"); }
}


// Load Database

void Backend::loadDatabase() {
    this->dbClient->readAllDownloads([this](const QJsonObject& obj) {
        APP::messageCenter()->sendDebug(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));

        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
        const auto ds_json = obj[JSON_FORMAT::PARAMETERS].toObject();

        if (status) {
            qint64 tmp_id = 0;

            const auto t = Download::fromJsonList(ds_json);

            for (const auto& it : t) {
                const auto tmp_internalId = it.getInternalId();

                if (tmp_internalId > tmp_id) { tmp_id = tmp_internalId; }

                const auto dynamic_dw = QSharedPointer<Download>::create(it);

                SharedStorage::instance().addDownload(dynamic_dw);
            }

            if (tmp_id != 0) { nextInternalId = ++tmp_id; }
        }

        else {
            const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();
            emit APP::messageCenter()->sendError(message);
        }

        this->dm->setDownloads(SharedStorage::instance().getDownloads());
    });
}

Q_INVOKABLE double Backend::doubleRound2(const double value) { return std::round(value * 100) / 100; }


// Download Model

void Backend::setDownloadModel(DownloadModel* m) { this->dm = m; }


// Import / Export Data

void Backend::exportData(const QUrl& filePath) {
    const auto fp = filePath.toLocalFile().trimmed();

    if (fp.isEmpty()) { emit dataExported(false, tr("Save Path is empty")); return; }

    const auto dws = SharedStorage::instance().getDownloads();

    if (!dws) { emit dataExported(false, tr("Download data is unavailable")); return; }

    if (dws->isEmpty()) { emit dataExported(false, tr("No data can be exported")); return; }

    const auto success = EXPORT_IMPORT_DATA::exportData<QList<QSharedPointer<Download>>>(*dws, fp);

    if (success) { emit dataExported(true, tr("Data exported successfully")); }

    else { emit dataExported(false, tr("Failed to export data")); }
}

void Backend::importData(const QUrl& filePath) {
    const auto fp = filePath.toLocalFile().trimmed();

    if (fp.isEmpty()) { emit dataImported(false, tr("Read Path is empty")); return; }

    const auto sz = SharedStorage::instance().getDownloadsSize();

    if (sz > 0) { emit dataImported(false, tr("All downloads should be cleared if you wanna import data")); return; }

    QList<QSharedPointer<Download>> t;

    const auto success = EXPORT_IMPORT_DATA::importData<QList<QSharedPointer<Download>>>(t, fp);

    if (success) {
        qint64 tmp_internalId = 0;

        for (const auto& it : t) {
            if (it.isNull()) {
                APP::messageCenter()->sendWarning("Import skipped: null Download");
                continue;
            }

            const auto tmp_ii = it->getInternalId();
            if (tmp_internalId < tmp_ii) { tmp_internalId = tmp_ii; }

            this->dbClient->addDownload(*it, [this, it](const QJsonObject& obj){
                APP::messageCenter()->sendDebug(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));

                const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
                const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
                const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

                if (status) { this->dm->append(it); }

                else { APP::messageCenter()->sendError(message); }
            });
        }

        nextInternalId = ++tmp_internalId;

        emit dataImported(true, tr("Data loaded successfully"));
    }

    else { emit dataImported(false, tr("Failed to load data")); }
}


// Tools Path

bool Backend::needConfig() {
    const auto yt_dlp_path = SYS_INFO::CONFIG::readConfig<QString>("tools/yt_dlp_path").trimmed();
    const auto ffmpeg_path = SYS_INFO::CONFIG::readConfig<QString>("tools/ffmpeg_path").trimmed();
    const auto node_path = SYS_INFO::CONFIG::readConfig<QString>("tools/node_path").trimmed();

    if (yt_dlp_path.isEmpty() || ffmpeg_path.isEmpty() || node_path.isEmpty()) {
        QJsonObject obj;

        obj.insert("yt_dlp", yt_dlp_path);
        obj.insert("ffmpeg", ffmpeg_path);
        obj.insert("node", node_path);

        return true;
    }

    else {
        SharedStorage::instance().setToolsPath({yt_dlp_path, ffmpeg_path, node_path});
        return false;
    }
}

void Backend::findToolsPath() {
    QJsonObject obj;

    auto yt_dlp_path = SYS_INFO::CONFIG::readConfig<QString>("tools/yt_dlp_path").trimmed();
    auto ffmpeg_path = SYS_INFO::CONFIG::readConfig<QString>("tools/ffmpeg_path").trimmed();
    auto node_path = SYS_INFO::CONFIG::readConfig<QString>("tools/node_path").trimmed();

    if (yt_dlp_path.isEmpty()) { yt_dlp_path = SYS_INFO::findToolPath("yt-dlp").trimmed(); }
    if (ffmpeg_path.isEmpty()) { ffmpeg_path = SYS_INFO::findToolPath("ffmpeg").trimmed(); }
    if (node_path.isEmpty())   { node_path = SYS_INFO::findToolPath("node").trimmed();     }

    obj.insert("yt_dlp", yt_dlp_path);
    obj.insert("ffmpeg", ffmpeg_path);
    obj.insert("node", node_path);

    emit toolsPathFound(obj);
}

void Backend::checkYtDlp(const QUrl& filePath) {
    const QRegularExpression re(R"(^\d{4}\.\d{2}\.\d{2}$)", QRegularExpression::CaseInsensitiveOption);

    if (this->checkExecutable(filePath)) {
        const auto path = filePath.toLocalFile();

        auto* process = new QProcess(this);
        connect(process, &QProcess::finished, this, [re, path, process, this](int exitCode, QProcess::ExitStatus status) {
            const auto output = process->readAllStandardOutput() + process->readAllStandardError();

            if (re.match(output.trimmed()).hasMatch()) { emit ytDlpChecked(true, true, path); }
            else { emit ytDlpChecked(true, false, {}); }

            process->deleteLater();
        });

        process->start(path, {"--version"});
    }

    else { emit ytDlpChecked(false, false, {}); }
}

void Backend::checkFFmpeg(const QUrl& filePath) {
    if (this->checkExecutable(filePath)) {
        const auto path = filePath.toLocalFile();

        auto* process = new QProcess(this);
        connect(process, &QProcess::finished, this, [path, process, this](int exitCode, QProcess::ExitStatus status) {
            const auto output = process->readAllStandardOutput() + process->readAllStandardError();

            if (output.trimmed().contains("ffmpeg")) { emit ffmpegChecked(true, true, path); }
            else { emit ffmpegChecked(true, false, {}); }

            process->deleteLater();
        });

        process->start(path, {"-version"});
    }

    else { emit ffmpegChecked(false, false, {}); }
}

void Backend::checkNode(const QUrl& filePath) {
    const QRegularExpression re(R"(^v\d+\.\d+\.\d+$)", QRegularExpression::CaseInsensitiveOption);

    if (this->checkExecutable(filePath)) {
        const auto path = filePath.toLocalFile();

        auto* process = new QProcess(this);
        connect(process, &QProcess::finished, this, [re, path, process, this](int exitCode, QProcess::ExitStatus status) {
            const auto output = process->readAllStandardOutput() + process->readAllStandardError();

            if (re.match(output.trimmed()).hasMatch()) { emit nodeChecked(true, true, path); }
            else { emit nodeChecked(true, false, {}); }

            process->deleteLater();
        });

        process->start(path, {"--version"});
    }

    else { emit nodeChecked(false, false, {}); }
}

void Backend::saveToolsPath(const QString& yt_dlp_path, const QString& ffmpeg_path, const QString& node_path) {
    const auto ytDlpPath = yt_dlp_path.trimmed();
    const auto ffmpegPath = ffmpeg_path.trimmed();
    const auto nodePath = node_path.trimmed();

    if (!ytDlpPath.isEmpty() && !ffmpegPath.isEmpty() && !nodePath.isEmpty()) {
        SharedStorage::instance().setToolsPath({ytDlpPath, ffmpegPath, nodePath});

        SYS_INFO::CONFIG::writeConfig(
            {
                {"tools/yt_dlp_path", ytDlpPath},
                {"tools/ffmpeg_path", ffmpegPath},
                {"tools/node_path", nodePath}
            }
        );

        emit toolsPathSaved(true, {});
    }

    else emit toolsPathSaved(false, tr("Tools path can not be empty"));
}


// Add Download

void Backend::showItags(
    const QString& url,
    const QString& yt_dlp_path,
    const QString& ffmpeg_path,
    const QString& node_path)
{
    const auto toolsPath = SharedStorage::instance().getToolsPath().data();

    const auto ytDlpPath = yt_dlp_path.trimmed().isEmpty() ? toolsPath->getYtDlpPath() : yt_dlp_path;
    const auto ffmpegPath = ffmpeg_path.trimmed().isEmpty() ? toolsPath->getFFmpegPath() : ffmpeg_path;
    const auto nodePath = node_path.trimmed().isEmpty() ? toolsPath->getNodePath() : node_path;

    auto* process = new QProcess();

    const QStringList arguments{
        "--ffmpeg-location", ffmpegPath,
        "--js-runtimes", "node:" + nodePath,
        "--no-playlist",
        "-F", url.trimmed()
    };

    connect(process, &QProcess::readyReadStandardOutput, this, [process, this](){
        const QString output = process->readAllStandardOutput();
        emit itagsShown(output);
    });

    connect(process, &QProcess::readyReadStandardError, this, [process, this](){
        const QString error = process->readAllStandardError();
        emit itagsShown(error);
    });

    connect(process, &QProcess::finished, this, [process](){
        process->deleteLater();
    });

    process->start(ytDlpPath, arguments);
}

void Backend::askAudioFilters() { emit filtersAsked(MediaCodecFilters::audioFilters); }

void Backend::askVideoFilters() { emit filtersAsked(MediaCodecFilters::videoFilters); }

void Backend::checkAvailablePath(const QUrl& path) {
    const auto realPath = path.toLocalFile().trimmed();
    emit availablePathChecked(SYS_INFO::checkPathWritable(realPath), realPath);
}

void Backend::addNewDownload(
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
    const double progress,
    const DownloadStatus downloadState,
    const double downloadedBytes,
    const double totalBytes,
    const double downloadSpeed,
    const QString& eta)
{
    Download dw {
        nextInternalId++,
        url.trimmed(),
        title.trimmed(),
        audioCode.trimmed(),
        videoCode.trimmed(),
        nonDashCode.trimmed(),
        bestAudio,
        bestVideo,
        savePath.trimmed(),
        QFileInfo(suffix.trimmed()).suffix().trimmed().toLower(),
        metadata,
        subtitles,
        doubleRound2(progress),
        downloadState,
        doubleRound2(downloadedBytes),
        doubleRound2(totalBytes),
        doubleRound2(downloadSpeed),
        eta.trimmed()
    };

    this->dbClient->addDownload(dw, [this, dw](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
        const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

        emit newDownloadAdded(status, message);

        if (status) {
            const auto dynamic_dw = QSharedPointer<Download>::create(dw);
            this->dm->append(dynamic_dw);
        }

        else { APP::messageCenter()->sendError(message); }
    });
}


// Remove Download

void Backend::removeDownload(const qint64 internalId) {
    {
        QMutexLocker locker(&this->m_mutex);

        if (this->m_downloadManager->isRunning()) {
            emit isAlreadyDownloading();
            return;
        }
    }

    const auto dw = SharedStorage::instance().getDownload(internalId);

    if (dw.isNull()) { APP::messageCenter()->sendError("Critical Error: internalId -> null"); return; }

    this->dbClient->removeDownload(internalId, [this, dw, internalId](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;

        if (status) {
            this->dm->removeByInternalId(internalId);
            emit downloadRemoved(true, {});
        }

        else {
            const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();
            APP::messageCenter()->sendError(message);
            emit downloadRemoved(false, message);
        }
    });
}

void Backend::removeAllDownloads() {
    {
        QMutexLocker locker(&this->m_mutex);

        if (this->m_downloadManager->isRunning()) {
            emit isAlreadyDownloading();
            return;
        }
    }

    const auto sz = SharedStorage::instance().getDownloadsSize();

    if (sz <= 0) {
        emit downloadsRemoved(false, tr("No downloads to remove"));
        return;
    }

    this->dbClient->removeAllDownloads([this](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;

        if (status) {
            this->dm->clear();
            emit downloadsRemoved(true, {});
        }

        else {
            const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();
            APP::messageCenter()->sendError(message);
            emit downloadsRemoved(false, message);
        }
    });
}


// Edit Download

void Backend::requestEditInformation(const qint64 internalId) {
    {
        QMutexLocker locker(&this->m_mutex);

        if (this->m_downloadManager->isRunning()) {
            emit isAlreadyDownloading();
            return;
        }
    }

    if (internalId <= 0) {
        APP::messageCenter()->sendError(
            QString("Invalid internalId: %1")
                .arg(internalId)
        );

        return;
    }

    const auto dynamic_dt = SharedStorage::instance().getDownload(internalId);

    if (dynamic_dt.isNull()) {
        APP::messageCenter()->sendError("Critical Error: Download is null");
        return;
    }

    emit editInformationRequest(dynamic_dt->toJson(), {});
}

void Backend::editDownload(
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
    const double progress,
    const DownloadStatus downloadState,
    const double downloadedBytes,
    const double totalBytes,
    const double downloadSpeed,
    const QString& eta)
{
    Download dw {
        internalId,
        url.trimmed(),
        title.trimmed(),
        audioCode.trimmed(),
        videoCode.trimmed(),
        nonDashCode.trimmed(),
        bestAudio,
        bestVideo,
        savePath.trimmed(),
        QFileInfo(suffix.trimmed()).suffix().trimmed().toLower(),
        metadata,
        subtitles,
        doubleRound2(progress),
        downloadState,
        doubleRound2(downloadedBytes),
        doubleRound2(totalBytes),
        doubleRound2(downloadSpeed),
        eta.trimmed()
    };

    this->dbClient->removeDownload(internalId, [this, dw, internalId](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
        const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

        if (status) {
            this->dbClient->addDownload(dw, [this, dw, internalId](const QJsonObject& obj){
                const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
                const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
                const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

                emit newDownloadAdded(status, message);

                if (status) {
                    const auto dt = SharedStorage::instance().getDownload(internalId);

                    *dt = dw;

                    this->dm->setDownloads(SharedStorage::instance().getDownloads());
                }

                else { APP::messageCenter()->sendError(message); }
            });
        }

        else { APP::messageCenter()->sendError(message); }
    });
}


// Start Download

void Backend::startDownload(const qint64 internalId) {
    {
        QMutexLocker locker(&this->m_mutex);

        if (this->m_downloadManager->isRunning()) {
            emit isAlreadyDownloading();
            return;
        }
    }

    const auto task = SharedStorage::instance().getDownload(internalId);

    if (!task) { return; }

    this->m_downloadManager->start(task);
}

void Backend::startDownloads() {
    {
        QMutexLocker locker(&this->m_mutex);

        if (this->m_downloadManager->isRunning()) {
            emit isAlreadyDownloading();
            return;
        }
    }

    const auto sz = SharedStorage::instance().getDownloadsSize();

    if (sz <= 0) {
        emit noAvailableDownloads();
        return;
    }

    this->m_downloadManager->startAll();
}


// Stop Download

void Backend::stopDownloads() {
    {
        QMutexLocker locker(&this->m_mutex);

        if (!this->m_downloadManager->isRunning()) {
            emit noDownloading();
            return;
        }
    }

    this->m_downloadManager->stopAll();

    const auto ds = SharedStorage::instance().getDownloads();

    for (const auto& it : *ds) {
        const auto _ds = it->getDownloadStatus();

        if (_ds != DownloadStatus::FINISHED && _ds != DownloadStatus::ERROR) {
            it->setDownloadStatus(DownloadStatus::PAUSED);

            this->dbClient->updateDownloadStatus(
                it->getInternalId(),
                DownloadStatus::PAUSED,
                [this, it](const QJsonObject&) {
                    this->dm->refresh(it);
                }
            );
        }
    }
}


// Clear Download Status

void Backend::clearAllStatus() {
    const auto ds = SharedStorage::instance().getDownloads();

    if (ds.isNull() || ds->isEmpty()) { return; }

    for (const auto& it : *ds) {
        it->setProgress(0.0);
        it->setDownloadStatus(DownloadStatus::WAITING);
        it->setDownloadedBytes(0.0);
        it->setTotalBytes(0.0);
        it->setDownloadSpeed(0.0);
        it->setEta("00:00");

        this->dm->refresh(it);

        const auto _internalId = it->getInternalId();

        this->dbClient->updateProgress(_internalId, 0.0, [](const QJsonObject&){});

        this->dbClient->updateDownloadStatus(_internalId, DownloadStatus::WAITING, [](const QJsonObject&){});

        this->dbClient->updateDownloadedBytes(_internalId, 0.0, [](const QJsonObject&){});

        this->dbClient->updateTotalBytes(_internalId, 0.0, [](const QJsonObject&){});

        this->dbClient->updateDownloadSpeed(_internalId, 0.0, [](const QJsonObject&){});

        this->dbClient->updateEta(_internalId, "00:00", [](const QJsonObject&){});
    }
}


// Quit App

void Backend::requestQuitApp() { this->m_downloadManager->stopAll(); }


// Private

bool Backend::checkExecutable(const QUrl& filePath) {
    QFileInfo fi(filePath.toLocalFile());
    return fi.isExecutable();
}
