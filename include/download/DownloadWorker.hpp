#pragma once

#include <download/Download.hpp>
#include <QProcess>

class DownloadWorker final : public QObject {
    Q_OBJECT

public:
    explicit DownloadWorker(
        const QSharedPointer<Download>& dt,
        QProcess* process,
        QObject* parent = nullptr
    );

    QProcess* getProcess() const;
    void setProcess(QProcess* newProcess);
    qint64 getInternalId() const;

public slots:
    void start();
    void stop();

signals:
    void modelChanged(const QSharedPointer<Download>& dt);
    void normalFinished(qint64 internalId);
    void unnormalFinished(qint64 internalId);
    void downloadPaused(qint64 internalId);
    void tooManyRequest();
    void errorOccurred(qint64 internalId, const QString& message);
    void failedAtStart(qint64 internalId);
    void subprocessErrorOccurred(qint64 internalId, const QString& message);

private:
    QSharedPointer<Download> m_task = nullptr;
    QProcess* m_process = nullptr;
};
