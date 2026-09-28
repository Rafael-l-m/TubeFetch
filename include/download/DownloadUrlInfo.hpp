#pragma once

#include <QJsonObject>
#include <QMutex>

class DownloadUrlInfo final {
public:
    DownloadUrlInfo() = default;
    DownloadUrlInfo(const qint64 createdAt, QString url, QString info) : createdAt(createdAt), url(std::move(url)), info(std::move(info)) {}
    DownloadUrlInfo(const DownloadUrlInfo& other) : createdAt(other.createdAt), url(other.url), info(other.info) {}

    DownloadUrlInfo& operator=(const DownloadUrlInfo& other) {
        std::scoped_lock lock(this->m_mutex, other.m_mutex);

        if (this != &other) {
            this->createdAt = other.createdAt;
            this->url = other.url;
            this->info = other.info;
        }

        return *this;
    }

    bool operator==(const DownloadUrlInfo& other) const { return this->url.trimmed().compare(other.url.trimmed()) == 0; }

    qint64 getCreatedAt() const { QMutexLocker locker(&this->m_mutex); return this->createdAt; }
    QString getUrl()      const { QMutexLocker locker(&this->m_mutex); return this->url;       }
    QString getInfo()     const { QMutexLocker locker(&this->m_mutex); return this->info;      }

    void setCreatedAt(const qint64 newCreatedAt) { QMutexLocker locker(&this->m_mutex); this->createdAt = newCreatedAt; }
    void setUrl(const QString& newUrl)           { QMutexLocker locker(&this->m_mutex); this->url = newUrl;             }
    void setInfo(const QString& newInfo)         { QMutexLocker locker(&this->m_mutex); this->info = newInfo;           }

    QJsonObject toJson() const {
        QMutexLocker locker(&this->m_mutex);

        QJsonObject obj;

        obj["createdAt"] = this->createdAt;
        obj["url"] = this->url;
        obj["info"] = this->info;

        return obj;
    }

    static DownloadUrlInfo fromJson(const QJsonObject& obj) {
        return {
            obj["createdAt"].toVariant().toLongLong(),
            obj["url"].toString().trimmed(),
            obj["info"].toString().trimmed()
        };
    }

private:
    mutable QMutex m_mutex;

    qint64 createdAt;
    QString url;
    QString info;
};
