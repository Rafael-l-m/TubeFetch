#pragma once

#include <download/DownloadWorker.hpp>
#include <download/ProcessPool.hpp>
#include <QQueue>

class DownloadManager final : public QObject {
    Q_OBJECT

public:
    explicit DownloadManager(int maxConcurrent = 2, QObject* parent = nullptr);

    void addDownload(const QSharedPointer<Download>& dt);
    void setDownloads(const QList<QSharedPointer<Download>>& dl);
    void removeDownload(qint64 internalId);
    void removeDownload(const QSharedPointer<Download>& dt);
    void clearDownloads();

    void startDownloads();
    void stopDownloads();

    void setMaxConcurrent(const int mc);

signals:
    void downloadModelChanged(const QSharedPointer<Download>& dt);
    void downloadNormalFinished(qint64 internalId);
    void downloadUnnormalFinished(qint64 internalId);
    void downloadPaused(qint64 internalId);
    void downloadErrorOccurred(qint64 internalId, const QString& message);
    void downloadFailedAtStart(qint64 internalId);
    void downloadSubprocessErrorOccurred(qint64 internalId, const QString& message);
    void downloadAllFinished();
    void requestConsume(qint64 internalId, int type, int weight);

private:
    void tryStartNextDownload();
    void startDownload(const QSharedPointer<Download>& task);
    void finishDownload(qint64 internalId, bool normal);
    void checkAllFinished();

private:
    mutable QMutex m_mutex;

    int maxConcurrent;
    bool m_downloading = false;

    std::optional<QDateTime> _begin = std::nullopt;
    std::optional<QDateTime> _end = std::nullopt;

    std::unique_ptr<ProcessPool> m_processPool;
    QHash<qint64, DownloadWorker*> m_workers;

    QSharedPointer<QQueue<QSharedPointer<Download>>> m_downloadQueue;
};
