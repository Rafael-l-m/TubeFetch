#pragma once

#include <core/Constants.hpp>
#include <QNetworkAccessManager>
#include <QProcess>

class ProviderManager final : public QObject {
    Q_OBJECT

public:
    explicit ProviderManager(QObject* parent = nullptr);
    ~ProviderManager() override;

    qint64 startProvider(const QString& exePath = TOOLS::poTokenProviderPath(), const QStringList& args = {"server", "--host", "127.0.0.1", "--port", "4416"});

    void stopProvider();

signals:
    void providerStarted(bool ok);

private:
    bool waitForProviderReady(int timeoutMs = 5000);

private:
    static constexpr quint16 ProviderPort = 4416;

    QProcess* process = nullptr;
    QNetworkAccessManager* networkManager = nullptr;
};
