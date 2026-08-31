#include <download/ProcessPool.hpp>


// Public

ProcessPool::ProcessPool(const int maxProcessesPermitted, QObject* parent) : QObject(parent), maxProcessesPermitted(maxProcessesPermitted) {
    for (int i = 0; i < maxProcessesPermitted; ++i) {
        auto* process = new QProcess(this);

        this->m_idleProcesses.enqueue(process);
        this->m_allProcesses.insert(process);
    }
}

QProcess* ProcessPool::acquire() {
    if (this->m_idleProcesses.isEmpty()) { return nullptr; }
    return this->m_idleProcesses.dequeue();
}

void ProcessPool::release(QProcess* process) {
    if (!process) { return; }

    if (!this->m_allProcesses.contains(process)) { return; }

    if (process->state() != QProcess::NotRunning) {
        process->kill();
        process->waitForFinished();
    }

    process->readAllStandardOutput();
    process->readAllStandardError();
    process->disconnect();

    if (this->m_allProcesses.size() > this->maxProcessesPermitted) {
        this->m_allProcesses.remove(process);
        process->deleteLater();
        return;
    }

    if (!this->m_idleProcesses.contains(process)) { this->m_idleProcesses.enqueue(process); }
}

int ProcessPool::idleCount() const { return static_cast<int>(this->m_idleProcesses.size()); }

int ProcessPool::size() const { return static_cast<int>(this->m_allProcesses.size()); }

void ProcessPool::setMaxProcessPermitted(const int mpp) {
    const auto newSize = qMax(0, mpp);

    if (newSize  == this->maxProcessesPermitted) { return; }

    if (newSize > this->maxProcessesPermitted) {
        for (int i = this->maxProcessesPermitted; i < newSize; ++i) {
            auto* process = new QProcess(this);
            this->m_idleProcesses.enqueue(process);
            this->m_allProcesses.insert(process);
        }

        this->maxProcessesPermitted = newSize;
        return;
    }

    this->maxProcessesPermitted = newSize;

    while (this->m_allProcesses.size() > this->maxProcessesPermitted &&
           !this->m_idleProcesses.isEmpty())
    {
        auto* process = this->m_idleProcesses.dequeue();

        if (!process) { continue; }

        if (process->state() != QProcess::NotRunning) {
            process->kill();
            process->waitForFinished();
        }

        this->m_allProcesses.remove(process);

        process->deleteLater();
    }
}

int ProcessPool::getMaxProcessPermitted() const { return this->maxProcessesPermitted; }
