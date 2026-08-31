#include <Backend.hpp>
#include <core/APP.hpp>
#include <core/BasicTools.hpp>
#include <SharedStorage.hpp>


// Public

Backend::Backend(QObject* parent) : QObject(parent) {
    // Start DbServer
    this->m_server = new DbServer(this);

    if (!this->m_server->start()) {
        APP::messageCenter()->sendError(QString("Failed to start dbServer. Please restart"));
        return;
    }

    this->serverPort = this->m_server->getPort();

    APP::messageCenter()->sendDebug(QString("DbServer started on port: %1").arg(this->serverPort));

    // Create DbClient
    this->m_client = new DbClient(this->serverPort, this);

    // Start PO Token Provider
    this->m_providerManager = new ProviderManager(this);
    connect(this->m_providerManager, &ProviderManager::providerStarted, this, &Backend::poTokenProviderStarted);
    this->startPoTokenProvider();

    // Load Data
    this->loadDataBase();

    // Load Download Manager
    this->m_downloadManager = new DownloadManager(2, this);

    for (const auto& item : *(SharedStorage::instance().getDownloads())) { this->m_downloadManager->addDownload(item); }

    connect(this->m_downloadManager, &DownloadManager::requestConsume, this, [this](const qint64 internalId, const int type, const int weight){
        this->consume(internalId, type, weight);
    });

    connect(this->m_downloadManager, &DownloadManager::downloadModelChanged, this, [this](const QSharedPointer<Download>& dt){
        this->m_downloadModel->refresh(dt);

        const auto _internalId = dt->getInternalId();

        this->m_client->updateDownloadStatus(_internalId, dt->getDownloadStatus(), [](const QJsonObject&) {});

        this->m_client->updateProgress(_internalId, dt->getProgress(), [](const QJsonObject&) {});

        this->m_client->updateDownloadedBytes(_internalId, dt->getDownloadedBytes(), [](const QJsonObject&) {});

        this->m_client->updateTotalBytes(_internalId, dt->getTotalBytes(), [](const QJsonObject&j) {});

        this->m_client->updateDownloadSpeed(_internalId, dt->getDownloadSpeed(), [](const QJsonObject&) {});

        this->m_client->updateEta(_internalId, dt->getEta().trimmed(), [](const QJsonObject&) {});
    });

    connect(this->m_downloadManager, &DownloadManager::downloadNormalFinished, this, [this](const qint64 internalId){
        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::FINISHED);
        targetDownload->setEta("00:00");

        this->m_downloadModel->refresh(targetDownload);

        const auto savePath = targetDownload->getSavePath().trimmed();
        const auto fileName = QFileInfo(savePath).fileName();

        APP::messageCenter()->sendDebug(QString("Download normally finished: %1").arg(fileName));

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::FINISHED, [internalId, this](const QJsonObject&){});

        this->m_client->updateEta(internalId, "00:00", [internalId, this](const QJsonObject&){});

        emit downloadFinished(internalId);
    });

    connect(this->m_downloadManager, &DownloadManager::downloadUnnormalFinished, this, [this](const qint64 internalId) {
        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::ERROR);

        this->m_downloadModel->refresh(targetDownload);

        const auto savePath = targetDownload->getSavePath().trimmed();
        const auto fileName = QFileInfo(savePath).fileName();

        APP::messageCenter()->sendWarning(QString("Download unnormally finished: %1").arg(fileName));

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::ERROR, [internalId, this](const QJsonObject&){});            emit downloadFinished(internalId);

        emit downloadFinished(internalId);
    });

    connect(this->m_downloadManager, &DownloadManager::downloadPaused, this, [this](const qint64 internalId) {
        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::PAUSED);

        this->m_downloadModel->refresh(targetDownload);

        const auto savePath = targetDownload->getSavePath().trimmed();
        const auto fileName = QFileInfo(savePath).fileName();

        APP::messageCenter()->sendWarning(QString("Download paused: %1").arg(fileName));

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::PAUSED, [](const QJsonObject&){});
    });

    connect(this->m_downloadManager, &DownloadManager::downloadSubprocessErrorOccurred, this, &Backend::subprocessErrorOccurred);
    connect(this->m_downloadManager, &DownloadManager::downloadErrorOccurred, this, &Backend::downloadErrorOccurred);

    connect(this->m_downloadManager, &DownloadManager::downloadFailedAtStart, this, [this](const qint64 internalId){
        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::ERROR);

        this->m_downloadModel->refresh(targetDownload);

        emit failedAtStart(internalId);

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::ERROR, [](const QJsonObject&){});
    });

    connect(this->m_downloadManager, &DownloadManager::downloadAllFinished, [this](){
        APP::messageCenter()->sendDebug("All download finished");
        emit allDownloadFinished();
    });

    // Load Tools Manager
    this->m_toolsManager = new ToolsManager(this);

    connect(this->m_toolsManager, &ToolsManager::fileDownloaded, this, [this](const bool ok, const QString& program){
        if (program.trimmed().compare("yt-dlp", Qt::CaseInsensitive) == 0) { emit ytDlpDownloaded(ok); }
        if (program.trimmed().compare("po-token-provider", Qt::CaseInsensitive) == 0) { this->m_toolsManager->downloadFile(TOOLS::poTokenProviderRsUrl, TOOLS::poTokenProviderRsPath(), "po-token-provider-rs"); }
        if (program.trimmed().compare("po-token-provider-rs", Qt::CaseInsensitive) == 0) {
            this->m_toolsManager->unzipFiles(TOOLS::poTokenProviderRsPath(), TOOLS::poTokenProviderRsUnzipPath());
            emit poTokenProviderDownloaded(ok);
            this->startPoTokenProvider();
        }
    });

    connect(this->m_toolsManager, &ToolsManager::fileDownloadedProgress, this, [this](const qint64 bytesReceived, const qint64 bytesTotal, const QString& program){
        if (program.trimmed().compare("yt-dlp", Qt::CaseInsensitive) == 0) { emit ytDlpDownloadedProgress(bytesReceived, bytesTotal); }
        if (program.trimmed().compare("po-token-provider", Qt::CaseInsensitive) == 0) { emit poTokenProviderDownloadedProgress(bytesReceived, bytesTotal); }
        if (program.trimmed().compare("po-token-provider-rs", Qt::CaseInsensitive) == 0) { emit poTokenProviderDownloadedProgress(bytesReceived, bytesTotal); }
    });

    connect(this->m_toolsManager, &ToolsManager::ytDlpUpdated, this, &Backend::ytDlpUpdated);
    connect(this->m_toolsManager, &ToolsManager::ytDlpUpdatedToNightly, this, &Backend::ytDlpUpdatedToNightly);

    connect(this->m_toolsManager, &ToolsManager::ytDlpChecked, this, [this](const bool, const bool isYtDlp, const QString&){
        const auto needYtDlp = !isYtDlp;

        const auto ts = SharedStorage::instance().getToolsPath();

        connect(this->m_toolsManager, &ToolsManager::ffmpegChecked, this, [needYtDlp, ts, this](const bool, const bool isFFmpeg, const QString&){
            const auto needFFmpeg = !isFFmpeg;

            connect(this->m_toolsManager, &ToolsManager::nodeChecked, this, [needYtDlp, needFFmpeg, ts, this](const bool, const bool isNode, const QString&){
                const auto needNode = !isNode;
                emit toolsChecked(needYtDlp, needFFmpeg, needNode, ts->getYtDlpPath(), ts->getFFmpegPath(), ts->getNodePath());
            });

            this->m_toolsManager->checkNode(ts->getNodePath());
        });

        this->m_toolsManager->checkFFmpeg(ts->getFFmpegPath());
    });

    connect(this->m_toolsManager, &ToolsManager::ffmpegChecked, this, &Backend::ffmpegChecked);
    connect(this->m_toolsManager, &ToolsManager::nodeChecked, this, &Backend::nodeChecked);

    // Load Update Checker
    this->m_updateChecker = new UpdateChecker(this);
    connect(this->m_updateChecker, &UpdateChecker::updateChecked, this, &Backend::updateChecked);

    // Load URL Info Manager
    this->m_urlInfoManager = new URLInfoManager(this);
    connect(this->m_urlInfoManager, &URLInfoManager::urlValid, this, &Backend::urlValid);
    connect(this->m_urlInfoManager, &URLInfoManager::urlCut, this, &Backend::urlCut);
    connect(this->m_urlInfoManager, &URLInfoManager::jsonError, this, &Backend::jsonError);
    connect(this->m_urlInfoManager, &URLInfoManager::jsonReady, this, &Backend::jsonReady);
}

