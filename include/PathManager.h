
#ifndef YOUTUBEMEDIADOWNLOADER_PATHMANAGER_H
#define YOUTUBEMEDIADOWNLOADER_PATHMANAGER_H

#include <QString>

class PathManager {
public:
    static PathManager &instance();

    [[nodiscard]] QString dataBasePath() const;

private:
    const QString dataBaseName = "YouTubeMediaDownloader.sqlite";

    PathManager();
    QString dataBaseDir;
};

#endif //YOUTUBEMEDIADOWNLOADER_PATHMANAGER_H
