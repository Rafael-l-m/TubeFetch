#include <core/APP.hpp>
#include <tools/ToolsManager.hpp>
#include <QDirIterator>
#include <QNetworkReply>
#include <QProcess>
#include <QRegularExpression>
#include <QTemporaryFile>


// Public

ToolsManager::ToolsManager(QObject* parent) : QObject(parent) {}

bool ToolsManager::checkExecutable(const QString& filePath) { return QFileInfo(filePath.trimmed()).isExecutable(); }

bool ToolsManager::checkPathWritable(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    if (filePathTrimmed.isEmpty()) { return false; }

    const auto dir = QFileInfo(filePathTrimmed).dir();

    QTemporaryFile file(dir.filePath("XXXXXX.tmp"));

    file.setAutoRemove(true);

    return file.open();
}

QString ToolsManager::findToolsPath(const QString& program) {
    #ifdef Q_OS_UNIX
        const auto programTrimmed = program.trimmed();
    #elif defined(Q_OS_WIN)
        const auto programTrimmed = program.trimmed() + ".exe";
    #endif

    if (programTrimmed.isEmpty()) { return {}; }

    const auto path = QStandardPaths::findExecutable(programTrimmed);

    const auto pathTrimmed = path.trimmed();

    if (!pathTrimmed.isEmpty()) { return pathTrimmed; }

    const auto searchPaths = SYS_PATHS::SEARCH_PATHS();

    for (const auto& dirPath : searchPaths) {
        QDirIterator it(dirPath, QDir::Files | QDir::Executable, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            it.next();

            QFileInfo fi(it.filePath());

            if (fi.fileName().compare(programTrimmed, Qt::CaseInsensitive) == 0) {
                return fi.absoluteFilePath();
            }
        }
    }

    return {};
}

void ToolsManager::unzipFiles(const QString& sourceFile, const QString& targetFile) {
    const auto sourceFileTrimmed = sourceFile.trimmed();
    const auto targetFileTrimmed = targetFile.trimmed();

    QFileInfo sourceInfo(sourceFileTrimmed);

    if (!sourceInfo.exists()) {
        APP::messageCenter()->sendError(QString("Source zip does not exist: %1").arg(sourceFileTrimmed));
        return;
    }

    if (!sourceInfo.isFile()) {
        APP::messageCenter()->sendError(QString("Source is not a file: %1").arg(sourceFileTrimmed));
        return;
    }

    if (!QDir().mkpath(targetFileTrimmed)) {
        APP::messageCenter()->sendError(QString("Failed to create target directory: %1").arg(targetFileTrimmed));
        return;
    }

    QProcess process;

    #ifdef Q_OS_UNIX
        process.start("unzip", QStringList() << "-o" << sourceFileTrimmed << "-d" << targetFileTrimmed);
    #elif defined(Q_OS_WIN)
        process.start("powershell", QStringList()
            << "-NoProfile"
            << "-Command"
            << QString("Expand-Archive -LiteralPath '%1' -DestinationPath '%2' -Force")
                .arg(sourceFileTrimmed)
                .arg(targetFileTrimmed));
    #endif

    if (!process.waitForStarted()) {
        APP::messageCenter()->sendWarning(QString("Failed to start unzip process: %1").arg(process.errorString()));
        return;
    }

    if (!process.waitForFinished(-1)) {
        APP::messageCenter()->sendError(QString("Unzip process failed: %1").arg(process.errorString()));
        return;
    }

    const auto standardOutput = process.readAllStandardOutput();
    const auto standardError = process.readAllStandardError();

    if (!standardOutput.isEmpty()) {
        APP::messageCenter()->sendOutput(QString::fromLocal8Bit(standardOutput));
    }

    if (!standardError.isEmpty()) {
        APP::messageCenter()->sendError(QString::fromLocal8Bit(standardError));
    }

    if (process.exitStatus() != QProcess::NormalExit) {
        APP::messageCenter()->sendError("Unzip process crashed");
        return;
    }

    if (process.exitCode() != 0) {
        APP::messageCenter()->sendError(QString("Unzip failed, exit code: %1").arg(process.exitCode()));
        return;
    }

    APP::messageCenter()->sendDebug(QString("Successfully extracted %1 to %2").arg(sourceFileTrimmed).arg(targetFileTrimmed));
    APP::messageCenter()->sendDebug("Removing .zip file ...");

    const auto ifRemoved = QFile::remove(sourceFileTrimmed);

    if (ifRemoved) { APP::messageCenter()->sendDebug("Successfully removed .zip file"); }

    else { APP::messageCenter()->sendWarning(QString("Failed to remove .zip file, you may remove it manually: %1").arg(sourceFileTrimmed)); }
}

