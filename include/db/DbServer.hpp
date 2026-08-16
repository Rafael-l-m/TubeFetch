#pragma once

#include "db/DbManager.hpp"
#include "db/DbRequest.hpp"
#include <QTcpServer>

class DbServer final : public QObject {
    Q_OBJECT

public:
    explicit DbServer(QObject* parent = nullptr);

    bool start();

    int getPort() const;

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    static void sendResponse(const DbRequest& req, const QJsonObject& data);
    static void sendError(QTcpSocket* socket, const int id, const QString& msg);

private:
    quint16 port;
    QTcpServer server;
    DbManager* manager;
    QHash<Operation, std::function<void(const DbRequest&)>> handlers;
};
