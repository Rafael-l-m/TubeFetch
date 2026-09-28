#pragma once

#include <QLocalServer>

class SingleAppProtection final : public QObject {
    Q_OBJECT

public:
    explicit SingleAppProtection(const QString& serverName, QObject* parent = nullptr);

    bool isRunning() const;

signals:
    void activateMainWindowRequest();

private:
    QLocalServer m_server;
    bool m_running = false;
};
