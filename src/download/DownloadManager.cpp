#include "core/APP.hpp"
#include "core/Constants.hpp"
#include "download/DownloadManager.hpp"
#include "SharedStorage.hpp"
#include <QTimer>


// Public

DownloadManager::DownloadManager(
    const int maxConcurrent,
    QObject* parent) : QObject(parent), _maxSize(maxConcurrent)
{
    this->m_processPool = std::make_unique<ProcessPool>(maxConcurrent, this);
}

bool DownloadManager::isRunning() const {
    QMutexLocker locker(&m_mutex);
    return !this->m_currentlyDownloading.isEmpty();
}

void DownloadManager::start(const QSharedPointer<Download>& dt) {
    if (!dt) { return; }

    QMutexLocker locker(&m_mutex);

    this->m_stopping = false;
    this->m_allFinishedEmitted = false;

    this->startDownload(dt);
}

void DownloadManager::stop(const QSharedPointer<Download>& dt) {
    if (!dt) { return; }
    this->finishDownload(dt->getInternalId(), false, true);
}

void DownloadManager::startAll() {
    {
        QMutexLocker locker(&m_mutex);

        this->m_stopping = false;
        this->m_allFinishedEmitted = false;
    }

    QTimer::singleShot(0, this, &DownloadManager::tryStartNextDownload);
}

void DownloadManager::stopAll() {
    QList<DownloadWorker*> workers;

    {
        QMutexLocker locker(&m_mutex);

        this->m_stopping = true;

        workers = this->m_workers.values();

        this->m_workers.clear();

        this->m_currentlyDownloading.clear();
    }

    for (auto& worker : workers) {

        if (!worker) { continue; }

        worker->stop();

        auto* process = worker->getProcess();

        this->m_processPool->release(process);

        worker->deleteLater();
    }

    this->checkAllFinished();
}

void DownloadManager::setMaxConcurrent(const int maxConcurrent) {
    if (maxConcurrent > DOWNLOAD_MANAGER::MAX_CONCURRENT) { return; }

    QMutexLocker locker(&this->m_mutex);

    this->_maxSize = maxConcurrent;
    this->m_processPool->setMaxSize(maxConcurrent);
}


// Private Slots

void DownloadManager::handleDownloadTaskChanged(const QSharedPointer<Download>& dt) { emit downloadTaskChanged(dt); }

void DownloadManager::handleWorkerNormalFinished(const qint64 internalId) {
    QMutexLocker locker(&this->m_mutex);

    if (this->_begin.has_value()) {
        this->_end = QDateTime::currentDateTime();

        auto db = this->_begin.value();
        auto de = this->_end.value();

        if (db.secsTo(de) > DOWNLOAD_MANAGER::WAIT_TIME_TOO_MANY_REQUESTS) {
            auto sz = this->m_processPool->getMaxSize();
            if (++sz <= this->_maxSize) { this->m_processPool->setMaxSize(sz); }

            this->_begin.reset();
            this->_end.reset();
        }
    }

    locker.unlock();

    this->finishDownload(internalId, true);
}

void DownloadManager::handleWorkerUnnormalFinished(const qint64 internalId) {
    this->finishDownload(internalId, false);
}

void DownloadManager::handleWorkerErrorOccurred(const qint64 internalId, const QString& message) {
    emit downloadErrorOccurred(internalId, message);
    this->finishDownload(internalId, false);
}

void DownloadManager::handleWorkerTooManyRequests() {
    QMutexLocker locker(&this->m_mutex);

    this->_begin = QDateTime::currentDateTime();
    this->_end.reset();

    auto sz = this->m_processPool->getMaxSize();

    if (--sz > 0) { this->m_processPool->setMaxSize(sz); }
}


// Private

