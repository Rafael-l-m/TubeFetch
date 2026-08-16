#pragma once

#include "download/DownloadWorker.hpp"
#include "download/ProcessPool.hpp"

class DownloadManager final : public QObject {
    Q_OBJECT

public:
    explicit DownloadManager(const int maxConcurrent = 3, QObject* parent = nullptr);

    bool isRunning() const;

    void start(const QSharedPointer<Download>& dt);

    void stop(const QSharedPointer<Download>& dt);

    void startAll();

    void stopAll();

    void setMaxConcurrent(const int maxConcurrent);

signals:
    void downloadTaskChanged(const QSharedPointer<Download>& dt);
    void downloadNormalFinished(const qint64 internalId);
    void downloadUnnormalFinished(const qint64 internalId);
    void downloadErrorOccurred(const qint64 internalId, const QString& message);
    void downloadAllFinished();

private slots:
    void handleDownloadTaskChanged(const QSharedPointer<Download>& dt);
    void handleWorkerNormalFinished(const qint64 internalId);
    void handleWorkerUnnormalFinished(const qint64 internalId);
    void handleWorkerErrorOccurred(const qint64 internalId, const QString& message);
    void handleWorkerTooManyRequests();

private:
    void tryStartNextDownload();
    void startDownload(const QSharedPointer<Download>& task);
    void finishDownload(const qint64 internalId, const bool normal, const bool ifStopOne = false);
    void checkAllFinished();

private:
    mutable QMutex m_mutex;

    int _maxSize;
    bool m_stopping = false;
    bool m_allFinishedEmitted = false;

    std::optional<QDateTime> _begin = std::nullopt;
    std::optional<QDateTime> _end = std::nullopt;

    std::unique_ptr<ProcessPool> m_processPool;
    QHash<qint64, DownloadWorker*> m_workers;
    QSet<qint64> m_currentlyDownloading;
};
