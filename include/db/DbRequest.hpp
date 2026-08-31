#pragma once

#include <db/Operation.hpp>
#include <db/Specification.hpp>
#include <QJsonObject>
#include <QPointer>
#include <QTcpSocket>

struct DbRequest {
    int id;
    Operation op;
    Specification spec;
    QJsonObject params;
    QPointer<QTcpSocket> socket;
};
