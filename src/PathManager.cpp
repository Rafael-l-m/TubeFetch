
#include "PathManager.h"
#include <QDir>
#include <QStandardPaths>


// Public

PathManager &PathManager::instance() {
    static PathManager inst;
    return inst;
}

QString PathManager::dataBasePath() const {return this->dataBaseDir;}


// Private

PathManager::PathManager() {
    this->dataBaseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (const QDir dir; !dir.exists(this->dataBaseDir)) {
        dir.mkpath(this->dataBaseDir);
    }

    this->dataBaseDir = this->dataBaseDir + "/" + this->dataBaseName;

    if (!QFile::exists(this->dataBaseDir)) {
        QFile file(this->dataBaseDir);

        if (!file.open(QIODevice::WriteOnly)) return;

        file.close();

        QFile::setPermissions(this->dataBaseDir,
            QFileDevice::ReadOwner | QFileDevice::WriteOwner |
            QFileDevice::ReadUser | QFileDevice::WriteUser);
    }
}