void ToolsManager::checkYtDlp(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    const QRegularExpression re(R"(^\d{4}\.\d{2}\.\d{2}.*$)", QRegularExpression::CaseInsensitiveOption);

    if (this->checkExecutable(filePathTrimmed)) {
        auto* process = new QProcess(this);

        connect(process, &QProcess::finished, [re, filePathTrimmed, process, this](const int, const QProcess::ExitStatus&){
            const auto output = process->readAllStandardOutput() + process->readAllStandardError();

            APP::messageCenter()->sendOutput(QString("Check yt-dlp: %1").arg(output));

            if (re.match(output.trimmed()).hasMatch()) { emit ytDlpChecked(true, true, filePathTrimmed); }

            else { emit ytDlpChecked(true, false, {}); }

            process->deleteLater();
        });

        process->start(filePathTrimmed, {"--version"});
    }

    else { emit ytDlpChecked(false, false, {}); }
}

void ToolsManager::checkFFmpeg(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    if (this->checkExecutable(filePathTrimmed)) {
        auto* process = new QProcess();

        connect(process, &QProcess::finished, [filePathTrimmed, process, this](const int, const QProcess::ExitStatus&){
            const auto output = process->readAllStandardOutput() + process->readAllStandardError();

            APP::messageCenter()->sendOutput(QString("Check ffmpeg: %1").arg(output));

            if (output.trimmed().contains("ffmpeg")) { emit ffmpegChecked(true, true, filePathTrimmed); }

            else { emit ffmpegChecked(true, false, {}); }

            process->deleteLater();
        });

        process->start(filePathTrimmed, {"-version"});
    }

    else { emit ffmpegChecked(false, false, {}); }
}

void ToolsManager::checkNode(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    const QRegularExpression re(R"(^v\d+\.\d+\.\d+$)", QRegularExpression::CaseInsensitiveOption);

    if (this->checkExecutable(filePathTrimmed)) {
        auto* process = new QProcess();

        connect(process, &QProcess::finished, [re, filePathTrimmed, process, this](const int, const QProcess::ExitStatus&){
            const auto output = process->readAllStandardOutput() + process->readAllStandardError();

            APP::messageCenter()->sendOutput(QString("Check node: %1").arg(output));

            if (re.match(output.trimmed()).hasMatch()) { emit nodeChecked(true, true, filePathTrimmed); }

            else { emit nodeChecked(true, false, {}); }

            process->deleteLater();
        });

        process->start(filePathTrimmed, {"--version"});
    }

    else { emit nodeChecked(false, false, {}); }
}

