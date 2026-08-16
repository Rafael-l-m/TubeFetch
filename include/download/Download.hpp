#pragma once

#include "DownloadStatus.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>

class Download final {
public:
    Download() : internalId(0), url(), title(), audioCode(), videoCode(), nonDashCode(), bestAudio(true),
        bestVideo(false), savePath(), suffix(), metadata(false), subtitles(false), progress(0.0),
        downloadStatus(DownloadStatus::WAITING), downloadedBytes(0.0), totalBytes(0.0), downloadSpeed(0.0),
        eta("00:00") {}

    Download(
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
        const bool metadata = false,
        const bool subtitles = false,
        const double progress = 0,
        const DownloadStatus& downloadStatus = DownloadStatus::WAITING,
        const double downloadedBytes = 0.0,
        const double totalBytes = 0.0,
        const double downloadSpeed = 0.0,
        const QString& eta = "00:00") : internalId(internalId), url(url), title(title), audioCode(audioCode),
        videoCode(videoCode),nonDashCode(nonDashCode), bestAudio(bestAudio), bestVideo(bestVideo),
        savePath(savePath), suffix(suffix),metadata(metadata), subtitles(subtitles), progress(progress),
        downloadStatus(downloadStatus), downloadedBytes(downloadedBytes), totalBytes(totalBytes),
        downloadSpeed(downloadSpeed), eta(eta) {}

    Download(const Download& other) : internalId(other.internalId), url(other.url), title(other.title),
        audioCode(other.audioCode), videoCode(other.videoCode), nonDashCode(other.nonDashCode),
        bestAudio(other.bestAudio), bestVideo(other.bestVideo), savePath(other.savePath), suffix(other.suffix),
        metadata(other.metadata), subtitles(other.subtitles), progress(other.progress),
        downloadStatus(other.downloadStatus), downloadedBytes(other.downloadedBytes), totalBytes(other.totalBytes),
        downloadSpeed(other.downloadSpeed), eta(other.eta) {}

    Download& operator=(const Download& other) {
        std::scoped_lock lock(this->m_mutex, other.m_mutex);

        if (this != &other) {
            this->internalId = other.internalId;
            this->url = other.url.trimmed();
            this->title = other.title.trimmed();
            this->audioCode = other.audioCode.trimmed();
            this->videoCode = other.videoCode.trimmed();
            this->nonDashCode = other.nonDashCode.trimmed();
            this->bestAudio = other.bestAudio;
            this->bestVideo = other.bestVideo;
            this->savePath = other.savePath.trimmed();
            this->suffix = other.suffix.trimmed();
            this->metadata = other.metadata;
            this->subtitles = other.subtitles;
            this->progress = other.progress;
            this->downloadStatus = other.downloadStatus;
            this->downloadedBytes = other.downloadedBytes;
            this->totalBytes = other.totalBytes;
            this->downloadSpeed = other.downloadSpeed;
            this->eta = other.eta.trimmed();
        }

        return *this;
    }

    bool operator==(const Download& other) const { return this->internalId == other.internalId; }

    qint64 getInternalId()             const { QMutexLocker locker(&this->m_mutex); return this->internalId;      }
    QString getUrl()                   const { QMutexLocker locker(&this->m_mutex); return this->url;             }
    QString getTitle()                 const { QMutexLocker locker(&this->m_mutex); return this->title;           }
    QString getAudioCode()             const { QMutexLocker locker(&this->m_mutex); return this->audioCode;       }
    QString getVideoCode()             const { QMutexLocker locker(&this->m_mutex); return this->videoCode;       }
    QString getNonDashCode()           const { QMutexLocker locker(&this->m_mutex); return this->nonDashCode;     }
    bool getBestAudio()                const { QMutexLocker locker(&this->m_mutex); return this->bestAudio;       }
    bool getBestVideo()                const { QMutexLocker locker(&this->m_mutex); return this->bestVideo;       }
    QString getSavePath()              const { QMutexLocker locker(&this->m_mutex); return this->savePath;        }
    QString getSuffix()                const { QMutexLocker locker(&this->m_mutex); return this->suffix;          }
    bool getMetadata()                 const { QMutexLocker locker(&this->m_mutex); return this->metadata;        }
    bool getSubtitles()                const { QMutexLocker locker(&this->m_mutex); return this->subtitles;       }
    double getProgress()               const { QMutexLocker locker(&this->m_mutex); return this->progress;        }
    DownloadStatus getDownloadStatus() const { QMutexLocker locker(&this->m_mutex); return this->downloadStatus;  }
    double getDownloadedBytes()        const { QMutexLocker locker(&this->m_mutex); return this->downloadedBytes; }
    double getTotalBytes()             const { QMutexLocker locker(&this->m_mutex); return this->totalBytes;      }
    double getDownloadSpeed()          const { QMutexLocker locker(&this->m_mutex); return this->downloadSpeed;   }
    QString getEta()                   const { QMutexLocker locker(&this->m_mutex); return this->eta;             }

