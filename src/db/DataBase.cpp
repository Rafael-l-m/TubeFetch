#include <core/APP.hpp>
#include <db/DataBase.hpp>
#include <QDir>
#include <QSqlError>
#include <QStandardPaths>


// Public

DataBase::DataBase(QObject* parent) : QObject(parent) {}

void DataBase::initDataBase() { this->init(); }


// Protected

bool DataBase::init() {
    this->dataBaseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    const QDir dir;

    if (!dir.exists(this->dataBaseDir)) { dir.mkpath(this->dataBaseDir); }

    this->dataBaseDir = this->dataBaseDir + QDir::separator() + "YoutubeMediaDownloader.sqlite";

    APP::messageCenter()->sendDebug(QString("Database Path: %1").arg(this->dataBaseDir));

    if (!QFile::exists(this->dataBaseDir)) {
        QFile file(this->dataBaseDir);

        if (!file.open(QIODevice::WriteOnly)) {
            APP::messageCenter()->sendError("Database not exist and can not be created");
            return false;
        }

        file.close();

        QFile::setPermissions(
            this->dataBaseDir,
            QFileDevice::ReadOwner | QFileDevice::WriteOwner
                | QFileDevice::ReadUser | QFileDevice::WriteUser
            );
    }

    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(this->dataBaseDir);

    return this->db.open() && this->createMainTable();
}

bool DataBase::isOpen() const { return this->db.isOpen(); }

bool DataBase::isEmpty() const {
    QSqlQuery q(this->db);

    q.prepare("SELECT 1 FROM downloads LIMIT 1");
    q.exec();

    return !q.next();
}

bool DataBase::ifExists(const qint64 id) const {
    QSqlQuery q(this->db);

    q.prepare("SELECT COUNT(*) FROM downloads WHERE internalId = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    if (!q.next()) {
        APP::messageCenter()->sendError("DataBase::ifExists: query returned no row");

        return false;
    }

    const auto count = q.value(0).toLongLong();

    return count > 0;
}

QString DataBase::getDataBaseDir() const { return this->dataBaseDir; }

