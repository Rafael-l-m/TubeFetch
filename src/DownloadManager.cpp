
#include "DownloadManager.h"
#include <QDir>
#include <QTimer>
#include <QRandomGenerator>


// Public

DownloadManager::DownloadManager(QString ytDlpPath, QString ffmpegPath, QString nodePath, const QString& databasePath, const int max_concurrent_downloads, const int retries, const int fragment_retries, const int concurrent_fragments) : yt_dlp_path(std::move(ytDlpPath)), ffmpeg_path(std::move(ffmpegPath)), node_js_path(std::move(nodePath)), maxConcurrentDownloads(max_concurrent_downloads), retries(retries), fragment_retries(fragment_retries), concurrent_fragments(concurrent_fragments) {
    this->activeCount = 0;
    this->taskQueue = new QQueue<int>();
    this->downloads = new QMap<int, DownloadItemWidget*>();
    this->database = new DataBaseManager(databasePath);
    this->activeProcesses = new QMap<int, QProcess*>();

    const auto* tmp = new DownloadInfoWidget{"", "", ""};

    this->supportedAudioTypes = tmp->getSupportedAudioTypes();
    this->supportedVideoTypes = tmp->getSupportedVideoTypes();

    delete tmp;
}

bool DownloadManager::addDownload(const int id, const DownloadItem *info, DownloadItemWidget* wg) const {
    this->taskQueue->enqueue(id);
    this->downloads->insert(id, wg);
    return this->database->addDownload(id, info->url, info->audio_code, info->video_code, info->file_path, info->suffix, info->oldTitle, info->newTitle, 0, false, info->withMetadata);
}

bool DownloadManager::removeDownload(const int id) const {
    auto& queue = *this->taskQueue;

    for (int i = 0; i < queue.size(); ++i) {
        if (queue[i] == id) {
            queue.removeAt(i);
            break;
        }
    }

    this->downloads->remove(id);
    return this->database->removeDownloaded(id);
}

bool DownloadManager::removeAllDownloads() const {
    this->taskQueue->clear();
    this->downloads->clear();
    return this->database->removeAllDownloads();
}

QPair<int, QList<DownloadItemWidget *>> DownloadManager::restoreData() const {
    QList<DownloadItemWidget*> list;
    int tmp_id = 0;

    if (this->database->isOpen()) {
        const auto* items = this->database->readAllDownloads();

        for (const auto* d : *items) {
            const auto ID = d->id;

            auto* dwWidget = new DownloadItemWidget {ID, d->oldTitle, d->newTitle};

            this->taskQueue->enqueue(ID);
            this->downloads->insert(ID, dwWidget);

            if (d->isDownload) dwWidget->setProgress(1000);

            else dwWidget->setProgress(d->progress);

            list.append(dwWidget);

            tmp_id = qMax(tmp_id, ID);
        }

        for (const auto* d : *items) {delete d;}

        delete items;

        ++tmp_id;
    }

    return {tmp_id, list};
}

bool DownloadManager::haveAvailableDownload() const {return !this->taskQueue->isEmpty();}

bool DownloadManager::haveAvailableWidget() const {return !this->downloads->isEmpty();}

bool DownloadManager::haveActiveProcesses() const {return !this->activeProcesses->isEmpty();}

bool DownloadManager::setConcurrentDownload(const int cd) {
    if (cd <= MAX_CONCURRENT_DOWNLOADS && cd >= MIN_CONCURRENT_DOWNLOADS) {
        this->maxConcurrentDownloads = cd;
        return true;
    }

    return false;
}

void DownloadManager::startNextDownload() {while (this->activeCount < this->maxConcurrentDownloads && !this->taskQueue->isEmpty()) {this->download(this->taskQueue->dequeue());}}

void DownloadManager::stopAll() {
    for (auto* process : *this->activeProcesses) {
        #ifdef Q_OS_WIN
                const QString cmd = QString("taskkill /PID %1 /T /F").arg(process->processId());
                QProcess::execute(cmd);
        #elif defined(Q_OS_MAC) || defined(Q_OS_LINUX)
                QProcess::execute("pkill -f yt-dlp");
                process->kill();
        #endif
    }

    for (auto it = this->downloads->begin(); it != this->downloads->end(); ++it) {
        const auto idd = it.key();
        const auto* dww = it.value();
        const auto progress = dww->getProgress();

        (void)this->database->updateProgress(idd, progress);

        if (progress == 1000) (void)this->database->updateDownloaded(idd, true);

        else (void)this->database->updateDownloaded(idd, false);
    }

    this->activeProcesses->clear();
    this->taskQueue->clear();
    this->activeCount = 0;
}

QString DownloadManager::upgradeYtDlp(const QString& ytDlpPath) {
    QProcess process;
    process.start(ytDlpPath, {"-U"});
    process.waitForFinished(-1);
    return QString::fromUtf8(process.readAllStandardOutput());
}


// Database

bool DownloadManager::updateNewTitle(const int id, const QString& newTitle) const {return this->database->updateNewTitle(id, newTitle);}

bool DownloadManager::updateAudioCode(const int id, const int audio_code) const {return this->database->updateAudioCode(id, audio_code);}

bool DownloadManager::updateVideoCode(const int id, const int video_code) const {return this->database->updateVideoCode(id, video_code);}

bool DownloadManager::updateIfMetadata(const int id, const bool withMetadata) const {return this->database->updateIfMetadata(id, withMetadata);}