double Backend::doubleRound2(const double value) { return std::round(value * 100) / 100; }


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

            this->m_client->addDownload(*it, [it, this](const QJsonObject& obj){
                APP::messageCenter()->sendDebug(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));

                const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
                const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
                const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

                if (status) {
                    this->m_downloadModel->append(it);
                    this->m_downloadManager->addDownload(it);
                }

                else { APP::messageCenter()->sendError(message); }
            });
        }

        nextInternalId = ++tmp_internalId;

        emit dataImported(true, tr("Data loaded successfully"));
    }

    else { emit dataImported(false, tr("Failed to load data")); }
}

void Backend::exportOutputs(const QUrl& filePath, const QString& text) {
    const auto fp = filePath.toLocalFile().trimmed();

    if (fp.isEmpty()) { emit outputExported(false, tr("Save Path is empty")); return; }

    if (text.trimmed().isEmpty()) { emit outputExported(false, tr("Content is empty")); }

    const auto flag = writeText(fp, text.trimmed());

    emit outputExported(flag, flag ? tr("Output exported successfully") : tr("Output failed exported"));
}


// Load Data

void Backend::setDownloadModel(DownloadModel* dm) { if (dm) { this->m_downloadModel = dm; } }

void Backend::loadDataBase() {
    this->m_client->readAllDownloads([this](const QJsonObject& obj) {
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

        this->m_downloadModel->setDownloads(SharedStorage::instance().getDownloads());
    });
}