    void setUrl(const QString& newUrl)                              { QMutexLocker locker(&this->m_mutex); this->url = newUrl.trimmed();                 }
    void setTitle(const QString& newTitle)                          { QMutexLocker locker(&this->m_mutex); this->title = newTitle.trimmed();             }
    void setAudioCode(const QString& newAudioCode)                  { QMutexLocker locker(&this->m_mutex); this->audioCode = newAudioCode.trimmed();     }
    void setVideoCode(const QString& newVideoCode)                  { QMutexLocker locker(&this->m_mutex); this->videoCode = newVideoCode.trimmed();     }
    void setNonDashCode(const QString& newNonDashCode)              { QMutexLocker locker(&this->m_mutex); this->nonDashCode = newNonDashCode.trimmed(); }
    void setBestAudio(const bool newBestAudio)                      { QMutexLocker locker(&this->m_mutex); this->bestAudio = newBestAudio;               }
    void setBestVideo(const bool newBestVideo)                      { QMutexLocker locker(&this->m_mutex); this->bestVideo = newBestVideo;               }
    void setSavePath(const QString& newSavePath)                    { QMutexLocker locker(&this->m_mutex); this->savePath = newSavePath.trimmed();       }
    void setSuffix(const QString& newSuffix)                        { QMutexLocker locker(&this->m_mutex); this->suffix = newSuffix.trimmed();           }
    void setMetadata(const bool newMetadata)                        { QMutexLocker locker(&this->m_mutex); this->metadata = newMetadata;                 }
    void setSubtitles(const bool newSubtitles)                      { QMutexLocker locker(&this->m_mutex); this->subtitles = newSubtitles;               }
    void setProgress(const double newProgress)                      { QMutexLocker locker(&this->m_mutex); this->progress = newProgress;                 }
    void setDownloadStatus(const DownloadStatus& newDownloadStatus) { QMutexLocker locker(&this->m_mutex); this->downloadStatus = newDownloadStatus;     }
    void setDownloadedBytes(const double newDownloadedBytes)        { QMutexLocker locker(&this->m_mutex); this->downloadedBytes = newDownloadedBytes;   }
    void setTotalBytes(const double newTotalBytes)                  { QMutexLocker locker(&this->m_mutex); this->totalBytes = newTotalBytes;             }
    void setDownloadSpeed(const double newDownloadSpeed)            { QMutexLocker locker(&this->m_mutex); this->downloadSpeed = newDownloadSpeed;       }
    void setEta(const QString& newEta)                              { QMutexLocker locker(&this->m_mutex); this->eta = newEta.trimmed();                 }

    QJsonObject toJson() const {
        QMutexLocker locker(&this->m_mutex);

        QJsonObject obj;

        obj["internalId"] = this->internalId;
        obj["url"] = this->url;
        obj["title"] = this->title;
        obj["audioCode"] = this->audioCode;
        obj["videoCode"] = this->videoCode;
        obj["nonDashCode"] = this->nonDashCode;
        obj["bestAudio"] = this->bestAudio;
        obj["bestVideo"] = this->bestVideo;
        obj["savePath"] = this->savePath;
        obj["suffix"] = this->suffix;
        obj["metadata"] = this->metadata;
        obj["subtitles"] = this->subtitles;
        obj["progress"] = this->progress;
        obj["downloadStatus"] = downloadStatusToInt(this->downloadStatus);
        obj["downloadedBytes"] = this->downloadedBytes;
        obj["totalBytes"] = this->totalBytes;
        obj["downloadSpeed"] = this->downloadSpeed;
        obj["eta"] = this->eta;

        return obj;
    }

