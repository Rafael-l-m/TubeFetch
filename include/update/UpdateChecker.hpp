#pragma once

#include <QNetworkAccessManager>

class UpdateChecker final : public QObject {
    Q_OBJECT

public:
    explicit UpdateChecker(QObject* parent = nullptr);

    static void visitRepo();
    static void helpDoc();
    static void reportIssues();
    static bool versionComparator(const QString& v1, const QString& v2);

    void checkUpdate();

signals:
    void updateChecked(int updateStatus, const QString& latestVersion, const QString& notes, const QString& downloadUrl, const QString& message);

private:
    static QString findDownloadUrl(const QJsonArray& assets);

private:
    QNetworkAccessManager* m_manager;
};
