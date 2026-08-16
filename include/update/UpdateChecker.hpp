#pragma once

#include <QNetworkAccessManager>

class UpdateChecker final : public QObject {
    Q_OBJECT

public:
    explicit UpdateChecker(QObject* parent = nullptr);

    Q_INVOKABLE void checkUpdate();
    Q_INVOKABLE static void visitRepo();
    Q_INVOKABLE static void helpDoc();
    Q_INVOKABLE static void reportIssues();

signals:
    void updateChecked(const int updateStatus, const QString& latestVersion, const QString& notes, const QString& downloadUrl, const QString& message);

private:
    static bool versionComparator(const QString& v1, const QString& v2);
    static QString findDownloadUrl(const QJsonArray& assets);

private:
    QNetworkAccessManager* m_manager;    
};
