#pragma once

#include <QLocalServer>

class SingleApplication final : public QObject {
    Q_OBJECT

public:
    explicit SingleApplication(const QString& serverName, QObject* parent = nullptr);

    bool isRunning() const;

signals:
    void activateMainWindowRequest();

private:
    QLocalServer m_server;
    bool m_running = false;
};