// Tools

void Backend::findToolsPath() {
    const auto ffp = ToolsManager::findToolsPath("ffmpeg").trimmed();
    const auto ndp = ToolsManager::findToolsPath("node").trimmed();
    emit toolsPathFound(ffp, ndp);
}

void Backend::loadTools() {
    const auto ydp = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_YT_DLP).trimmed();
    const auto ffp = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_FFMPEG).trimmed();
    const auto ndp = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_NODE).trimmed();

    SharedStorage::instance().setToolsPath({ydp, ffp, ndp});
}

void Backend::checkTools() {
    const auto ydp = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_YT_DLP).trimmed();
    const auto ffp = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_FFMPEG).trimmed();
    const auto ndp = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_NODE).trimmed();

    SharedStorage::instance().setToolsPath({ydp, ffp, ndp});

    this->m_toolsManager->checkYtDlp(ydp);
}

void Backend::checkFFmpeg(const QUrl& ffmpegPath) {
    const auto ffmpegPathTrimmed = ffmpegPath.toLocalFile().trimmed();

    if (ffmpegPathTrimmed.isEmpty()) {
        APP::messageCenter()->sendWarning("Unable to check ffmpeg: path is empty");
        return;
    }

    this->m_toolsManager->checkFFmpeg(ffmpegPathTrimmed);
}

void Backend::checkNode(const QUrl& nodePath) {
    const auto nodePathTrimmed = nodePath.toLocalFile().trimmed();

    if (nodePathTrimmed.isEmpty()) {
        APP::messageCenter()->sendWarning("Unable to check node.js: path is empty");
        return;
    }

    this->m_toolsManager->checkNode(nodePathTrimmed);
}

void Backend::downloadYtDlp() { this->m_toolsManager->downloadFile(TOOLS::ytDlpDownloadUrl(), TOOLS::ytDlpFilePath(), "yt-dlp"); }

void Backend::downloadPoTokenProvider() {this->m_toolsManager->downloadFile(TOOLS::poTokenProviderUrl(), TOOLS::poTokenProviderPath(), "po-token-provider"); }

void Backend::startPoTokenProvider(const QString& poTokenProviderPath) { this->m_providerManager->startProvider(poTokenProviderPath.trimmed()); }

void Backend::updateYtDlp() { this->m_toolsManager->updateYtDlp(); }

void Backend::updateYtDlpToNightly() { this->m_toolsManager->updateYtDlpToNightly(); }

void Backend::saveYtDlpPath(const QString& ytDlpPath) {
    const auto ytDlpPathTrimmed = ytDlpPath.trimmed();

    if (ytDlpPathTrimmed.isEmpty()) { return; }

    SharedStorage::instance().getToolsPath()->setYtDlpPath(ytDlpPathTrimmed);

    CONFIG::writeConfig({ { SYS_CONFIG::KEY_PATH_YT_DLP, ytDlpPathTrimmed } });
}