    static Download fromJson(const QJsonObject& obj) {
        return {
            obj["internalId"].toVariant().toLongLong(),
            obj["url"].toString().trimmed(),
            obj["title"].toString().trimmed(),
            obj["audioCode"].toString().trimmed(),
            obj["videoCode"].toString().trimmed(),
            obj["nonDashCode"].toString().trimmed(),
            obj["bestAudio"].toBool(),
            obj["bestVideo"].toBool(),
            obj["savePath"].toString().trimmed(),
            obj["suffix"].toString().trimmed(),
            obj["metadata"].toBool(),
            obj["subtitles"].toBool(),
            obj["progress"].toDouble(),
            intToDownloadStatus(obj["downloadStatus"].toInt()),
            obj["downloadedBytes"].toDouble(),
            obj["totalBytes"].toDouble(),
            obj["downloadSpeed"].toDouble(),
            obj["eta"].toString().trimmed()
        };
    }

    static QJsonObject toJsonList(const QList<Download>& list) {
        QJsonObject obj;

        QJsonArray arr;

        for (const auto& item : list) { arr.append(item.toJson()); }

        obj["params"] = arr;

        return obj;
    }

    static QList<Download> fromJsonList(const QJsonObject& obj) {
        QList<Download> list;

        const auto _arr = obj["params"];

        if (_arr.isArray()) {
            const auto arr = _arr.toArray();

            for (const auto& item : arr) { list.append(fromJson(item.toObject())); }
        }

        return list;
    }

private:
    mutable QMutex m_mutex;

    qint64 internalId;
    QString url;
    QString title;
    QString audioCode;
    QString videoCode;
    QString nonDashCode;
    bool bestAudio;
    bool bestVideo;
    QString savePath;
    QString suffix;
    bool metadata;
    bool subtitles;
    double progress;
    DownloadStatus downloadStatus;
    double downloadedBytes;
    double totalBytes;
    double downloadSpeed;
    QString eta;
};

inline QDataStream& operator<<(QDataStream& out, const Download& d) {
    out << d.getInternalId() << d.getUrl() << d.getTitle() << d.getAudioCode() << d.getVideoCode()
        << d.getNonDashCode() << d.getBestAudio() << d.getBestVideo() << d.getSavePath() << d.getSuffix()
        << d.getMetadata() << d.getSubtitles() << d.getProgress() << downloadStatusToInt(d.getDownloadStatus())
        << d.getDownloadedBytes() << d.getTotalBytes() << d.getDownloadSpeed() << d.getEta();
    return out;
}

inline QDataStream& operator>>(QDataStream& in, Download& d) {
    qint64 internalId; double progress; double downloadedBytes; double totalBytes; double downloadSpeed;
    bool bestAudio; bool bestVideo; bool metadata; bool subtitles;
    QString url; QString title; QString audioCode; QString videoCode; QString nonDashCode; QString savePath;
    QString suffix; QString eta;
    int _ds;

    in >> internalId >> url >> title >> audioCode >> videoCode >> nonDashCode >> bestAudio >> bestVideo
       >> savePath >> suffix >> metadata >> subtitles >> progress >> _ds >> downloadedBytes
       >> totalBytes >> downloadSpeed >> eta;

    d = Download(
        internalId,
        url,
        title,
        audioCode,
        videoCode,
        nonDashCode,
        bestAudio,
        bestVideo,
        savePath,
        suffix,
        metadata,
        subtitles,
        progress,
        intToDownloadStatus(_ds),
        downloadedBytes,
        totalBytes,
        downloadSpeed,
        eta
    );

    return in;
}

inline QDataStream& operator<< (QDataStream& out, const QSharedPointer<Download>& d) {
    const auto isNull = d.isNull();

    out << isNull;

    if (!isNull) { out << *d; }

    return out;
}

inline QDataStream& operator>> (QDataStream& in, QSharedPointer<Download>& d) {
    bool isNull;

    in >> isNull;

    if (isNull) { d.clear(); }

    else {
        d = QSharedPointer<Download>::create();
        in >> *d;
    }

    return in;
}
