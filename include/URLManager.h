
#ifndef YOUTUBEMEDIADOWNLOADER_URLMANAGER_H
#define YOUTUBEMEDIADOWNLOADER_URLMANAGER_H

#include "json.hpp"
#include <QRegularExpression>

class URLManager final {
public:
    static bool isValidURL(const QString& url);

    static QString cutURL(const QString& url);

    static nlohmann::json getInfo(const QString& yt_dlp_path, const QString& ffmpeg_path, const QString& url);
};

#endif //YOUTUBEMEDIADOWNLOADER_URLMANAGER_H
