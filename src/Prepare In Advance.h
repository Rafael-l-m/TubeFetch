
#ifndef DOWNLOADHIGHQUALITYAUDIO_PREPARE_IN_ADVANCE_H
#define DOWNLOADHIGHQUALITYAUDIO_PREPARE_IN_ADVANCE_H

#include <QDir>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QProcess>
#include <QString>
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#include <QMessageBox>
#include <QRect>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

const QString styles = R"(
    QProgressBar {
        border: 2px solid #4CAF50;
        border-radius: 10px;
        text-align: center;
        font-size: 18px;
        height: 30px;
        color: black;
        background-color: #EEE;
    }

    QProgressBar::chunk {
        background-color: #4CAF50;
        border-radius: 10px;
        width: 10px;
    }
)";

inline bool createFolder(const QString& folderPath, const bool isHidden) {
    qDebug() << "Begin createFolder";

    if (const QDir dir; !dir.exists(folderPath)) {  // if folder do not exist
        qDebug() << "1";

        if (const auto ok = dir.mkdir(folderPath); !ok) {
            qDebug() << "2";
            QMessageBox::information(nullptr, QObject::tr("Error"), QObject::tr("Failed to create directory: %1").arg(folderPath));
            return false;
        }
    }

    if (isHidden) {  // if folder needs to be hidden
        qDebug() << "3";
        #ifdef Q_OS_WIN  // just for windows
            qDebug() << "4";
            SetFileAttributesW(reinterpret_cast<LPCWSTR>(folderPath.utf16()), FILE_ATTRIBUTE_HIDDEN);  // Set folder hidden
        #endif
    }

    qDebug() << "Finish createFolder";

    return true;
}

inline bool ifExistsFile(const QString& filePath) {
    return QFile::exists(filePath);
}

inline void downloadFile(const QUrl& url, const QString& fileName, const QString& title) {
    qDebug() << "Begin downloadFile";

    QProgressBar progressBar(nullptr);
    progressBar.setMinimum(0);
    progressBar.setMaximum(100);
    progressBar.setValue(0);
    progressBar.setWindowTitle(title);
    progressBar.resize(600, 60);
    progressBar.setStyleSheet(styles);

    qDebug() << "1";
    const auto screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();

    qDebug() << "2";
    const auto centeredPos = std::pair{
        (screenGeometry.width() - progressBar.width()) / 2,
        (screenGeometry.height() - progressBar.height()) / 2
    };

    qDebug() << "3";
    progressBar.move(std::get<0>(centeredPos), std::get<1>(centeredPos));

    qDebug() << "4";
    progressBar.show();

    qDebug() << "5";
    QNetworkAccessManager manager;

    qDebug() << "6";
    const QNetworkRequest request(url);

    qDebug() << "7";
    QNetworkReply* reply = manager.get(request);

    qDebug() << "8";
    QObject::connect(reply, &QNetworkReply::downloadProgress, [progressBarPtr = &progressBar](const qint64 received, const qint64 total) {
        if (total > 0) {
            const int percent = static_cast<int>((received * 100) / total);
            progressBarPtr->setValue(percent);
        }
    });

    qDebug() << "9";
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "10";
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "11";

        if (QFile file(fileName); file.open(QIODevice::WriteOnly)) {
            qDebug() << "12";
            file.write(reply->readAll());
            file.close();
            progressBar.setValue(100);
        }

        else {
            qDebug() << "-2";
            QMessageBox::warning(nullptr, QObject::tr("Save Failed"), QObject::tr("Download but failed to save"));
        }
    }

    else {
        qDebug() << "-1";
        QMessageBox::critical(nullptr, QObject::tr("Download Failed"), reply->errorString());
    }

    reply->deleteLater();

    qDebug() << "Finish downloadFile";
}

inline void runCommand(const QString& command, const QStringList& args) {
    qDebug() << "Begin runCommand";

    QProcess process;

    qDebug() << "1";
    process.start(command, args);

    if (!process.waitForFinished()) {
        qDebug() << "-1";
        qWarning() << "Failed to run command: " << command << "\t " << process.errorString();
        return;
    }

    qDebug() << "2";
    const QString stdoutOutput = process.readAllStandardOutput();
    const QString stderrOutput = process.readAllStandardError();

    if (!stdoutOutput.isEmpty()) {
        qDebug() << "3";
        QMessageBox::information(nullptr, QObject::tr("Run Command"), stdoutOutput);
        qDebug() << "stdout: " << stdoutOutput;
    }

    if (!stderrOutput.isEmpty()) {
        qDebug() << "-2";
        QMessageBox::critical(nullptr, QObject::tr("Failed"), stderrOutput);
        qDebug() << "stderr: " << stderrOutput;
    }

    qDebug() << "Finish runCommand";
}

inline void moveFile(const QString& sourcePath, const QString& targetPath) {
    qDebug() << "Begin moveFile";

    if (QFile::copy(sourcePath, targetPath)) {
        qDebug() << "1";
        QFile::remove(sourcePath);
    }

    else {
        qDebug() << "-1";
        QMessageBox::critical(nullptr, QObject::tr("Move Failed"), QObject::tr("Failed to move: %1").arg(sourcePath));
    }

    qDebug() << "Finish moveFile";
}

inline void moveFolderContents(const QString& sourceDirPath, const QString& targetDirPath) {
    qDebug() << "Begin moveFolderContents";

    const QDir sourceDir(sourceDirPath);
    const QDir targetDir(targetDirPath);

    if (!sourceDir.exists()) {
        qDebug() << "-1";
        QMessageBox::critical(nullptr, QObject::tr("Source Missing"), QObject::tr("Source folder does not exist"));

        qDebug() << "Finish moveFolderContents";
        return;
    }

    if (!targetDir.exists()) {
        qDebug() << "-2";

        if (!QDir().mkpath(targetDirPath)) {
            qDebug() << "-3";
            QMessageBox::critical(nullptr, QObject::tr("Target Creation Failed"), QObject::tr("Failed to create target folder"));

            qDebug() << "Finish moveFolderContents";
            return;
        }
    }

    qDebug() << "1";
    for (const auto fileList = sourceDir.entryList(QDir::Files); const auto& fileName : fileList) {
        qDebug() << "1.1";
        const auto sourceFilePath = sourceDir.absoluteFilePath(fileName);

        if (const auto targetFilePath = targetDir.absoluteFilePath(fileName); QFile::copy(sourceFilePath, targetFilePath)) {
            qDebug() << "1.2";
            QFile::remove(sourceFilePath);
        }

        else {
            qDebug() << "-1.3";
            QMessageBox::warning(nullptr, QObject::tr("Move Failed"), QObject::tr("Failed to move: %1").arg(fileName));
        }
    }

    qDebug() << "Finish moveFolderContents";
}

#endif //DOWNLOADHIGHQUALITYAUDIO_PREPARE_IN_ADVANCE_H
