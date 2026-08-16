#include "core/APP.hpp"
#include "core/Constants.hpp"
#include "update/UpdateChecker.hpp"
#include <QDesktopServices>
#include <QDir>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>


// Public

UpdateChecker::UpdateChecker(QObject* parent) : QObject(parent) { this->m_manager = new QNetworkAccessManager(this); }

void UpdateChecker::checkUpdate() {
    QNetworkRequest req(UPDATE::URL_CHECK_UPDATE);
    req.setRawHeader("Cache-Control", "no-cache");

    auto* reply = this->m_manager->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply](){
        if (reply->error() != QNetworkReply::NoError) {
            emit updateChecked(-1, {}, {}, {}, reply->errorString());

            APP::messageCenter()->sendError(reply->errorString());

            reply->deleteLater();

            return;
        }

        const auto inf = reply->readAll();
        reply->deleteLater();

        QJsonParseError parseError;
        const auto doc = QJsonDocument::fromJson(inf, &parseError);

        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            emit updateChecked(-1, {}, {}, {}, parseError.errorString());

            APP::messageCenter()->sendError(parseError.errorString());

            return;
        }

        const auto obj = doc.object();

        const auto latestVersion = obj["tag_name"].toString().trimmed();
        const auto notes = obj["body"].toString().trimmed();
        const auto ass = obj["assets"];

        APP::messageCenter()->sendDebug(QString("Current Verion: %1").arg(UPDATE::CURRENT_VERSION));
        APP::messageCenter()->sendDebug(QString("Latest Version: %1").arg(latestVersion));

        QString downloadUrl;

        if (ass.isArray()) { downloadUrl = this->findDownloadUrl(ass.toArray()); }

        const auto ifExists = this->versionComparator(latestVersion, UPDATE::CURRENT_VERSION);

        if (ifExists) { emit updateChecked(1, latestVersion, notes, downloadUrl, {}); }

        else {
            emit updateChecked(0, {}, {}, {}, tr("The current version is already the newest one"));
            APP::messageCenter()->sendError("It is already up to date");
        }
    });
}

void UpdateChecker::visitRepo() { QDesktopServices::openUrl(UPDATE::URL_REPO); }

void UpdateChecker::helpDoc() {
    #ifdef Q_OS_MACOS
        const auto pdfPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../Resources/HelpDoc_v2.pdf");
    #else
        const auto pdfPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/HelpDoc_v2.pdf");
    #endif

    APP::messageCenter()->sendDebug(
        QString("HelpDoc path: %1")
            .arg(pdfPath)
    );

    if (!QFile::exists(pdfPath)) {
        APP::messageCenter()->sendWarning(
            QString("No such file: %1")
                .arg(pdfPath)
        );

        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath));
}

void UpdateChecker::reportIssues() { QDesktopServices::openUrl(UPDATE::URL_REPORT_ISSUES); }


// Private

bool UpdateChecker::versionComparator(const QString& v1, const QString& v2) {
    auto split = [](const QString& v) {
        const auto parts = v.trimmed().remove(QRegularExpression("^[vV]")).split(".");

        QList<int> nums;

        for (const auto& p : parts) {
            auto ok = false;

            const auto num = p.toInt(&ok);

            if (ok) { nums << num; }
        }

        return nums;
    };

    const auto a = split(v1);
    const auto b = split(v2);

    for (int i = 0; i < qMax(a.size(), b.size()); ++i) {
        const auto x = i < a.size() ? a[i] : 0;
        const auto y = i < b.size() ? b[i] : 0;

        if (x != y) return x > y;
    }

    return false;
}

QString UpdateChecker::findDownloadUrl(const QJsonArray& assets) {
    QStringList keywords;

    #if defined (Q_PROCESSOR_ARM_64)
        keywords << "arm64";
    #elif defined(Q_PROCESSOR_X86_64)
        keywords << "x86-64";
    #endif

    #if defined(Q_OS_MACOS)
        keywords << "macOs";
        keywords << ".dmg";
    #elif defined(Q_OS_WIN)
        keywords << "Windows";
        keywords << ".exe";
    #elif defined(Q_OS_LINUX)
        keywords << "Linux";
        keywords << ".AppImage"
    #endif

    for (const auto& asset : assets) {
        const auto obj = asset.toObject();
        const auto name = obj["name"].toString().trimmed();

        auto matched = true;

        for (const auto& keyword : keywords) {
            if (!name.contains(keyword, Qt::CaseInsensitive)) {
                matched = false;
                break;
            }
        }

        if (matched) { return obj["browser_download_url"].toString().trimmed(); }
    }

    return {};
}