void ToolsManager::downloadFile(const QUrl& downloadUrl, const QString& savePath, const QString& program) {
    const auto savePathTrimmed = savePath.trimmed();

    if (savePathTrimmed.isEmpty()) {
        APP::messageCenter()->sendError("Download path is empty.");

        emit fileDownloaded(false, program);

        return;
    }

    QFileInfo fi(savePathTrimmed);

    auto parentDir = fi.dir();

    if (!parentDir.exists()) {
        if (!parentDir.mkpath(".")) {
            APP::messageCenter()->sendError(QString("Failed to create directory: %1").arg(parentDir.absolutePath()));

            emit fileDownloaded(false, program);

            return;
        }
    }

    auto* manager = new QNetworkAccessManager(this);
    auto* reply = manager->get(QNetworkRequest(downloadUrl));
    auto* file = new QFile(savePathTrimmed, reply);

    if (!file->open(QIODevice::WriteOnly)) {
        APP::messageCenter()->sendError(QString("Failed to open file: %1").arg(savePathTrimmed));

        emit fileDownloaded(false, program);

        reply->abort();
        reply->deleteLater();
        manager->deleteLater();

        return;
    }

    connect(reply, &QNetworkReply::downloadProgress, this, [program, this](const qint64 bytesReceived, const qint64 bytesTotal){
        if (bytesTotal <= 0) { return; }
        emit fileDownloadedProgress(bytesReceived, bytesTotal, program);
    });

    connect(reply, &QNetworkReply::readyRead, this, [reply, file](){ file->write(reply->readAll()); });

    connect(reply, &QNetworkReply::finished, this, [program, manager, reply, file, savePathTrimmed, this](){
        file->write(reply->readAll());
        file->close();

        if (reply->error() != QNetworkReply::NoError) {
            APP::messageCenter()->sendError(QString("Download failed: %1").arg(reply->errorString()));

            QFile::remove(savePathTrimmed);

            emit fileDownloaded(false, program);

            reply->deleteLater();
            manager->deleteLater();

            return;
        }

        #ifdef Q_OS_UNIX
            QFile::Permissions permissions = QFile::permissions(savePathTrimmed);

            permissions |= QFile::ExeOwner;
            permissions |= QFile::ExeGroup;
            permissions |= QFile::ExeOther;

            if (!QFile::setPermissions(savePathTrimmed, permissions)) {
                APP::messageCenter()->sendWarning(QString("Failed to set executable permission: %1").arg(savePathTrimmed));

                emit fileDownloaded(false, program);

                reply->deleteLater();
                manager->deleteLater();

                return;
            }
        #endif

        APP::messageCenter()->sendDebug(QString("Download completed: %1").arg(savePathTrimmed));

        emit fileDownloaded(true, program);

        reply->deleteLater();
        manager->deleteLater();
    });
}

void ToolsManager::updateYtDlp(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    if (filePathTrimmed.isEmpty()) {
        APP::messageCenter()->sendWarning("yt-dlp path is empty");

        emit ytDlpUpdated(false);

        return;
    }

    if (!this->checkExecutable(filePathTrimmed)) {
        APP::messageCenter()->sendError(QString("File not executable: %1").arg(filePathTrimmed));

        emit ytDlpUpdated(false);

        return;
    }

    auto* process = new QProcess(this);

    connect(process, &QProcess::finished, this, [process, this](const int exitCode, const QProcess::ExitStatus& exitStatus){
        if (exitStatus != QProcess::NormalExit || exitCode != 0) {
            APP::messageCenter()->sendError(QString("yt-dlp update failed: %1").arg(process->readAllStandardError()));

            emit ytDlpUpdated(false);

            process->deleteLater();

            return;
        }

        APP::messageCenter()->sendOutput(process->readAllStandardOutput() + process->readAllStandardError());

        emit ytDlpUpdated(true);

        process->deleteLater();
    });

    process->start(filePathTrimmed, {"-U"});
}

void ToolsManager::updateYtDlpToNightly(const QString& filePath) {
    const auto filePathTrimmed = filePath.trimmed();

    if (filePathTrimmed.isEmpty()) {
        APP::messageCenter()->sendWarning("yt-dlp path is empty");

        emit ytDlpUpdatedToNightly(false);

        return;
    }

    if (!this->checkExecutable(filePathTrimmed)) {
        APP::messageCenter()->sendError(QString("File not executable: %1").arg(filePathTrimmed));

        emit ytDlpUpdatedToNightly(false);

        return;
    }

    auto* process = new QProcess(this);

    connect(process, &QProcess::finished, this, [process, this](const int exitCode, const QProcess::ExitStatus& exitStatus){
        if (exitStatus != QProcess::NormalExit || exitCode != 0) {
            APP::messageCenter()->sendError(QString("yt-dlp nightly update failed: %1").arg(process->readAllStandardError()));

            emit ytDlpUpdatedToNightly(false);

            process->deleteLater();

            return;
        }

        APP::messageCenter()->sendOutput(process->readAllStandardOutput() + process->readAllStandardError());

        emit ytDlpUpdatedToNightly(true);

        process->deleteLater();
    });

    process->start(filePathTrimmed, {"--update-to", "nightly"});
}
