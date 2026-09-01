#include <core/APP.hpp>
#include <core/Constants.hpp>
#include <download/DownloadWorker.hpp>
#include <SharedStorage.hpp>
#include <url/URLInfoManager.hpp>
#include <QRandomGenerator>
#include <QTimer>


// Public

DownloadWorker::DownloadWorker(
    const QSharedPointer<Download>& dt,
    QProcess* process,
    QObject* parent) : QObject(parent), m_task(dt), m_process(process) {}

QProcess* DownloadWorker::getProcess() const { return this->m_process; }

void DownloadWorker::setProcess(QProcess* newProcess) {
    if (!newProcess) { return; }

    if (this->m_process->state() != QProcess::NotRunning) {
        APP::messageCenter()->sendWarning("Failed to set new process, the old one is running");
        return;
    }

    this->m_process->disconnect();
    this->m_process->deleteLater();

    this->m_process = newProcess;
}

qint64 DownloadWorker::getInternalId() const { return this->m_task ? this->m_task->getInternalId() : 0; }


// Public Slots

void DownloadWorker::start() {
    const auto randomDelayMs =
        QRandomGenerator::global()->bounded(
            DOWNLOAD::TIME_WAIT_MIN,
            DOWNLOAD::TIME_WAIT_MAX
        );

    const auto randomKString =
        QRandomGenerator::global()->bounded(
            DOWNLOAD::RATE_MIN,
            DOWNLOAD::RATE_MAX
        );

    APP::messageCenter()->sendDebug(QString("DownloadWorker start with internalId: %1").arg(this->getInternalId()));
    APP::messageCenter()->sendDebug(QString("Delay: %1; Rate: %2").arg(randomDelayMs).arg(randomKString));

    QTimer::singleShot(randomDelayMs, this, [this, randomKString](){
        if (!this->m_process || !this->m_task) {
            emit unnormalFinished(this->getInternalId());
            APP::messageCenter()->sendError("Process is null || DownloadTask is null || DownloadTask->Download is null when downloading");
            return;
        }

        const auto tp = SharedStorage::instance().getToolsPath();

        const auto ytDlpPath = tp->getYtDlpPath();
        const auto ffmpegPath = tp->getFFmpegPath();
        const auto nodePath = tp->getNodePath();

        const auto _internalId = this->m_task->getInternalId();
        const auto __url = this->m_task->getUrl().trimmed();
        const auto __ac = this->m_task->getAudioCode().trimmed();
        const auto __vc = this->m_task->getVideoCode().trimmed();
        const auto _ndc = this->m_task->getNonDashCode().trimmed();
        const auto _ba = this->m_task->getBestAudio();
        const auto _bv = this->m_task->getBestVideo();
        const auto _savePath = this->m_task->getSavePath().trimmed();
        const auto _suf = this->m_task->getSuffix().trimmed();
        const auto _met = this->m_task->getMetadata();

        QStringList arguments;
        arguments << "--ffmpeg-location" << ffmpegPath
                  << "--js-runtimes" << "node:" + nodePath
                  << "--no-playlist"
                  << "--limit-rate" << QString::number(randomKString) + "K"
                  << "--output" << _savePath
                  << "--verbose";

        if (_met) { arguments.append("--embed-metadata"); }

        static QRegularExpression re(R"((\d+)\s*\(([^)]+)\))");

        const auto audioMatch = re.match(__ac);
        const auto videoMatch = re.match(__vc);

        const auto _ac = audioMatch.captured(1).trimmed();
        const auto _acodec = audioMatch.captured(2).trimmed().toLower();

        const auto _vc = videoMatch.captured(1).trimmed();
        const auto _vcodec = videoMatch.captured(2).trimmed().toLower();

        APP::messageCenter()->sendDebug(
            QString("InternalId: %1; suf: %2; ac: %3, acodec: %4; vc: %5, vcodec: %6")
                .arg(_internalId)
                .arg(_suf)
                .arg(_ac).arg(_acodec)
                .arg(_vc).arg(_vcodec)
        );

        if (!__ac.isEmpty() && !__vc.isEmpty()) {
            arguments.append("-f");
            arguments.append(_vc + "+" + _ac);

            const auto flag = MediaCodecFilters::canMux(_ac, _vc, _suf);

            APP::messageCenter()->sendDebug(QString("CanMux: %1").arg(flag ? "true" : "false"));

            if (flag) { arguments.append("--merge-output-format"); }
            else { arguments.append("--recode-video"); }

            arguments.append(_suf);
        }

        else if (!__ac.isEmpty()) {
            arguments.append("-x");
            arguments.append("--audio-format");
            arguments.append(_suf);
            arguments.append("-f");
            arguments.append(_ac);
            arguments.append("--audio-quality");
            arguments.append("0");
        }

        else if (!_ndc.isEmpty()) {
            arguments.append("-f");
            arguments.append(_ndc);
            arguments.append("--remux-video");
            arguments.append(_suf);
        }

        else {
            if (_ba) {
                arguments.append("-f");
                arguments.append("bestaudio");
                arguments.append("--audio-format");
                arguments.append(_suf);
                arguments.append("--audio-quality");
                arguments.append("0");

            }

            else if (_bv) {
                arguments.append("-f");
                arguments.append("bv+ba");
                arguments.append("--merge-output-format");
                arguments.append(_suf);
            }
        }

        const auto _url = URLInfoManager::cutUrl(__url);

        arguments.append(_url);

        APP::messageCenter()->sendDebug(QString("Starting yt-dlp: %1; %2").arg(ytDlpPath).arg(arguments.join(", ")));

        connect(this->m_process, &QProcess::readyReadStandardOutput, this, [this](){
            const auto text = QString::fromUtf8(this->m_process->readAllStandardOutput());

            static QRegularExpression re(
                R"(\[download\]\s+([\d.]+)%\s+of\s+(?:~\s*)?([\d.]+)([KMG]i?B)\s+at\s+([\d.]+)([KMG]i?B/s)\s+ETA\s+([\d:]+))"
            );


            APP::messageCenter()->sendOutput(QString("Standard output: %1").arg(text));

            const auto match = re.match(text);

            if (match.hasMatch()) {
                const auto progress = match.captured(1);
                const auto size = match.captured(2);
                const auto speed = match.captured(4);
                const auto eta = match.captured(6);

                auto flag = false;

                const auto prog = progress.toDouble(&flag);
                const auto _prog = flag ? prog : -1;

                const auto sz = size.toDouble(&flag);
                const auto _size = flag ? sz : -1.0;

                const auto spd = speed.toDouble(&flag);
                const auto _speed = flag ? spd : -1.0;

                flag = false;

                if (_prog > 0) {
                    flag = true;
                    this->m_task->setProgress(_prog);
                }

                if (_prog > 0 && _size > 0) {
                    flag = true;
                    this->m_task->setDownloadedBytes(prog * _size / 100);
                    this->m_task->setTotalBytes(_size);
                }

                if (_speed > 0) { flag = true; this->m_task->setDownloadSpeed(_speed); }

                const auto _eta_trimmed = eta.trimmed();

                if (!_eta_trimmed.isEmpty() && _eta_trimmed.compare("00:00") != 0) {
                    flag = true;
                    this->m_task->setEta(_eta_trimmed);
                }

                if (flag) { emit modelChanged(this->m_task); }
            }
        });

        connect(this->m_process, &QProcess::readyReadStandardError, this, [this](){
            const auto error = this->m_process->readAllStandardError();

            if (!error.trimmed().isEmpty()) {
                if (error.contains("429") ||
                    error.contains("403") ||
                    error.contains("Too Many Requests") ||
                    error.contains("Forbidden") ||
                    error.contains("Rate limit") ||
                    error.contains("Limit exceeded"))
                {
                    APP::messageCenter()->sendError(QString("Standard error: %1").arg(error));

                    emit tooManyRequest();
                    emit subprocessErrorOccurred(this->getInternalId(), error);

                    this->stop();

                    return;
                }
            }

            APP::messageCenter()->sendOutput(QString("Standard debug output: %1").arg(error));
        });

        connect(this->m_process, &QProcess::errorOccurred, this, [this](const QProcess::ProcessError& processError) {
            if (processError == QProcess::FailedToStart) {
                APP::messageCenter()->sendError("Failed to start process");
                emit failedAtStart(this->getInternalId());
                this->stop();
                return;
            }

            const auto error = this->m_process->errorString();

            APP::messageCenter()->sendWarning(QString("Error occurred: %1").arg(error));

            emit errorOccurred(this->getInternalId(), this->m_process->errorString());

            this->stop();
        });

        connect(this->m_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this](const int exitCode, const QProcess::ExitStatus& exitStatus){
            APP::messageCenter()->sendDebug(
                QString("Process finished: (internalId) %1; (exitCode) %2; (exitStatus) %3")
                    .arg(this->getInternalId())
                    .arg(exitCode)
                    .arg(exitStatus)
            );

            if (exitCode == 0 && exitStatus == QProcess::NormalExit) { emit normalFinished(this->getInternalId()); }

            else if (exitCode == 15 && exitStatus == QProcess::CrashExit) { emit downloadPaused(this->getInternalId()); }

            else { emit unnormalFinished(this->getInternalId()); }
        });

        this->m_process->start(ytDlpPath, arguments);
    });
}

void DownloadWorker::stop() {
    if (!this->m_process) { return; }

    if (this->m_process->state() == QProcess::NotRunning) { return; }

    this->m_process->terminate();

    if (!this->m_process->waitForFinished()) {
        this->m_process->kill();
        this->m_process->waitForFinished();
    }

    this->m_process->readAllStandardOutput();
    this->m_process->readAllStandardError();
    this->m_process->disconnect();
}
