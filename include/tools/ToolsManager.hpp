#pragma once

#include <core/Constants.hpp>

class ToolsManager final : public QObject {
    Q_OBJECT

public:
    ToolsManager(QObject* parent = nullptr);

    static bool checkExecutable(const QString& filePath);
    static bool checkPathWritable(const QString& filePath);
    static QString findToolsPath(const QString& program);
    static void unzipFiles(const QString& sourceFile, const QString& targetFile);

    void checkYtDlp(const QString& filePath);
    void checkFFmpeg(const QString& filePath);
    void checkNode(const QString& filePath);

    void downloadFile(const QUrl& downloadUrl, const QString& savePath, const QString& program);
    void updateYtDlp(const QString& filePath = TOOLS::ytDlpFilePath());
    void updateYtDlpToNightly(const QString& filePath = TOOLS::ytDlpFilePath());

signals:
    void ytDlpChecked(bool isExecutable, bool isYtDlp, const QString& filePath);
    void ffmpegChecked(bool isExecutable, bool isFFmpeg, const QString& filePath);
    void nodeChecked(bool isExecutable, bool isNode, const QString& filePath);

    void fileDownloaded(bool ok, const QString& program);
    void fileDownloadedProgress(qint64 bytesReceived, qint64 bytesTotal, const QString& program);
    void ytDlpUpdated(bool ok);
    void ytDlpUpdatedToNightly(bool ok);
};