bool DataBase::add(const Download& d) const {
    QSqlQuery q(this->db);

    const auto audioCode = d.getAudioCode().trimmed();
    const auto videoCode = d.getVideoCode().trimmed();
    const auto nonDashCode = d.getNonDashCode().trimmed();

    q.prepare("INSERT INTO downloads (internalId, url, title, audioCode, videoCode, nonDashCode, bestAudio, bestVideo, savePath, suffix, metadata, subtitles, progress, downloadStatus, downloadedBytes, totalBytes, downloadSpeed, eta)"
              "VALUES (:idd, :url, :tt, :ac, :vc, :ndc, :ba, :bv, :sp, :suf, :met, :subt, :prog, :dst, :db, :tb, :dsp, :et)");

    q.bindValue(":idd",  d.getInternalId());
    q.bindValue(":url",  d.getUrl().trimmed());
    q.bindValue(":tt",   d.getTitle().trimmed());
    q.bindValue(":ac",   audioCode.isEmpty()   ? QVariant(QMetaType::fromType<QString>(), nullptr) : QVariant(audioCode));
    q.bindValue(":vc",   videoCode.isEmpty()   ? QVariant(QMetaType::fromType<QString>(), nullptr) : QVariant(videoCode));
    q.bindValue(":ndc",  nonDashCode.isEmpty() ? QVariant(QMetaType::fromType<QString>(), nullptr) : QVariant(nonDashCode));
    q.bindValue(":ba",   d.getBestAudio() ? 1 : 0);
    q.bindValue(":bv",   d.getBestVideo() ? 1 : 0);
    q.bindValue(":sp",   d.getSavePath().trimmed());
    q.bindValue(":suf",  d.getSuffix().trimmed());
    q.bindValue(":met",  d.getMetadata() ? 1 : 0);
    q.bindValue(":subt", d.getSubtitles() ? 1 : 0);
    q.bindValue(":prog", d.getProgress());
    q.bindValue(":dst",  downloadStatusToInt(d.getDownloadStatus()));
    q.bindValue(":db",   d.getDownloadedBytes());
    q.bindValue(":tb",   d.getTotalBytes());
    q.bindValue(":dsp",  d.getDownloadSpeed());
    q.bindValue(":et",   d.getEta().trimmed());

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::addYoutubeAction(const YoutubeActions& y) const {
    QSqlQuery q(this->db);

    q.prepare("INSERT INTO youtubeActions (internalId, createdAt, type, weight)"
              "VALUES (:idd, :ca, :tp, :wh)");

    q.bindValue(":idd", y.getInternalId());
    q.bindValue(":ca",  y.getCreatedAt());
    q.bindValue(":tp",  youtubeActionsTypeToInt(y.getType()));
    q.bindValue(":wh",  y.getWeight());

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::remove(const qint64 id) const {
    QSqlQuery q(this->db);

    q.prepare("DELETE FROM downloads WHERE internalId = :id");

    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::removeAll() const {
    QSqlQuery q(this->db);

    q.prepare("DELETE FROM downloads");

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::removeUselessActions(const qint64 timeLeft) const {
    QSqlQuery q(this->db);

    q.prepare("DELETE FROM youtubeActions WHERE createdAt < :ca");

    q.bindValue(":ca", timeLeft);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateUrl(const qint64 id, const QString& newUrl) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET url = :url WHERE internalId = :id");

    q.bindValue(":url", newUrl.trimmed());
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateTitle(const qint64 id, const QString& newTitle) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET title = :tt WHERE internalId = :id");

    q.bindValue(":tt", newTitle.trimmed());
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateAudioCode(const qint64 id, const QString& newAudioCode) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET audioCode = :ac WHERE internalId = :id");

    const auto audioCode = newAudioCode.trimmed();

    q.bindValue(
        ":ac",
        audioCode.isEmpty()
            ? QVariant(QMetaType::fromType<QString>(), nullptr)
            : QVariant(audioCode)
        );

    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateVideoCode(const qint64 id, const QString& newVideoCode) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET videoCode = :vc WHERE internalId = :id");

    const auto videoCode = newVideoCode.trimmed();

    q.bindValue(
        ":vc",
        videoCode.isEmpty()
            ? QVariant(QMetaType::fromType<QString>(), nullptr)
            : QVariant(videoCode)
        );

    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateNonDashCode(const qint64 id, const QString& newNonDashCode) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET nonDashCode = :ndc WHERE internalId = :id");

    const auto nonDashCode = newNonDashCode.trimmed();

    q.bindValue(
        ":ndc",
        nonDashCode.isEmpty()
            ? QVariant(QMetaType::fromType<QString>(), nullptr)
            : QVariant(nonDashCode)
        );

    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateBestAudio(const qint64 id, const bool newBestAudio) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET bestAudio = :ba WHERE internalId = :id");

    q.bindValue(":ba", newBestAudio ? 1 : 0);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateBestVideo(const qint64 id, const bool newBestVideo) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET bestVideo = :bv WHERE internalId = :id");

    q.bindValue(":bv", newBestVideo ? 1 : 0);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateSavePath(const qint64 id, const QString& newSavePath) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET savePath = :sp WHERE internalId = :id");

    q.bindValue(":sp", newSavePath.trimmed());
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateSuffix(const qint64 id, const QString& newSuffix) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET suffix = :suf WHERE internalId = :id");

    q.bindValue(":suf", newSuffix.trimmed());
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateMetadata(const qint64 id, const bool metadata) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET metadata = :met WHERE internalId = :id");

    q.bindValue(":met", metadata ? 1 : 0);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateSubtitles(const qint64 id, const bool subtitles) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET subtitles = :subt WHERE internalId = :id");

    q.bindValue(":subt", subtitles ? 1 : 0);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateProgress(const qint64 id, const double progress) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET progress = :prog WHERE internalId = :id");

    q.bindValue(":prog", progress);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateDownloadStatus(const qint64 id, const DownloadStatus& downloadStatus) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET downloadStatus = :dst WHERE internalId = :id");

    q.bindValue(":dst", downloadStatusToInt(downloadStatus));
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateDownloadedBytes(const qint64 id, const double downloadedBytes) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET downloadedBytes = :db WHERE internalId = :id");

    q.bindValue(":db", downloadedBytes);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateTotalBytes(const qint64 id, const double totalBytes) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET totalBytes = :tb WHERE internalId = :id");

    q.bindValue(":tb", totalBytes);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateDownloadSpeed(const qint64 id, const double downloadSpeed) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET downloadSpeed = :dsp WHERE internalId = :id");

    q.bindValue(":dsp", downloadSpeed);
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

bool DataBase::updateEta(const qint64 id, const QString& eta) const {
    QSqlQuery q(this->db);

    q.prepare("UPDATE downloads SET eta = :et WHERE internalId = :id");

    q.bindValue(":et", eta.trimmed());
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return q.numRowsAffected() > 0;
}

Download DataBase::read(const qint64 id) const {
    QSqlQuery q(this->db);

    q.prepare("SELECT url, title, audioCode, videoCode, nonDashCode, bestAudio, bestVideo, savePath, suffix, metadata, subtitles, progress, downloadStatus, downloadedBytes, totalBytes, downloadSpeed, eta FROM downloads WHERE internalId = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return {};
    }

    if (q.next()) {
        const auto url  = q.value("url").toString().trimmed();
        const auto tt   = q.value("title").toString().trimmed();
        const auto ac   = q.value("audioCode").toString().trimmed();
        const auto vc   = q.value("videoCode").toString().trimmed();
        const auto ndc  = q.value("nonDashCode").toString().trimmed();
        const auto ba   = q.value("bestAudio").toInt() == 1;
        const auto bv   = q.value("bestVideo").toInt() == 1;
        const auto sp   = q.value("savePath").toString().trimmed();
        const auto suf  = q.value("suffix").toString().trimmed();
        const auto met  = q.value("metadata").toInt() == 1;
        const auto subt = q.value("subtitles").toInt() == 1;
        const auto prog = q.value("progress").toDouble();
        const auto dst  = parseDownloadStatus(q.value("downloadStatus").toInt());
        const auto db   = q.value("downloadedBytes").toDouble();
        const auto tb   = q.value("totalBytes").toDouble();
        const auto dsp  = q.value("downloadSpeed").toDouble();
        const auto eta  = q.value("eta").toString().trimmed();

        return { id, url, tt, ac, vc, ndc, ba, bv, sp, suf, met, subt, prog, dst, db, tb, dsp, eta };
    }

    return {};
}

QList<Download> DataBase::readAll() const {
    QList<Download> list;

    QSqlQuery q(this->db);

    q.prepare("SELECT internalId, url, title, audioCode, videoCode, nonDashCode, bestAudio, bestVideo, savePath, suffix, metadata, subtitles, progress, downloadStatus, downloadedBytes, totalBytes, downloadSpeed, eta FROM downloads");

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return {};
    }

    while (q.next()) {
        const qint64 idd = q.value("internalId").toLongLong();
        const auto url   = q.value("url").toString().trimmed();
        const auto tt    = q.value("title").toString().trimmed();
        const auto ac    = q.value("audioCode").toString().trimmed();
        const auto vc    = q.value("videoCode").toString().trimmed();
        const auto ndc   = q.value("nonDashCode").toString().trimmed();
        const auto ba    = q.value("bestAudio").toInt() == 1;
        const auto bv    = q.value("bestVideo").toInt() == 1;
        const auto sp    = q.value("savePath").toString().trimmed();
        const auto suf   = q.value("suffix").toString().trimmed();
        const auto met   = q.value("metadata").toInt() == 1;
        const auto subt  = q.value("subtitles").toInt() == 1;
        const auto prog  = q.value("progress").toDouble();
        const auto dst   = parseDownloadStatus(q.value("downloadStatus").toInt());
        const auto db    = q.value("downloadedBytes").toDouble();
        const auto tb    = q.value("totalBytes").toDouble();
        const auto dsp   = q.value("downloadSpeed").toDouble();
        const auto eta   = q.value("eta").toString().trimmed();

        list.append({ idd, url, tt, ac, vc, ndc, ba, bv, sp, suf, met, subt, prog, dst, db, tb, dsp, eta });
    }

    return list;
}

int DataBase::getWeight(const qint64 time) const {
    QSqlQuery q(this->db);

    q.prepare("SELECT COALESCE(SUM(weight),0) FROM youtubeActions WHERE createdAt >= :ca");

    q.bindValue(":ca", time);

    if (!q.exec()) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return 0;
    }

    if (q.next()) { return q.value(0).toInt(); }

    return 0;
}


// Private

bool DataBase::createMainTable() const {
    QSqlQuery q(this->db);

    constexpr auto createTable1 = R"(
        CREATE TABLE IF NOT EXISTS downloads (
            internalId      INTEGER         PRIMARY KEY,
            url             TEXT            NOT NULL,
            title           TEXT            NOT NULL,
            audioCode       TEXT,
            videoCode       TEXT,
            nonDashCode     TEXT,
            bestAudio       INTEGER         NOT NULL    DEFAULT 0,
            bestVideo       INTEGER         NOT NULL    DEFAULT 0,
            savePath        TEXT    UNIQUE  NOT NULL,
            suffix          TEXT            NOT NULL,
            metadata        INTEGER         NOT NULL    DEFAULT 0,
            subtitles       INTEGER         NOT NULL    DEFAULT 0,
            progress        REAL            NOT NULL    DEFAULT 0.0,
            downloadStatus  INTEGER         NOT NULL    DEFAULT 0,
            downloadedBytes REAL            NOT NULL    DEFAULT 0.0,
            totalBytes      REAL            NOT NULL    DEFAULT 0.0,
            downloadSpeed   REAL            NOT NULL    DEFAULT 0.0,
            eta             TEXT            NOT NULL    DEFAULT "00:00",

            CHECK (internalId > 0),
            CHECK (bestAudio IN (0, 1)),
            CHECK (bestVideo IN (0, 1)),
            CHECK (metadata IN (0, 1)),
            CHECK (subtitles IN (0, 1)),
            CHECK (progress >= 0.0 AND progress <= 100.0),
            CHECK (downloadStatus IN (-1, 0, 1, 2, 3)),
            CHECK (downloadedBytes >= 0.0),
            CHECK (totalBytes >= 0.0),
            CHECK (downloadSpeed >= 0.0),

            CHECK (
                (bestAudio = 1 AND bestVideo = 0
                    AND audioCode IS NULL
                    AND videoCode IS NULL
                    AND nonDashCode IS NULL)

                OR

                (bestAudio = 0 AND bestVideo = 1
                    AND audioCode IS NULL
                    AND videoCode IS NULL
                    AND nonDashCode IS NULL)

                OR

                (bestAudio = 0 AND bestVideo = 0 AND (
                    (audioCode IS NOT NULL AND nonDashCode IS NULL)

                    OR

                    (audioCode IS NOT NULL
                        AND videoCode IS NOT NULL
                        AND nonDashCode IS NULL)

                    OR

                    (audioCode IS NULL
                        AND videoCode IS NULL
                        AND nonDashCode IS NOT NULL)
                ))
            )
        )
    )";

    if (!q.exec(createTable1)) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    constexpr auto createTable2 = R"(
        CREATE TABLE IF NOT EXISTS youtubeActions (
            internalId      INTEGER     NOT NULL,
            createdAt       INTEGER     NOT NULL,
            type            INTEGER     NOT NULL,
            weight          INTEGER     NOT NULL,

            CHECK (type >= 0 AND type <= 2),
            CHECK (weight >= 0 AND weight < 7)
        )
    )";

    if (!q.exec(createTable2)) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    constexpr auto createIndex = R"(CREATE INDEX IF NOT EXISTS idx_youtubeActions_createdAt ON visitCount(createdAt);)";

    if (!q.exec(createIndex)) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return true;
}

/*bool DataBase::createYoutubeActionsTable() const {
    QSqlQuery q(this->db);

    constexpr auto createTable = R"(
        CREATE TABLE IF NOT EXISTS youtubeActions (
            internalId      INTEGER     NOT NULL,
            createdAt       INTEGER     NOT NULL,
            type            INTEGER     NOT NULL,
            weight          INTEGER     NOT NULL,

            CHECK (type >= 0 AND type <= 2),
            CHECK (weight >= 0 AND weight < 7)
        )
    )";

    if (!q.exec(createTable)) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    constexpr auto createIndex = R"(CREATE INDEX IF NOT EXISTS idx_youtubeActions_createdAt ON visitCount(createdAt);)";

    if (!q.exec(createIndex)) {
        APP::messageCenter()->sendError(QString("SQL: %1").arg(q.lastQuery()));
        APP::messageCenter()->sendError(QString("SQL Error: %1").arg(q.lastError().text().trimmed()));

        return false;
    }

    return true;
}*/