bool DownloadManager::updateFilePath(const int id, const QString& filePath) const {return this->database->updateFilePath(id, filePath);}

bool DownloadManager::updateSuffix(const int id, const QString& suffix) const {return this->database->updateSuffix(id, suffix);}

DownloadItem* DownloadManager::readDownload(const int id) const {return this->database->readDownload(id);}


// Private

void DownloadManager::download(const int id) {
    if (this->downloads->contains(id)) {
        const auto* info = this->readDownload(id);

        if (const auto* dw = this->downloads->value(id, nullptr); info && dw) {
            ++this->activeCount;
            dw->disabledW();

            const auto randomDelayMs = QRandomGenerator::global()->bounded(MIN_WAIT_TIME, MAX_WAIT_TIME + 1);
            const auto randomKString = QRandomGenerator::global()->bounded(MIN_RATE, MAX_RATE);
            const auto ua = this->userAgents.at(QRandomGenerator::global()->bounded(this->userAgents.size()));

            QTimer::singleShot(randomDelayMs, [this, dw, randomKString, info, ua]() {
                auto* process = new QProcess;

                this->activeProcesses->insert(info->id, process);

                QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                env.insert("PATH", this->node_js_path + QDir::listSeparator() + env.value("PATH"));
                process->setProcessEnvironment(env);

                QStringList arguments;
                arguments << "--ffmpeg-location" << this->ffmpeg_path
                          << "--no-playlist"
                          << "--limit-rate" << QString::number(randomKString) + "K"
                          << "--user-agent" << ua
                          << "--output" << info->file_path
                          << "--retries" << QString::number(this->retries)
                          << "--fragment-retries" << QString::number(this->fragment_retries)
                          << "--concurrent-fragments" << QString::number(this->concurrent_fragments);

                if (info->withMetadata) arguments.append("--embed-metadata");

                if (info->audio_code != 0 && info->video_code != 0) {
                    arguments.append("-f");
                    arguments.append(QString("%1+%2").arg(info->video_code).arg(info->audio_code));
                    arguments.append("--merge-output-format");
                    arguments.append(info->suffix);
                }

                else if (info->video_code == 0 && info->audio_code != 0) {
                    arguments.append("-x");
                    arguments.append("--audio-format");
                    arguments.append(info->suffix);
                    arguments.append("-f");
                    arguments.append(QString("%1").arg(info->audio_code));
                    arguments.append("--audio-quality");
                    arguments.append("0");
                }

                else {
                    if (this->supportedAudioTypes.contains(info->suffix)) {
                        arguments.append("-f");
                        arguments.append("bestaudio");
                        arguments.append("--audio-format");
                        arguments.append(info->suffix);
                        arguments.append("--audio-quality");
                        arguments.append("0");
                    }

                    else if (this->supportedVideoTypes.contains(info->suffix)) {
                        arguments.append("-f");
                        arguments.append("bv+ba");
                        arguments.append("--merge-output-format");
                        arguments.append(info->suffix);
                    }
                }

                arguments.append(info->url);

                connect(process, &QProcess::readyReadStandardOutput, [=]() {
                    #ifdef Q_OS_WIN
                        const auto text = QString::fromLocal8Bit(process->readAllStandardOutput());
                    #else
                        const auto text = QString::fromUtf8(process->readAllStandardOutput());
                    #endif

                    static const QRegularExpression re(R"(\[download\]\s+(\d+(\.\d+)?)%)");

                    if (const auto match = re.match(text); match.hasMatch()) {
                        const auto pg = match.captured(1).toDouble();
                        const auto progress = qRound(pg*10);
                        dw->setProgress(progress);
                    }
                });

                connect(process, &QProcess::readyReadStandardError, [=, this]() {
                    const auto err = process->readAllStandardError();
                    const auto content = QString::fromLocal8Bit(err);

                    if (content.contains("warning", Qt::CaseInsensitive)) return;

                    if (err.contains("429") || err.contains("Too Many Requests") || err.contains("403") || err.contains("Forbidden") || err.contains("rate limit") || err.contains("limit exceeded")) this->maxConcurrentDownloads = this->maxConcurrentDownloads <= 1 ? this->maxConcurrentDownloads : this->maxConcurrentDownloads - 1;

                    NotificationWindow::warning(tr("Download Failed"), content);
                });

                connect(process, &QProcess::finished, this, [=, this](const int exitCode, const QProcess::ExitStatus existStatus) mutable {
                    this->activeProcesses->remove(info->id);
                    process->deleteLater();

                    if (exitCode == 0 && existStatus == QProcess::NormalExit) {
                        (void)this->database->updateProgress(info->id, 1000);
                        (void)this->database->updateDownloaded(info->id, true);
                    }

                    --this->activeCount;
                    dw->enabledW();
                    this->startNextDownload();

                    if (this->activeCount == 0 && this->taskQueue->isEmpty()) {
                        if (existStatus == QProcess::NormalExit) {
                            NotificationWindow::information(tr("Download Finished"), tr("All downloads have completed"));

                            const auto data = this->restoreData();
                            emit renewWidgets(data);
                            emit finishedDownload();
                        }
                    }
                });

                connect(process, &QProcess::errorOccurred, process, [process, info, this]() {
                    this->activeProcesses->remove(info->id);
                    process->deleteLater();
                });

                process->start(this->yt_dlp_path, arguments);
            });
        }

        else this->startNextDownload();
    }

    else this->startNextDownload();
}
