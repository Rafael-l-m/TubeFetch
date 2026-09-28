#pragma once

#include <QProcess>

class URLInfoManager final : public QObject {
    Q_OBJECT

public:
    explicit URLInfoManager(QObject* parent = nullptr);

    Q_INVOKABLE static QString cutUrl(const QString& url);

    Q_INVOKABLE void isValidUrl(const QString& url);

    Q_INVOKABLE void getInfo(
        const QString& url,
        const QString& yt_dlp_path = "",
        const QString& ffmpeg_path = "",
        const QString& node_path = ""
    );

    Q_INVOKABLE void analyzeUrlInfo(const QJsonObject& obj);

signals:
    void newInfo(const QString& url, const QString& info);
    void urlValid(const QString& url, bool ok);
    void urlCut(const QString& url);
    void jsonReady(const QJsonObject& obj);
    void jsonError(const QString& message);

private:
    void handleFinished(const QString& url, int exitCode, const QProcess::ExitStatus& status);

private:
    QProcess* process = nullptr;
};
