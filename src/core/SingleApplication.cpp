#include "core/SingleApplication.hpp"
#include <QLocalSocket>


// Public

SingleApplication::SingleApplication(const QString& serverName, QObject* parent) : QObject(parent) {
    QLocalSocket socket;

    socket.connectToServer(serverName);

    if (socket.waitForConnected(100)) {
        this->m_running = true;

        socket.write("ACTIVATE");
        socket.flush();
        socket.waitForBytesWritten(200);
        socket.disconnectFromServer();

        return;
    }

    QLocalServer::removeServer(serverName);

    if (!this->m_server.listen(serverName)) { this->m_running = true; return; }

    connect(&this->m_server, &QLocalServer::newConnection, this, [this]() {
        auto* socket = this->m_server.nextPendingConnection();

        if (!socket) { return; }

        connect(socket, &QLocalSocket::readyRead, this, [this, socket]() {
            const QByteArray message = socket->readAll();

            if (message == "ACTIVATE") { emit activateMainWindowRequest(); }

            socket->disconnectFromServer();
            socket->deleteLater();
        });
    });
}

bool SingleApplication::isRunning() const { return this->m_running; }
