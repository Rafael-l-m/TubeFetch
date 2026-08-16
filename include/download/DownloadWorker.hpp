#pragma once

#include "download/Download.hpp"
#include <QProcess>

class DownloadWorker final : public QObject {
    Q_OBJECT

public:
    explicit DownloadWorker(
        const QSharedPointer<Download>& dt,
        QProcess* process,
        const int retries = 2,
        const int fragmentRetries = 3,
        const int concurrentRetries = 1,
        QObject* parent = nullptr
    );

    QProcess* getProcess() const;

    qint64 getInternalId() const;

public slots:
    void start();
    void stop();

signals:
    void modelChanged(const QSharedPointer<Download>& dt);
    void normalFinished(const qint64 internalId);
    void unnormalFinished(const qint64 internalId);
    void tooManyRequest();
    void errorOccurred(const qint64 internalId, const QString& message);

private:
    const int retries;
    const int fragmentRetries;
    const int concurrentRetries;
    QSharedPointer<Download> m_task;
    QProcess* m_process = nullptr;
};