void Backend::saveFFmpegPath(const QString& ffmpegPath) {
    const auto ffmpegPathTrimmed = ffmpegPath.trimmed();

    if (ffmpegPathTrimmed.isEmpty()) { return; }

    SharedStorage::instance().getToolsPath()->setFFmpegPath(ffmpegPathTrimmed);

    CONFIG::writeConfig({ { SYS_CONFIG::KEY_PATH_FFMPEG, ffmpegPathTrimmed } });
}

void Backend::saveNodePath(const QString& nodePath) {
    const auto nodePathTrimmed = nodePath.trimmed();

    if (nodePathTrimmed.isEmpty()) { return; }

    SharedStorage::instance().getToolsPath()->setNodePath(nodePathTrimmed);

    CONFIG::writeConfig({ { SYS_CONFIG::KEY_PATH_NODE, nodePathTrimmed } });
}


// Add Download

void Backend::showCodecs(const QString& url, const QString& ytDlpPath, const QString& ffmpegPath, const QString& nodePath) {
    const auto toolsPath = SharedStorage::instance().getToolsPath().data();

    const auto ydp = ytDlpPath.trimmed().isEmpty() ? toolsPath->getYtDlpPath() : ytDlpPath;
    const auto ffp = ffmpegPath.trimmed().isEmpty() ? toolsPath->getFFmpegPath() : ffmpegPath;
    const auto ndp = nodePath.trimmed().isEmpty() ? toolsPath->getNodePath() : nodePath;

    auto* process = new QProcess();

    const QStringList arguments{
        "--ffmpeg-location", ffp,
        "--js-runtimes", "node:" + ndp,
        "--no-playlist",
        "--verbose",
        "-F", url.trimmed()
    };

    connect(process, &QProcess::readyReadStandardOutput, this, [process, this](){ emit codecsShown(process->readAllStandardOutput()); });
    connect(process, &QProcess::readyReadStandardError, this, [process, this](){ emit codecsShown(process->readAllStandardError()); });
    connect(process, &QProcess::finished, this, [process](){ process->deleteLater(); });

    process->start(ydp, arguments);
}

void Backend::askAudioFilters() { emit filtersAsked(MediaCodecFilters::audioFilters); }

void Backend::askVideoFilters() { emit filtersAsked(MediaCodecFilters::videoFilters); }

void Backend::checkAvailablePath(const QUrl& path) {
    const auto realPath = path.toLocalFile().trimmed();
    emit availablePathChecked(ToolsManager::checkPathWritable(realPath), realPath);
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
    const DownloadStatus& downloadState,
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

    this->m_client->addDownload(dw, [this, dw](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
        const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

        emit newDownloadAdded(status, message);

        if (status) {
            const auto dynamic_dw = QSharedPointer<Download>::create(dw);
            this->m_downloadModel->append(dynamic_dw);
            this->m_downloadManager->addDownload(dynamic_dw);
        }

        else { APP::messageCenter()->sendError(message); }
    });
}

