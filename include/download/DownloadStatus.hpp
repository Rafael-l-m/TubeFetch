#pragma once

#include <QString>

enum class DownloadStatus {
    WAITING,
    DOWNLOADING,
    PAUSED,
    FINISHED,
    ERROR
};

inline int downloadStatusToInt(const DownloadStatus& downloadStatus) {
    switch (downloadStatus) {
        case DownloadStatus::WAITING:     return 0;
        case DownloadStatus::DOWNLOADING: return 1;
        case DownloadStatus::PAUSED:      return 2;
        case DownloadStatus::FINISHED:    return 3;
        case DownloadStatus::ERROR:       return -1;
        default:                          return 0;
    }
}

inline DownloadStatus intToDownloadStatus(const int value) {
    switch (value) {
        case -1: return DownloadStatus::ERROR;
        case 0:  return DownloadStatus::WAITING;
        case 1:  return DownloadStatus::DOWNLOADING;
        case 2:  return DownloadStatus::PAUSED;
        case 3:  return DownloadStatus::FINISHED;
        default: return DownloadStatus::WAITING;
    }
}

inline QString downloadStatusToStr(const DownloadStatus& downloadStatus) {
    switch (downloadStatus) {
        case DownloadStatus::WAITING:     return "Waiting";
        case DownloadStatus::DOWNLOADING: return "Downloading";
        case DownloadStatus::PAUSED:      return "Paused";
        case DownloadStatus::FINISHED:    return "Finished";
        case DownloadStatus::ERROR:       return "Error";
        default:                          return "Waiting";
    }
}

inline DownloadStatus strToDownloadStatus(const QString& value) {
    if (value.compare("Waiting",     Qt::CaseInsensitive) == 0) return DownloadStatus::WAITING;
    if (value.compare("Downloading", Qt::CaseInsensitive) == 0) return DownloadStatus::DOWNLOADING;
    if (value.compare("Paused",      Qt::CaseInsensitive) == 0) return DownloadStatus::PAUSED;
    if (value.compare("Finished",    Qt::CaseInsensitive) == 0) return DownloadStatus::FINISHED;
    if (value.compare("Error",       Qt::CaseInsensitive) == 0) return DownloadStatus::ERROR;
    return DownloadStatus::WAITING;
}
