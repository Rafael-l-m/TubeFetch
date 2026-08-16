#pragma once

#include "db/DataBase.hpp"

class DbManager final : public DataBase {
public:
    explicit DbManager(QObject *parent = nullptr);

    QJsonObject isOpenDB() const;

    QJsonObject addDownload(const QJsonObject& obj) const;

    QJsonObject removeDownload(const QJsonObject& obj) const;

    QJsonObject removeAllDownloads() const;

    QJsonObject updateUrl(const QJsonObject& obj) const;

    QJsonObject updateTitle(const QJsonObject& obj) const;

    QJsonObject updateAudioCode(const QJsonObject& obj) const;

    QJsonObject updateVideoCode(const QJsonObject& obj) const;

    QJsonObject updateNonDashCode(const QJsonObject& obj) const;

    QJsonObject updateBestAudio(const QJsonObject& obj) const;

    QJsonObject updateBestVideo(const QJsonObject& obj) const;

    QJsonObject updateSavePath(const QJsonObject& obj) const;

    QJsonObject updateSuffix(const QJsonObject& obj) const;

    QJsonObject updateMetadata(const QJsonObject& obj) const;

    QJsonObject updateSubtitles(const QJsonObject& obj) const;

    QJsonObject updateProgress(const QJsonObject& obj) const;

    QJsonObject updateDownloadStatus(const QJsonObject& obj) const;

    QJsonObject updateDownloadedBytes(const QJsonObject& obj) const;

    QJsonObject updateTotalBytes(const QJsonObject& obj) const;

    QJsonObject updateDownloadSpeed(const QJsonObject& obj) const;

    QJsonObject updateEta(const QJsonObject& obj) const;

    QJsonObject readDownload(const QJsonObject& obj) const;

    QJsonObject readAllDownloads() const;
};