void Backend::removeDownload(const qint64 internalId) {
    {
        QMutexLocker locker(&this->m_mutex);

        if (this->downloadingIds.contains(internalId)) {
            emit isAlreadyDownloading(internalId, tr("Failed to remove download: task is running"));
            return;
        }
    }

    const auto dw = SharedStorage::instance().getDownload(internalId);

    if (dw.isNull()) { APP::messageCenter()->sendError("Critical Error: internalId -> null"); return; }

    this->m_client->removeDownload(internalId, [this, dw, internalId](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;

        if (status) {
            this->m_downloadModel->removeByInternalId(internalId);
            this->m_downloadManager->removeDownload(internalId);
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

        if (!this->downloadingIds.isEmpty()) {
            emit isAlreadyDownloading(0, tr("Failed to remove all downloads: some task is running"));
            return;
        }
    }

    const auto sz = SharedStorage::instance().getDownloadsSize();

    if (sz <= 0) {
        emit downloadsRemoved(false, tr("No downloads to remove"));
        return;
    }

    this->m_client->removeAllDownloads([this](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;

        if (status) {
            this->m_downloadModel->clear();
            this->m_downloadManager->clearDownloads();
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

        if (this->downloadingIds.contains(internalId)) {
            emit isAlreadyDownloading(internalId, tr("Failed to edit download: task is running"));
            return;
        }
    }

    if (internalId <= 0) {
        APP::messageCenter()->sendError(QString("Invalid internalId: %1").arg(internalId));
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
    const DownloadStatus& downloadState,
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

    this->m_client->removeDownload(internalId, [this, dw, internalId](const QJsonObject& obj){
        const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
        const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
        const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

        if (status) {
            this->m_client->addDownload(dw, [this, dw, internalId](const QJsonObject& obj){
                const auto status_s = obj[JSON_FORMAT::STATUS].toString().trimmed();
                const auto status = status_s.compare(JSON_FORMAT::STATUS_ACCEPTED, Qt::CaseInsensitive) == 0;
                const auto message = obj[JSON_FORMAT::MESSAGE].toString().trimmed();

                emit newDownloadAdded(status, message);

                if (status) {
                    const auto dt = SharedStorage::instance().getDownload(internalId);

                    *dt = dw;

                    this->m_downloadModel->setDownloads(SharedStorage::instance().getDownloads());
                }

                else { APP::messageCenter()->sendError(message); }
            });
        }

        else { APP::messageCenter()->sendError(message); }
    });
}


// Start Download

void Backend::startDownload(const qint64 internalId) {
    if (internalId <= 0) { return; }

    this->consume(internalId, 2, 6);

    QMutexLocker locker(&this->m_mutex);

    const auto size = this->downloadingIds.size();
    const auto ifContains = this->downloadingIds.contains(internalId);

    locker.unlock();

    const auto targetDownload = SharedStorage::instance().getDownload(internalId);

    if (size > 1) {
        emit isAlreadyDownloading(0, tr("Other files are downloading. Please wait for finished"));
        return;
    }

    if (size == 1 && ifContains) {
        const auto message = tr("Target file ") +  QFileInfo(targetDownload->getSavePath()).fileName() + tr(" is already downloading");

        emit isAlreadyDownloading(internalId, message);

        return;
    }

    if (size == 1 && !ifContains) {
        if (this->m_downloadWorker) {
            const auto process = this->m_downloadWorker->getProcess();

            if (process && process->state() != QProcess::NotRunning) {
                const auto currentId = this->m_downloadWorker->getInternalId();
                const auto currentDownload =SharedStorage::instance().getDownload(currentId);
                const auto message = tr("Another file is downloading: ") + QFileInfo(currentDownload->getSavePath()).fileName() + tr(" Please wait for finished");

                emit isAlreadyDownloading(currentId, message);
                return;
            }
        }

        emit isAlreadyDownloading(0, tr("Other files are downloading. Please wait for finished"));

        return;
    }

    {
        QMutexLocker locker(&this->m_mutex);
        this->downloadingIds.insert(internalId);
    }

    auto* process = new QProcess(this);
    auto* worker = new DownloadWorker(targetDownload, process, this);
    this->m_downloadWorker = worker;

    connect(worker, &DownloadWorker::modelChanged, this, [this](const QSharedPointer<Download>& dt){
        this->m_downloadModel->refresh(dt);

        const auto _internalId = dt->getInternalId();

        this->m_client->updateDownloadStatus(_internalId, dt->getDownloadStatus(), [](const QJsonObject&) {});

        this->m_client->updateProgress(_internalId, dt->getProgress(), [](const QJsonObject&) {});

        this->m_client->updateDownloadedBytes(_internalId, dt->getDownloadedBytes(), [](const QJsonObject&) {});

        this->m_client->updateTotalBytes(_internalId, dt->getTotalBytes(), [](const QJsonObject&j) {});

        this->m_client->updateDownloadSpeed(_internalId, dt->getDownloadSpeed(), [](const QJsonObject&) {});

        this->m_client->updateEta(_internalId, dt->getEta().trimmed(), [](const QJsonObject&) {});
    });

    connect(worker, &DownloadWorker::normalFinished, this, [worker, this](const qint64 internalId){
        {
            QMutexLocker locker(&this->m_mutex);
            this->downloadingIds.clear();
        }

        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::FINISHED);
        targetDownload->setEta("00:00");

        this->m_downloadModel->refresh(targetDownload);

        const auto savePath = targetDownload->getSavePath().trimmed();
        const auto fileName = QFileInfo(savePath).fileName();

        APP::messageCenter()->sendDebug(QString("Download normally finished: %1").arg(fileName));

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::FINISHED, [](const QJsonObject&){});

        this->m_client->updateEta(internalId, "00:00", [](const QJsonObject&){});

        emit downloadFinished(internalId);

        if (this->m_downloadWorker == worker) {
            this->m_downloadWorker = nullptr;
        }

        worker->deleteLater();
    });

    connect(worker, &DownloadWorker::downloadPaused, this, [worker, this](const qint64 internalId) {
        {
            QMutexLocker locker(&this->m_mutex);
            this->downloadingIds.clear();
        }

        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::PAUSED);

        this->m_downloadModel->refresh(targetDownload);

        const auto savePath = targetDownload->getSavePath().trimmed();
        const auto fileName = QFileInfo(savePath).fileName();

        APP::messageCenter()->sendWarning(QString("Download paused: %1").arg(fileName));

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::PAUSED, [](const QJsonObject&){});

        emit downloadFinished(internalId);

        if (this->m_downloadWorker == worker) {
            this->m_downloadWorker = nullptr;
        }

        worker->deleteLater();
    });

    connect(worker, &DownloadWorker::unnormalFinished, this, [worker, this](const qint64 internalId) {
        {
            QMutexLocker locker(&this->m_mutex);
            this->downloadingIds.clear();
        }

        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::ERROR);

        this->m_downloadModel->refresh(targetDownload);

        const auto savePath = targetDownload->getSavePath().trimmed();
        const auto fileName = QFileInfo(savePath).fileName();

        APP::messageCenter()->sendWarning(QString("Download unnormally finished: %1").arg(fileName));

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::ERROR, [](const QJsonObject&){});

        emit downloadFinished(internalId);

        if (this->m_downloadWorker == worker) {
            this->m_downloadWorker = nullptr;
        }

        worker->deleteLater();
    });

    connect(worker, &DownloadWorker::subprocessErrorOccurred, this, &Backend::subprocessErrorOccurred);

    connect(worker, &DownloadWorker::errorOccurred, this, &Backend::downloadErrorOccurred);

    connect(worker, &DownloadWorker::failedAtStart, this, [worker, this](const qint64 internalId){
        {
            QMutexLocker locker(&this->m_mutex);
            this->downloadingIds.clear();
        }

        const auto targetDownload = SharedStorage::instance().getDownload(internalId);

        targetDownload->setDownloadStatus(DownloadStatus::ERROR);

        this->m_downloadModel->refresh(targetDownload);

        this->m_client->updateDownloadStatus(internalId, DownloadStatus::ERROR, [](const QJsonObject&){});

        emit failedAtStart(internalId);

        if (this->m_downloadWorker == worker) {
            this->m_downloadWorker = nullptr;
        }

        worker->deleteLater();
    });

    this->m_downloadWorker->start();
}

