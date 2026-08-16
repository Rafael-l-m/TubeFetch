#pragma once

#include <QProcess>
#include <QQueue>
#include <QSet>

class ProcessPool final : public QObject {
    Q_OBJECT

public:
    explicit ProcessPool(const int maxSize = 3, QObject* parent = nullptr);

    QProcess* acquire();
    void release(QProcess* process);

    int idleCount() const;
    int size() const;

    void setMaxSize(const int ms);
    int getMaxSize() const;

private:
    int maxSize;
    QQueue<QProcess*> m_idleProcesses;
    QSet<QProcess*> m_allProcesses;
};
