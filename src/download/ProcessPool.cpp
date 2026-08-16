#include "download/ProcessPool.hpp"

ProcessPool::ProcessPool(const int maxSize, QObject* parent) : QObject(parent), maxSize(maxSize) {
    for (int i = 0; i < maxSize; ++i) {
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

    if (this->m_allProcesses.size() > this->maxSize) {
        this->m_allProcesses.remove(process);

        process->deleteLater();

        return;
    }

    if (!this->m_idleProcesses.contains(process)) { this->m_idleProcesses.enqueue(process); }
}

int ProcessPool::idleCount() const { return static_cast<int>(this->m_idleProcesses.size()); }

int ProcessPool::size() const { return static_cast<int>(this->m_allProcesses.size()); }

void ProcessPool::setMaxSize(const int ms) {
    const auto newSize = qMax(0, ms);

    if (newSize == this->maxSize) { return; }

    if (newSize > this->maxSize) {
        for (int i = maxSize; i < newSize; ++i) {
            auto* process = new QProcess(this);

            this->m_idleProcesses.enqueue(process);
            this->m_allProcesses.insert(process);
        }

        this->maxSize = newSize;

        return;
    }

    this->maxSize = newSize;

    while (this->m_allProcesses.size() > this->maxSize && !this->m_idleProcesses.isEmpty()) {
        auto* process = this->m_idleProcesses.dequeue();

        this->m_allProcesses.remove(process);

        if (process->state() != QProcess::NotRunning) {
            process->kill();
            process->waitForFinished();
        }

        process->deleteLater();
    }
}

int ProcessPool::getMaxSize() const { return this->maxSize; }