void Backend::startDownload() {
    if (!this->m_downloadManager) { return; }

    const auto downloads = SharedStorage::instance().getDownloads();

    this->m_downloadManager->setDownloads(*downloads);

    this->m_downloadManager->startDownloads();
}


// Stop Download

void Backend::stopDownload(const qint64 internalId) {
    if (internalId <= 0) { return; }

    QMutexLocker locker(&this->m_mutex);

    const auto size = this->downloadingIds.size();
    const auto ifContains = this->downloadingIds.contains(internalId);

    locker.unlock();

    if (size > 1 || size < 1 || !ifContains) {
        emit downloadStopped(internalId, false, tr("Target file is not downloading"));
        return;
    }

    if (!this->m_downloadWorker) {
        emit downloadStopped(internalId, false, tr("Target file is not downloading"));
        return;
    }

    const auto process = this->m_downloadWorker->getProcess();

    if (!process ||
        process->state() == QProcess::NotRunning) {

        emit downloadStopped(internalId, false, tr("Target file is not downloading"));

        return;
    }

    const auto _internalId = this->m_downloadWorker->getInternalId();
    const auto _download = SharedStorage::instance().getDownload(_internalId);
    const auto _message = tr("Stopping: ") + QFileInfo(_download->getSavePath()).fileName() + " ...";

    this->m_downloadWorker->stop();

    {
        QMutexLocker locker(&this->m_mutex);
        this->downloadingIds.clear();
    }

    emit downloadStopped(internalId, true, _message);

    return;
}

