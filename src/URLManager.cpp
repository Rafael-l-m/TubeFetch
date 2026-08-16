
#include "URLManager.h"
#include <QProcess>


// Public

bool URLManager::isValidURL(const QString& url) {
    static const QRegularExpression pattern(R"(^(https?|ftp)://[^\s/$.?#].[^\s]*$)", QRegularExpression::CaseInsensitiveOption);
    return pattern.match(url).hasMatch();
}

QString URLManager::cutURL(const QString& url) {
    static const QRegularExpression pattern(R"regex(https?://(www\.)?(youtube\.com/watch\?v=|youtu\.be/))regex", QRegularExpression::CaseInsensitiveOption);

    if (pattern.match(url).hasMatch()) {  // Case 1: YouTube
        if (const auto pos = url.indexOf('&'); pos != -1) return url.left(pos);
    }

    return url;
}

nlohmann::json URLManager::getInfo(const QString& yt_dlp_path, const QString& ffmpeg_path, const QString& url) {
    QProcess process;

    QStringList arguments;
    arguments << "--ffmpeg-location" << ffmpeg_path
              << "--no-playlist"
              << "--dump-json" << url;

    process.start(yt_dlp_path, arguments);

    if (const auto finished = process.waitForFinished(60000); !finished) {
        process.kill();
        return {};
    }

    const auto output = process.readAllStandardOutput();

    if (const auto errorOutput = process.readAllStandardError(); !errorOutput.isEmpty()) qWarning() << "Error OutPut" << errorOutput;

    try {
        const nlohmann::json j = nlohmann::json::parse(output.toStdString());
        return j;
    }

    catch (const std::exception& err) {
        qWarning() << "Parsing json failed" << err.what();
        qDebug() << "Debug content: " << output;
    }

    return {};
}
