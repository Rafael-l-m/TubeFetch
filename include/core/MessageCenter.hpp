#pragma once

#include <QObject>

class MessageCenter final : public QObject {
    Q_OBJECT

public:
    explicit MessageCenter(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void sendDebug   (const QString& message) { emit debugSent(message);   }
    Q_INVOKABLE void sendInfo    (const QString& message) { emit infoSent(message);    }
    Q_INVOKABLE void sendWarning (const QString& message) { emit warningSent(message); }
    Q_INVOKABLE void sendError   (const QString& message) { emit errorSent(message);   }
    Q_INVOKABLE void sendOutput  (const QString& message) { emit outputSent(message);  }

signals:
    void debugSent   (const QString& message);
    void infoSent    (const QString& message);
    void warningSent (const QString& message);
    void errorSent   (const QString& message);
    void outputSent  (const QString& message);
};