void Backend::stopDownload() {
    if (this->m_downloadWorker) { this->stopDownload(this->m_downloadWorker->getInternalId()); }

    this->m_downloadManager->stopDownloads();

    APP::messageCenter()->sendDebug("All downloads stopped");

    emit allDownloadStopped();
}


// URL Manager

void Backend::isValidUrl(const QString& url) { this->m_urlInfoManager->isValidUrl(url.trimmed()); }

void Backend::getUrlInfo(const QString& url) { this->m_urlInfoManager->getInfo(url.trimmed()); }


// Update Checker

void Backend::visitRepo()    { UpdateChecker::visitRepo();    }

void Backend::helpDoc()      { UpdateChecker::helpDoc();      }

void Backend::reportIssues() { UpdateChecker::reportIssues(); }

void Backend::checkUpdate() const {
    if (!this->m_updateChecker) {
        APP::messageCenter()->sendError("UpdateChecker is null");
        return;
    }

    this->m_updateChecker->checkUpdate();
}


// Youtube Rate Limiter

void Backend::consume(const qint64 internalId, const int type, const int weight, const qint64 createdAt) {
    this->canPerform(weight, [internalId, createdAt, type, weight, this](const bool ok){
        if (!ok) { return; }

        this->m_client->addYoutubeAction({internalId, parseYoutubeActionsType(type), weight, createdAt}, [](const QJsonObject&){});
    });
}

void Backend::cleanUp() {
    constexpr qint64 retentionTime = 48 * 60 * 60;

    const qint64 threshold = QDateTime::currentSecsSinceEpoch() - retentionTime;

    this->m_client->removeUselessActions(threshold, [](const QJsonObject&){});
}


// Clear Status

void Backend::clearAllDownloadStatus() {
    const auto ds = SharedStorage::instance().getDownloads();

    if (ds.isNull() || ds->isEmpty()) { return; }

    for (const auto& it : *ds) {
        it->setProgress(0.0);
        it->setDownloadStatus(DownloadStatus::WAITING);
        it->setDownloadedBytes(0.0);
        it->setTotalBytes(0.0);
        it->setDownloadSpeed(0.0);
        it->setEta("00:00");

        this->m_downloadModel->refresh(it);

        const auto _internalId = it->getInternalId();

        this->m_client->updateProgress(_internalId, 0.0, [](const QJsonObject&){});

        this->m_client->updateDownloadStatus(_internalId, DownloadStatus::WAITING, [](const QJsonObject&){});

        this->m_client->updateDownloadedBytes(_internalId, 0.0, [](const QJsonObject&){});

        this->m_client->updateTotalBytes(_internalId, 0.0, [](const QJsonObject&){});

        this->m_client->updateDownloadSpeed(_internalId, 0.0, [](const QJsonObject&){});

        this->m_client->updateEta(_internalId, "00:00", [](const QJsonObject&){});
    }
}


// Settings

void Backend::loadSettings() {
    QJsonObject obj;

    const auto lightModeAuto = CONFIG::readConfig<bool>(SYS_CONFIG::GENERAL_SETTINGS::KEY_LIGHTMODE_AUTOMATIC);
    const auto lightMode = CONFIG::readConfig<bool>(SYS_CONFIG::GENERAL_SETTINGS::KEY_LIGHTMODE);
    const auto selfCheck = CONFIG::readConfig<bool>(SYS_CONFIG::GENERAL_SETTINGS::KEY_SELF_CHECK_WHEN_EXECUTE);
    const auto rateLimit = CONFIG::readConfig<bool>(SYS_CONFIG::GENERAL_SETTINGS::KEY_USE_RATELIMIT);
    const auto messageBeforeSaving = CONFIG::readConfig<bool>(SYS_CONFIG::DOWNLOAD_SETTINGS::KEY_MESSAGE_BEFORE_SAVING);
    const auto ffmpegPath = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_FFMPEG).trimmed();
    const auto nodePath = CONFIG::readConfig<QString>(SYS_CONFIG::KEY_PATH_NODE).trimmed();

    obj.insert("lightModeAutomatic", lightModeAuto);
    obj.insert("lightMode", lightMode);
    obj.insert("selfCheck", selfCheck);
    obj.insert("rateLimit", rateLimit);
    obj.insert("messageBeforeSaving", messageBeforeSaving);
    obj.insert("ffmpegPath", ffmpegPath);
    obj.insert("nodePath", nodePath);

    emit settingsLoaded(obj);
}

