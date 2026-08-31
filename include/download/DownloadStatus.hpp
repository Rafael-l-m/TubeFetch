#pragma once

#include <QObject>

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

inline DownloadStatus parseDownloadStatus(const int value) {
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
        case DownloadStatus::WAITING:     return QObject::tr("Waiting");
        case DownloadStatus::DOWNLOADING: return QObject::tr("Downloading");
        case DownloadStatus::PAUSED:      return QObject::tr("Paused");
        case DownloadStatus::FINISHED:    return QObject::tr("Finished");
        case DownloadStatus::ERROR:       return QObject::tr("Error");
        default:                          return QObject::tr("Waiting");
    }
}

inline DownloadStatus parseDownloadStatus(const QString& value) {
    if (value.compare(QObject::tr("Waiting"),     Qt::CaseInsensitive) == 0) return DownloadStatus::WAITING;
    if (value.compare(QObject::tr("Downloading"), Qt::CaseInsensitive) == 0) return DownloadStatus::DOWNLOADING;
    if (value.compare(QObject::tr("Paused"),      Qt::CaseInsensitive) == 0) return DownloadStatus::PAUSED;
    if (value.compare(QObject::tr("Finished"),    Qt::CaseInsensitive) == 0) return DownloadStatus::FINISHED;
    if (value.compare(QObject::tr("Error"),       Qt::CaseInsensitive) == 0) return DownloadStatus::ERROR;
    return DownloadStatus::WAITING;
}
