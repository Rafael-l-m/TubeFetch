#pragma once

#include <download/YoutubeActionsType.hpp>
#include <QJsonObject>
#include <QMutex>

class YoutubeActions final {
public:
    YoutubeActions() = default;
    YoutubeActions(const qint64 internalId, const YoutubeActionsType& type, const int weight, const qint64 createdAt = QDateTime::currentSecsSinceEpoch()) : internalId(internalId), createdAt(createdAt), type(type), weight(weight) {}
    YoutubeActions(const YoutubeActions& other) : internalId(other.internalId), createdAt(other.createdAt), type(other.type), weight(other.weight) {}

    YoutubeActions& operator=(const YoutubeActions& other) {
        std::scoped_lock lock(this->m_mutex, other.m_mutex);

        if (this != &other) {
            this->internalId = other.internalId;
            this->createdAt = other.createdAt;
            this->type = other.type;
            this->weight = other.weight;
        }

        return *this;
    }

    bool operator==(const YoutubeActions& other) const { return this->internalId == other.internalId; }

    qint64 getInternalId()       const { QMutexLocker locker(&this->m_mutex); return this->internalId; }
    qint64 getCreatedAt()        const { QMutexLocker locker(&this->m_mutex); return this->createdAt;  }
    YoutubeActionsType getType() const { QMutexLocker locker(&this->m_mutex); return this->type;       }
    int getWeight()              const { QMutexLocker locker(&this->m_mutex); return this->weight;     }

    void setCreatedAt(const qint64 newCreatedAt)    { QMutexLocker locker(&this->m_mutex); this->createdAt = newCreatedAt; }
    void setType(const YoutubeActionsType& newType) { QMutexLocker locker(&this->m_mutex); this->type = newType;           }
    void setWeight(const int newWeight)             { QMutexLocker locker(&this->m_mutex); this->weight = newWeight;       }

    QJsonObject toJson() const {
        QMutexLocker locker(&this->m_mutex);

        QJsonObject obj;

        obj["internalId"] = this->internalId;
        obj["createdAt"] = this->createdAt;
        obj["type"] = youtubeActionsTypeToInt(this->type);
        obj["weight"] = this->weight;

        return obj;
    }

    static YoutubeActions fromJson(const QJsonObject& obj) {
        return {
            obj["internalId"].toVariant().toLongLong(),
            parseYoutubeActionsType(obj["type"].toInt()),
            obj["weight"].toInt(),
            obj["createdAt"].toVariant().toLongLong()
        };
    }

private:
    mutable QMutex m_mutex;

    qint64 internalId;
    qint64 createdAt;
    YoutubeActionsType type;
    int weight;
};
