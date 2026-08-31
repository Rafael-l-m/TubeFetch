#pragma once

#include <download/Download.hpp>
#include <tools/Tools.hpp>

class SharedStorage final {
public:
    static SharedStorage& instance() { static SharedStorage inst; return inst; }

    SharedStorage() {
        this->tools = QSharedPointer<Tools>::create();
        this->downloadList = QSharedPointer<QList<QSharedPointer<Download>>>::create();
    }

    // Tools Path
    void setToolsPath(const Tools& tpm) const { QMutexLocker locker(&this->mutex); this->tools->setAll(tpm); }
    void setToolsPath(const QSharedPointer<Tools> tmp) { QMutexLocker locker(&this->mutex); this->tools = tmp; }
    QSharedPointer<Tools> getToolsPath() const { QMutexLocker locker(&this->mutex); return this->tools; }

    // Download List
    void addDownload(const Download& newDownload) { this->addDownload(QSharedPointer<Download>::create(newDownload)); }
    void addDownload(const QSharedPointer<Download>& newDownload) { QMutexLocker locker(&this->mutex); this->downloadList->append(newDownload); this->sortDownload(); }
    void addDownload(const QList<QSharedPointer<Download>>& newDownloads) { for (const auto& d : newDownloads) { this->addDownload(d); } }
    void addDownload(const QSharedPointer<QList<QSharedPointer<Download>>>& newDownloads) { this->addDownload(*newDownloads); }
    void removeDownload(const Download& download) { this->removeDownload(QSharedPointer<Download>::create(download)); }
    void removeDownload(const QList<QSharedPointer<Download>>& downloads) { for (const auto& d : downloads) { this->removeDownload(d); } }
    void removeDownload(const QSharedPointer<QList<QSharedPointer<Download>>>& downloads) { this->removeDownload(*downloads); }
    void clearDownloads() { QMutexLocker locker(&this->mutex); this->downloadList->clear(); }

    void removeDownload(const QSharedPointer<Download>& download) {
        QMutexLocker locker(&this->mutex);

        auto& list = *this->downloadList;
        const qint64 idd = download->getInternalId();

        for (auto it = list.begin(); it != list.end();) {
            if ((*it)->getInternalId() == idd) { it = list.erase(it); }
            else { ++it; }
        }
    }

    QSharedPointer<Download> getDownload(const qint64 internalId) {
        const auto ds = this->getDownloads();

        QMutexLocker locker(&this->mutex);

        QSharedPointer<Download> found;

        for (const auto& it : *ds) {
            if (it && it->getInternalId() == internalId) {
                found = it;
                break;
            }
        }

        return found;
    }

    QSharedPointer<QList<QSharedPointer<Download>>> getDownloads() const { QMutexLocker locker(&this->mutex); return this->downloadList; }
    int getDownloadsSize() const { QMutexLocker locker(&this->mutex); return static_cast<int>(this->downloadList->size()); }

private:
    void sortDownload() const {
        std::sort(
            this->downloadList->begin(),
            this->downloadList->end(),
            [](const QSharedPointer<Download>& a, const QSharedPointer<Download>& b) {
                return a->getInternalId() < b->getInternalId();
        });
    }

private:
    mutable QMutex mutex;

    QSharedPointer<Tools> tools;
    QSharedPointer<QList<QSharedPointer<Download>>> downloadList;
};
