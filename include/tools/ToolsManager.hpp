#pragma once

#include <QMutex>

class ToolsPathManager final {
public:
    ToolsPathManager() : yt_dlp_path(), ffmpeg_path(), node_path() {}

    ToolsPathManager(
        const QString& yt_dlp_path,
        const QString& ffmpeg_path,
        const QString& node_path) : yt_dlp_path(yt_dlp_path), ffmpeg_path(ffmpeg_path), node_path(node_path) {}

    ToolsPathManager(
        const ToolsPathManager& other ) : yt_dlp_path(other.yt_dlp_path), ffmpeg_path(other.ffmpeg_path), node_path(other.node_path) {}

    QString getYtDlpPath()  const { QMutexLocker locker(&this->mutex); return this->yt_dlp_path; }
    QString getFFmpegPath() const { QMutexLocker locker(&this->mutex); return this->ffmpeg_path; }
    QString getNodePath()   const { QMutexLocker locker(&this->mutex); return this->node_path;   }

    void setYtDlpPath(const QString& ytDlpPath)   { QMutexLocker locker(&this->mutex); this->yt_dlp_path = ytDlpPath;  }
    void setFFmpegPath(const QString& ffmpegPath) { QMutexLocker locker(&this->mutex); this->ffmpeg_path = ffmpegPath; }
    void setNodePath(const QString& nodePath)     { QMutexLocker locker(&this->mutex); this->node_path = nodePath;     }

    void setAll(const ToolsPathManager& other) {
        this->setYtDlpPath(other.yt_dlp_path);
        this->setFFmpegPath(other.ffmpeg_path);
        this->setNodePath(other.node_path);
    }

private:
    mutable QMutex mutex;

    QString yt_dlp_path;
    QString ffmpeg_path;
    QString node_path;
};
