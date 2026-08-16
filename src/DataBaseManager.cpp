
#include "DataBaseManager.h"
#include <QSqlError>


// Public

DataBaseManager::DataBaseManager(const QString& dbPath, QObject* parent) : QObject(parent) {
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(dbPath);

    if (!this->db.open()) qDebug() << "Failed to open database" << this->db.lastError().text();

    else {if (!createTable()) qDebug() << "Failed to create new Database" << this->db.lastError().text();}
}

bool DataBaseManager::isOpen() const {return this->db.isOpen();}

bool DataBaseManager::addDownload(const int id, const QString& url, const int audioCode, const int videoCode, const QString& filePath, const QString& suffix, const QString& oldTitle, const QString& newTitle, const int progress, const bool isDownload, const bool withMetadata) const {
    QSqlQuery q(this->db);

    q.prepare("INSERT INTO downloads (id, url, audio_code, video_code, file_path, suffix, old_title, new_title, progress, is_download, with_metadata)"
              "VALUES (:id, :url, :audio, :video, :path, :suf, :ot, :nt, :prog, :isd, :ism)");

    q.bindValue(":id", id);
    q.bindValue(":url", url);
    q.bindValue(":audio", audioCode);
    q.bindValue(":video", videoCode);
    q.bindValue(":path", filePath);
    q.bindValue(":suf", suffix);
    q.bindValue(":ot", oldTitle);
    q.bindValue(":nt", newTitle);
    q.bindValue(":prog", progress);
    q.bindValue(":isd", isDownload ? 1 : 0);
    q.bindValue(":ism", withMetadata ? 1 : 0);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

DownloadItem* DataBaseManager::readDownload(const int id) const {
    QSqlQuery q(this->db);

    q.prepare("SELECT id, url, audio_code, video_code, file_path, suffix, old_title, new_title, progress, is_download, with_metadata FROM downloads WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "Query failed:" << q.lastError().text();
        return {};
    }

    if (q.next()) {
        auto* download = new DownloadItem();

        download->id = q.value("id").toInt();
        download->url = q.value("url").toString();
        download->audio_code = q.value("audio_code").toInt();
        download->video_code = q.value("video_code").toInt();
        download->file_path = q.value("file_path").toString();
        download->suffix = q.value("suffix").toString();
        download->oldTitle = q.value("old_title").toString();
        download->newTitle = q.value("new_title").toString();
        download->progress = q.value("progress").toInt();
        download->isDownload = q.value("is_download").toInt() != 0;
        download->withMetadata = q.value("with_metadata").toInt() != 0;

        return download;
    }

    return new DownloadItem();
}

QList<DownloadItem *>* DataBaseManager::readAllDownloads() const {
    auto* list = new QList<DownloadItem*>();

    QSqlQuery q("SELECT id, url, audio_code, video_code, file_path, suffix, old_title, new_title, progress, is_download, with_metadata FROM downloads", this->db);

    while (q.next()) {
        auto* item = new DownloadItem();

        item->id = q.value("id").toInt();
        item->url = q.value("url").toString();
        item->audio_code = q.value("audio_code").toInt();
        item->video_code = q.value("video_code").toInt();
        item->file_path = q.value("file_path").toString();
        item->suffix = q.value("suffix").toString();
        item->oldTitle = q.value("old_title").toString();
        item->newTitle = q.value("new_title").toString();
        item->progress = q.value("progress").toInt();
        item->isDownload = q.value("is_download").toInt() != 0;
        item->withMetadata = q.value("with_metadata").toInt() != 0;

        list->append(item);
    }

    return list;
}

bool DataBaseManager::updateAudioCode(const int id, const int newAudioCode) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET audio_code = :ac WHERE id = :id");
    q.bindValue(":ac", newAudioCode);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::updateVideoCode(const int id, const int newVideoCode) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET video_code = :vc WHERE id = :id");

    q.bindValue(":vc", newVideoCode);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::updateNewTitle(const int id, const QString& newTitle) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET new_title = :nt WHERE id = :id");
    q.bindValue(":nt", newTitle);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
};

bool DataBaseManager::updateFilePath(const int id, const QString& filePath) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET file_path = :fp WHERE id = :id");
    q.bindValue(":fp", filePath);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::updateSuffix(const int id, const QString& suffix) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET suffix = :suf WHERE id = :id");
    q.bindValue(":suf", suffix);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::updateProgress(const int id, const int newProgress) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET progress = :prog WHERE id = :id");
    q.bindValue(":prog", newProgress);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::updateDownloaded(const int id, const bool downloaded) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET is_download = :d WHERE id = :id");
    q.bindValue(":d", downloaded ? 1 : 0);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::updateIfMetadata(const int id, const bool ifM) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET with_metadata = :wm WHERE id = :id");
    q.bindValue(":wm", ifM ? 1 : 0);
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::removeDownloaded(const int id) const {
    QSqlQuery q(this->db);

    q.prepare("DELETE FROM downloads WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
}

bool DataBaseManager::removeAllDownloads() const {
    QSqlQuery q(this->db);

    q.prepare("DELETE FROM downloads");

    if (!q.exec()) return false;

    return q.numRowsAffected() > 0;
};


// Private

bool DataBaseManager::createTable() const {
    const QString sintaxis = R"(
        CREATE TABLE IF NOT EXISTS downloads (
            id             INTEGER PRIMARY KEY,
            url            TEXT NOT NULL,
            audio_code     INTEGER,
            video_code     INTEGER,
            file_path      TEXT NOT NULL,
            suffix         TEXT NOT NULL,
            old_title      TEXT NOT NULL,
            new_title      TEXT NOT NULL,
            progress       INTEGER DEFAULT 0,
            is_download    INTEGER DEFAULT 0,
            with_metadata  INTEGER DEFAULT 0
        )
    )";

    QSqlQuery q(this->db);

    if (!q.exec(sintaxis)) return false;

    return true;
}