void Backend::writeDefaultSettings() {
    const auto flag = CONFIG::writeConfig(
        {
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_LIGHTMODE_AUTOMATIC, true },
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_LIGHTMODE, true },
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_SELF_CHECK_WHEN_EXECUTE, true },
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_USE_RATELIMIT, true },
            { SYS_CONFIG::DOWNLOAD_SETTINGS::KEY_MESSAGE_BEFORE_SAVING, true }
        }
    );
}

void Backend::updateLightMode(const bool automatic, const bool light) {
    const auto flag = CONFIG::writeConfig(
        {
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_LIGHTMODE_AUTOMATIC, automatic },
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_LIGHTMODE, automatic ? true : light }
        }
    );

    emit lightModeUpdated(flag);
}

void Backend::updateSelfCheck(const bool ok) {
    const auto flag = CONFIG::writeConfig(
        {
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_SELF_CHECK_WHEN_EXECUTE, ok },
        }
    );

    emit selfCheckUpdated(flag);
}

void Backend::updateRateLimit(const bool ok) {
    const auto flag = CONFIG::writeConfig(
        {
            { SYS_CONFIG::GENERAL_SETTINGS::KEY_USE_RATELIMIT, ok },
        }
    );

    emit rateLimitUpdated(flag);
}

void Backend::updateMessageBeforeSaving(const bool ok) {
    const auto flag = CONFIG::writeConfig(
        {
            { SYS_CONFIG::DOWNLOAD_SETTINGS::KEY_MESSAGE_BEFORE_SAVING, ok }
        }
    );

    emit messageBeforeSavingUpdated(flag);
}

void Backend::updateFFmpegPath(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    if (filePathTrimmed.isEmpty()) { return; }

    const auto flag = CONFIG::writeConfig(
        {
            { SYS_CONFIG::KEY_PATH_FFMPEG, filePathTrimmed }
        }
    );

    if (flag) { SharedStorage::instance().getToolsPath()->setFFmpegPath(filePathTrimmed); }
}

void Backend::updateNodePath(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    if (filePathTrimmed.isEmpty()) { return; }

    const auto flag = CONFIG::writeConfig(
        {
            { SYS_CONFIG::KEY_PATH_NODE, filePathTrimmed }
        }
    );

    if (flag) { SharedStorage::instance().getToolsPath()->setNodePath(filePathTrimmed); }
}


// Quit App

void Backend::requestQuitApp() { this->m_downloadManager->stopDownloads(); }


// Private

void Backend::usageSince(const qint64 timestamp, std::function<void(int)> cb) {
    if (timestamp <= 0) { return; }

    this->m_client->getWeights(timestamp, [timestamp, cb](const QJsonObject& obj){
        const auto weights = obj[JSON_FORMAT::WEIGHTS].toInt();
        cb(weights);
    });
}

void Backend::usageLastHour(std::function<void(int)> cb) {
    const auto now = QDateTime::currentSecsSinceEpoch();
    constexpr qint64 oneHour = 60 * 60;
    this->usageSince(now - oneHour, cb);
}

void Backend::usageLastDay(std::function<void(int)> cb) {
    const auto now = QDateTime::currentSecsSinceEpoch();
    constexpr qint64 oneDay = 24 * 60 * 60;
    this->usageSince(now - oneDay, cb);
}

void Backend::canPerform(const int weight, std::function<void(bool)> cb) {
    if (weight != 1 && weight != 6 && weight != 0) {
        APP::messageCenter()->sendWarning("Error weight: must be 1 or 6");
        return ;
    }

    this->cleanUp();

    this->usageLastHour([weight, cb, this](const int weightsHour){
        APP::messageCenter()->sendDebug(QString("WeightHour: %1; maxPerHour: %2").arg(weightsHour).arg(Backend::maxPerHour));

        if (weightsHour + weight > Backend::maxPerHour) {
            emit possiblePerform(false);
            cb(false);
            return;
        }

        this->usageLastDay([weight, cb, this](const int weightsDay){
            APP::messageCenter()->sendDebug(QString("WeightDay: %1; maxPerDay: %2").arg(weightsDay).arg(Backend::maxPerDay));

            if (weightsDay + weight > Backend::maxPerDay) {
                emit possiblePerform(false);
                cb(false);
                return;
            }

            emit possiblePerform(true);
            cb(true);
        });
    });
}
