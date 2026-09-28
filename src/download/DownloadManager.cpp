#include <core/APP.hpp>
#include <core/Constants.hpp>
#include <download/DownloadManager.hpp>
#include <SharedStorage.hpp>
#include <QTimer>


// Public

DownloadManager::DownloadManager(const int maxConcurrent, QObject* parent) : QObject(parent), maxConcurrent(maxConcurrent) {
    this->m_processPool = std::make_unique<ProcessPool>(maxConcurrent, this);
    this->m_downloadQueue = QSharedPointer<QQueue<QSharedPointer<Download>>>::create();
}

void DownloadManager::addDownload(const QSharedPointer<Download>& dt) {
    if (dt.isNull()) { return; }

    if (this->m_downloadQueue->contains(dt)) { return; }

    this->m_downloadQueue->enqueue(dt);
}

void DownloadManager::setDownloads(const QList<QSharedPointer<Download>>& dl) {
    if (dl.isEmpty()) { return; }

    this->m_downloadQueue->clear();

    for (const auto& item : dl) {
        if (item.isNull()) { continue; }
        if (item->getDownloadStatus() != DownloadStatus::FINISHED) { this->m_downloadQueue->enqueue(item); }
    }
}

void DownloadManager::removeDownload(const qint64 internalId) {
    for (auto it = this->m_downloadQueue->constBegin(); it != this->m_downloadQueue->constEnd(); ++it) {
        if (it->data()->getInternalId() == internalId) {
            this->m_downloadQueue->erase(it);
            break;
        }
    }
}

void DownloadManager::removeDownload(const QSharedPointer<Download>& dt) { if (!dt.isNull()) { this->removeDownload(dt->getInternalId()); } }

void DownloadManager::clearDownloads() { this->m_downloadQueue->clear(); }

void DownloadManager::startDownloads() {
    QMutexLocker locker(&m_mutex);

    this->m_downloading = true;

    const auto ifEmpty = this->m_downloadQueue->isEmpty();

    locker.unlock();

    if (ifEmpty) { emit downloadAllFinished(); return; }

    QTimer::singleShot(300, this, &DownloadManager::tryStartNextDownload);
}

void DownloadManager::stopDownloads() {
    QList<DownloadWorker*> workers;

    {
        QMutexLocker locker(&m_mutex);

        this->m_downloading = false;

        workers = this->m_workers.values();

        this->m_workers.clear();
        this->m_downloadQueue->clear();
    }

    for (auto& worker : workers) {

        if (!worker) { continue; }

        worker->stop();

        auto* process = worker->getProcess();

        if (this->m_processPool && process) { this->m_processPool->release(process); }

        worker->deleteLater();
    }
}

void DownloadManager::setMaxConcurrent(const int mc) {
    if (mc > DOWNLOAD_MANAGER::MAX_CONCURRENT || mc < 1) { return; }

    QMutexLocker locker(&this->m_mutex);

    this->maxConcurrent = mc;

    if (this->m_processPool) {
        this->m_processPool->setMaxProcessPermitted(this->maxConcurrent);
    }
}


// Private

void DownloadManager::tryStartNextDownload() {
    QMutexLocker locker(&m_mutex);

    if (!this->m_downloading) {
        APP::messageCenter()->sendWarning("tryStartNextDownload: false -> downloading");
        return;
    }

    if (!this->m_processPool ||
        this->m_downloadQueue.isNull() ||
        this->m_downloadQueue->isEmpty() ||
        this->m_processPool->idleCount() < 1) { return; }

    const auto ds = this->m_downloadQueue->dequeue();

    locker.unlock();

    this->startDownload(ds);

    emit requestConsume(ds->getInternalId(), 2, 6);

    QTimer::singleShot(0, this, &DownloadManager::tryStartNextDownload);
}

void DownloadManager::startDownload(const QSharedPointer<Download>& task) {
    if (!task || !this->m_processPool) { return; }

    const auto internalId = task->getInternalId();

    auto* process = this->m_processPool->acquire();

    auto* worker = new DownloadWorker(task, process, this);

    QMutexLocker locker(&this->m_mutex);

    this->m_workers.insert(internalId, worker);
    task->setDownloadStatus(DownloadStatus::DOWNLOADING);

    locker.unlock();

    connect(worker, &DownloadWorker::modelChanged, this, &DownloadManager::downloadModelChanged);
    connect(worker, &DownloadWorker::errorOccurred, this, &DownloadManager::downloadErrorOccurred);
    connect(worker, &DownloadWorker::subprocessErrorOccurred, this, &DownloadManager::downloadSubprocessErrorOccurred);
    connect(worker, &DownloadWorker::downloadPaused, this, &DownloadManager::downloadPaused);

    connect(worker, &DownloadWorker::tooManyRequest, this, [this](){
        QMutexLocker locker(&this->m_mutex);

        this->_begin = QDateTime::currentDateTime();
        this->_end.reset();

        auto sz = this->m_processPool->getMaxProcessPermitted();

        if (--sz > 0) { this->m_processPool->setMaxProcessPermitted(sz); }
    });

    connect(worker, &DownloadWorker::normalFinished, this, [this](const qint64 internalId) {
        QMutexLocker locker(&this->m_mutex);

        if (this->_begin.has_value()) {
            this->_end = QDateTime::currentDateTime();

            auto db = this->_begin.value();
            auto de = this->_end.value();

            if (db.secsTo(de) > DOWNLOAD_MANAGER::WAIT_TIME_TOO_MANY_REQUESTS) {
                auto sz = this->m_processPool->getMaxProcessPermitted();
                if (++sz <= this->maxConcurrent) { this->m_processPool->setMaxProcessPermitted(sz); }

                this->_begin.reset();
                this->_end.reset();
            }
        }

        locker.unlock();

        this->finishDownload(internalId, true);
    });

    connect(worker, &DownloadWorker::unnormalFinished, this, [this](const qint64 internalId) {
        this->finishDownload(internalId, false);
    });

    connect(worker, &DownloadWorker::failedAtStart, this, [this](const qint64 internalId){
        QMutexLocker locker(&this->m_mutex);

        auto* worker = this->m_workers.take(internalId);

        locker.unlock();

        if (worker) {
            auto* process = worker->getProcess();

            if (process) { worker->stop(); }

            if (this->m_processPool && process) {
                this->m_processPool->release(process);
            }

            worker->deleteLater();
        }

        emit downloadFailedAtStart(internalId);

        QTimer::singleShot(300, this, &DownloadManager::tryStartNextDownload);

        this->checkAllFinished();
    });

    worker->start();
}

void DownloadManager::finishDownload(const qint64 internalId, const bool normal) {
    QMutexLocker locker(&this->m_mutex);

    auto* worker = this->m_workers.take(internalId);

    locker.unlock();

    if (worker) {
        auto* process = worker->getProcess();

        if (process) { worker->stop(); }

        if (this->m_processPool && process) {
            this->m_processPool->release(process);
        }

        worker->deleteLater();
    }

    if (normal) { emit downloadNormalFinished(internalId); }
    else { emit downloadUnnormalFinished(internalId); }

    QTimer::singleShot(300, this, &DownloadManager::tryStartNextDownload);

    this->checkAllFinished();
}

void DownloadManager::checkAllFinished() {
    auto finished = false;

    {
        QMutexLocker locker(&m_mutex);

        if (!this->m_downloading) { return; }

        if (this->m_downloadQueue->isEmpty() && this->m_workers.isEmpty()) {
            this->m_downloading = false;
            finished = true;
        }
    }

    if (finished) { emit downloadAllFinished(); }
}
