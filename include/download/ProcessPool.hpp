#pragma once

#include <QProcess>
#include <QQueue>
#include <QSet>

class ProcessPool final : public QObject {
    Q_OBJECT

public:
    explicit ProcessPool(int maxProcessesPermited = 3, QObject* parent = nullptr);

    QProcess* acquire();
    void release(QProcess* process);

    int idleCount() const;
    int size() const;

    void setMaxProcessPermitted(int mpp);
    int getMaxProcessPermitted() const;


private:
    int maxProcessesPermitted;
    QQueue<QProcess*> m_idleProcesses;
    QSet<QProcess*> m_allProcesses;
};
