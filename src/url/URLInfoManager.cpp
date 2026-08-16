#include "core/APP.hpp"
#include "core/Constants.hpp"
#include "SharedStorage.hpp"
#include "url/URLInfoManager.hpp"


// Public

URLInfoManager::URLInfoManager(QObject* parent) : QObject(parent) {}

QString URLInfoManager::cutUrl(const QString& url) {
    const auto url_trimmed = url.trimmed();

    static const QRegularExpression re(
        R"regex(https?://(www\.)?(youtube\.com/watch\?v=|youtu\.be/))regex",
        QRegularExpression::CaseInsensitiveOption
    );

    if (re.match(url_trimmed).hasMatch()) {
        if (const auto pos = url.indexOf('&'); pos != -1) {
            const auto urlAfterCut = url.left(pos);
            return urlAfterCut;
        }
    }

    return url_trimmed;
}

void URLInfoManager::isValidUrl(const QString& url) {
    const auto url_trimmed = url.trimmed();

    static const QRegularExpression re(
        R"(^(https?|ftp)://[^\s/$.?#].[^\s]*$)",
        QRegularExpression::CaseInsensitiveOption
    );

    const auto ifMatch = re.match(url_trimmed).hasMatch();

    emit urlValid(url_trimmed, ifMatch);

    APP::messageCenter()->sendDebug(
        QString("Url '%1' is %2").arg(url_trimmed).arg(ifMatch ? "valid" : "invalid")
    );
}

void URLInfoManager::getInfo(
    const QString& url,
    const QString& yt_dlp_path,
    const QString& ffmpeg_path,
    const QString& node_path)
{
    const auto toolsPath = SharedStorage::instance().getToolsPath().data();

    const auto ytDlpPath = yt_dlp_path.trimmed().isEmpty() ? toolsPath->getYtDlpPath() : yt_dlp_path;
    const auto ffmpegPath = ffmpeg_path.trimmed().isEmpty() ? toolsPath->getFFmpegPath() : ffmpeg_path;
    const auto nodePath = node_path.trimmed().isEmpty() ? toolsPath->getNodePath() : node_path;

    if (this->process) {
        (void)this->process->disconnect();
        this->process->deleteLater();
    }

    this->process = new QProcess(this);

    const QStringList arguments {
        "--ffmpeg-location", ffmpegPath,
        "--no-playlist",
        "--js-runtimes", "node:" + nodePath,
        "--dump-json", url
    };

    connect(this->process, &QProcess::finished, this, &URLInfoManager::handleFinished);

    connect(this->process, &QProcess::errorOccurred, this, [this]() {
        const auto err_str = this->process->errorString();

        emit jsonError(err_str);

        APP::messageCenter()->sendError(
            QString("Error extracting information from url: %1")
                .arg(err_str)
        );
    });

    this->process->start(ytDlpPath, arguments);
}

void URLInfoManager::analyzeUrlInfo(const QJsonObject& obj) {
    static const QRegularExpression re("^[0-9]+$");

    const auto formats = obj["formats"].toArray();
    const auto title = obj["title"].toString().trimmed();
    const auto finalTitle = title.isEmpty() ? tr("Failed") : title;

    QJsonObject info;
    QJsonArray mpegDashAudioFormats;
    QJsonArray mpegDashVideoFormats;
    QJsonArray nonDashFormats;
    QJsonArray aiUpscaledFormats;
    QJsonArray otherFormats;

    // qDebug() << "************************************************************************************";
    // qDebug() << obj["subtitles"];
    // qDebug() << "************************************************************************************";
    // qDebug() << obj["automatic_captions"];
    // qDebug() << "************************************************************************************";

    for (const auto& value : formats) {
        const auto format = value.toObject();
        const auto str = format["format_id"].toString().trimmed().toLower();
        const auto audioYoutubeCodec = format["acodec"].toString().trimmed().toLower();
        const auto videoYoutubeCodec = format["vcodec"].toString().trimmed().toLower();

        if (re.match(str).hasMatch()) {
            const auto num = str.toInt();

            if (MediaCodecFilters::MPEG_DASH_AUDIO.contains(num)) {
                const auto audioCodec = MediaCodecFilters::parseAudioCodec(audioYoutubeCodec).trimmed().toLower();
                const auto finalFormat = QString("%1 (%2)").arg(QString::number(num), audioCodec);
                mpegDashAudioFormats.append(finalFormat);
            }

            else if (MediaCodecFilters::MPEG_DASH_VIDEO.contains(num)) {
                const auto videoCodec = MediaCodecFilters::parseVideoCodec(videoYoutubeCodec).trimmed().toLower();
                const auto finalFormat = QString("%1 (%2)").arg(QString::number(num), videoCodec);
                mpegDashVideoFormats.append(finalFormat);
            }

            else { nonDashFormats.append(num); }
        }

        else {
            static const QRegularExpression re(R"((\d+)-sr$)");
            const auto match = re.match(str);

            if (match.hasMatch()) {
                const auto num = match.captured(1).toInt();

                const auto videoCodec = MediaCodecFilters::parseVideoCodec(videoYoutubeCodec).trimmed().toLower();
                const auto finalFormat = QString("%1-sr (%2)").arg(QString::number(num), videoCodec);

                aiUpscaledFormats.append(finalFormat);
            }

            else otherFormats.append(str);
        }
    }

    info.insert("title", finalTitle);
    info.insert("mpeg_dash_audio_formats", mpegDashAudioFormats);
    info.insert("mpeg_dash_video_formats", mpegDashVideoFormats);
    info.insert("non_dash_formats", nonDashFormats);
    info.insert("ai_upscaled_formats", aiUpscaledFormats);
    info.insert("other_formats", otherFormats);

    APP::messageCenter()->sendDebug(QString::fromUtf8(QJsonDocument(info).toJson(QJsonDocument::Indented)));

    emit jsonReady(info);
}


// Private

void URLInfoManager::handleFinished(const int exitCode, const QProcess::ExitStatus status) {
    if (!this->process) { return; }

    const auto stdoutData = this->process->readAllStandardOutput();
    const auto stderrData = this->process->readAllStandardError();

    APP::messageCenter()->sendDebug(
        QString("Exit Code: %1")
            .arg(exitCode)
    );

    APP::messageCenter()->sendError(stderrData);

    if (!stderrData.isEmpty()) { emit jsonError(stderrData); }

    if (status != QProcess::NormalExit || exitCode != 0) {
        emit jsonError(tr("yt-dlp exited with error"));
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(stdoutData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        emit jsonError(parseError.errorString());
        return;
    }

    if (!doc.isObject()) {
        emit jsonError(tr("JSON is not an object"));
        return;
    }

    this->analyzeUrlInfo(doc.object());
}