void DownloadManager::tryStartNextDownload() {
    QMutexLocker locker(&m_mutex);

    if (this->m_stopping) {
        APP::messageCenter()->sendWarning(
            "tryStartNextDownload: false -> Stopping"
        );

        return;
    }

    if (!this->m_processPool) { return; }

    auto tasks = SharedStorage::instance().getDownloads();

    if (!tasks) { return; }

    while (this->m_processPool->idleCount() > 0) {
        QSharedPointer<Download> task;

        for (const auto& candidate : *tasks) {
            if (!candidate) { continue; }

            const auto id = candidate->getInternalId();

            if (this->m_currentlyDownloading.contains(id)) { continue; }

            const auto _ds = candidate->getDownloadStatus();

            if (_ds == DownloadStatus::FINISHED) { continue; }

            task = candidate;

            break;
        }

        if (!task) { break; }

        this->startDownload(task);
    }

    locker.unlock();
}

void DownloadManager::startDownload(const QSharedPointer<Download>& task) {
    if (!task) { return; }

    const auto internalId = task->getInternalId();

    auto* process = this->m_processPool->acquire();

    if (!process) {
        APP::messageCenter()->sendWarning(
            "No available process. Must wait ..."
        );

        emit downloadErrorOccurred(internalId, tr("No available process to download. Must wait ..."));

        return;
    }

    auto* worker = new DownloadWorker(task, process, 3, 5, 3, this);

    this->m_workers.insert(internalId, worker);
    this->m_currentlyDownloading.insert(internalId);

    task->setDownloadStatus(DownloadStatus::DOWNLOADING);

    connect(worker, &DownloadWorker::modelChanged, this, [this](const QSharedPointer<Download>& dt) {
        this->handleDownloadTaskChanged(dt);
    });

    connect(worker, &DownloadWorker::tooManyRequest, this, &DownloadManager::handleWorkerTooManyRequests);

    connect(worker, &DownloadWorker::errorOccurred, this, [this](const qint64 internalId, const QString& message) {
        this->handleWorkerErrorOccurred(internalId, message);
    });

    connect(worker, &DownloadWorker::normalFinished, this, [this](const qint64 internalId) {
        this->handleWorkerNormalFinished(internalId);
    });

    connect(worker, &DownloadWorker::unnormalFinished, this, [this](const qint64 internalId) {
        this->handleWorkerUnnormalFinished(internalId);
    });

    worker->start();
}

void DownloadManager::finishDownload(const qint64 internalId, const bool normal, const bool ifStopOne) {
    QProcess* process = nullptr;

    DownloadWorker* worker = nullptr;

    {
        QMutexLocker locker(&m_mutex);

        worker = this->m_workers.take(internalId);

        if (!worker) { return; }

        this->m_currentlyDownloading.remove(internalId);
    }

    process = worker->getProcess();

    const auto task = SharedStorage::instance().getDownloads();

    if (task) {
        for (const auto& item : *task) {
            if (!item) { continue; }

            if (item->getInternalId() != internalId) { continue; }

            item->setDownloadStatus(
                normal
                    ? DownloadStatus::FINISHED
                    : (ifStopOne
                           ? DownloadStatus::PAUSED
                           : DownloadStatus::ERROR)
            );

            item->setEta("00:00");

            emit downloadTaskChanged(item);

            break;
        }
    }

    worker->stop();

    if (this->m_processPool && process) {
        this->m_processPool->release(process);
    }

    worker->deleteLater();

    if (normal) { emit downloadNormalFinished(internalId); }

    QTimer::singleShot(0, this, &DownloadManager::tryStartNextDownload);

    this->checkAllFinished();
}

void DownloadManager::checkAllFinished() {
    QMutexLocker locker(&m_mutex);

    if (this->m_stopping) {
        APP::messageCenter()->sendWarning(
            "checkAllFinished: false -> Stopping"
        );

        return;
    }

    if (!this->m_currentlyDownloading.isEmpty()) { return; }

    const auto tasks = SharedStorage::instance().getDownloads();

    if (!tasks) { return; }

    for (const auto& task : *tasks) {
        if (!task) { continue; }

        if (task->getDownloadStatus() == DownloadStatus::WAITING ||
            task->getDownloadStatus() == DownloadStatus::DOWNLOADING)
        {
            return;
        }
    }

    this->m_allFinishedEmitted = true;

    locker.unlock();

    APP::messageCenter()->sendDebug(
        "All downloads finished (checkAllFinished)"
    );

    emit downloadAllFinished();
}
