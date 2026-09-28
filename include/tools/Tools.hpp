#pragma once

#include <QMutex>

class Tools final {
public:
    Tools() : ytDlpPath(), ffmpegPath(), nodePath() {}
    Tools(const QString& ytDlpPath, const QString& ffmpegPath, const QString& nodePath) : ytDlpPath(ytDlpPath.trimmed()), ffmpegPath(ffmpegPath.trimmed()), nodePath(nodePath.trimmed()) {}
    Tools(const Tools& other) : ytDlpPath(other.ytDlpPath.trimmed()), ffmpegPath(other.ffmpegPath.trimmed()), nodePath(other.nodePath.trimmed()) {}

    QString getYtDlpPath()  const { QMutexLocker locker(&this->mutex); return this->ytDlpPath.trimmed();  }
    QString getFFmpegPath() const { QMutexLocker locker(&this->mutex); return this->ffmpegPath.trimmed(); }
    QString getNodePath()   const { QMutexLocker locker(&this->mutex); return this->nodePath.trimmed();   }

    void setYtDlpPath(const QString& newYtDlpPath)   { QMutexLocker locker(&this->mutex); this->ytDlpPath  = newYtDlpPath.trimmed();  }
    void setFFmpegPath(const QString& newFFmpegPath) { QMutexLocker locker(&this->mutex); this->ffmpegPath = newFFmpegPath.trimmed(); }
    void setNodePath(const QString& newNodePath)     { QMutexLocker locker(&this->mutex); this->nodePath   = newNodePath.trimmed();   }

    void setAll(const Tools& other) {
        QMutexLocker locker(&this->mutex);
        this->ytDlpPath = other.ytDlpPath.trimmed();
        this->ffmpegPath = other.ffmpegPath.trimmed();
        this->nodePath = other.nodePath.trimmed();
    }

private:
    mutable QMutex mutex;

    QString ytDlpPath;
    QString ffmpegPath;
    QString nodePath;
};
