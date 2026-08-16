#pragma once

#include <QString>

enum class Specification {
    Url,
    Title,
    AudioCode,
    VideoCode,
    NonDashCode,
    BestAudio,
    BestVideo,
    SavePath,
    Suffix,
    Metadata,
    Subtitles,
    Progress,
    DownloadState,
    DownloadedBytes,
    TotalBytes,
    DownloadSpeed,
    Eta,
    Unknown
};

inline Specification parseSpecification(const QString& spec) {
    if (spec.compare("Url",             Qt::CaseInsensitive) == 0) return Specification::Url;
    if (spec.compare("Title",           Qt::CaseInsensitive) == 0) return Specification::Title;
    if (spec.compare("AudioCode",       Qt::CaseInsensitive) == 0) return Specification::AudioCode;
    if (spec.compare("VideoCode",       Qt::CaseInsensitive) == 0) return Specification::VideoCode;
    if (spec.compare("NonDashCode",     Qt::CaseInsensitive) == 0) return Specification::NonDashCode;
    if (spec.compare("BestAudio",       Qt::CaseInsensitive) == 0) return Specification::BestAudio;
    if (spec.compare("BestVideo",       Qt::CaseInsensitive) == 0) return Specification::BestVideo;
    if (spec.compare("SavePath",        Qt::CaseInsensitive) == 0) return Specification::SavePath;
    if (spec.compare("Suffix",          Qt::CaseInsensitive) == 0) return Specification::Suffix;
    if (spec.compare("Metadata",        Qt::CaseInsensitive) == 0) return Specification::Metadata;
    if (spec.compare("Subtitles",       Qt::CaseInsensitive) == 0) return Specification::Subtitles;
    if (spec.compare("Progress",        Qt::CaseInsensitive) == 0) return Specification::Progress;
    if (spec.compare("DownloadState",   Qt::CaseInsensitive) == 0) return Specification::DownloadState;
    if (spec.compare("DownloadedBytes", Qt::CaseInsensitive) == 0) return Specification::DownloadedBytes;
    if (spec.compare("TotalBytes",      Qt::CaseInsensitive) == 0) return Specification::TotalBytes;
    if (spec.compare("DownloadSpeed",   Qt::CaseInsensitive) == 0) return Specification::DownloadSpeed;
    if (spec.compare("Eta",             Qt::CaseInsensitive) == 0) return Specification::Eta;
    return Specification::Unknown;
}

inline QString specToString(const Specification& spec) {
    switch (spec) {
        case Specification::Url:             return "Url";
        case Specification::Title:           return "Title";
        case Specification::AudioCode:       return "AudioCode";
        case Specification::VideoCode:       return "VideoCode";
        case Specification::NonDashCode:     return "NonDashCode";
        case Specification::BestAudio:       return "BestAudio";
        case Specification::BestVideo:       return "BestVideo";
        case Specification::SavePath:        return "SavePath";
        case Specification::Suffix:          return "Suffix";
        case Specification::Metadata:        return "Metadata";
        case Specification::Subtitles:       return "Subtitles";
        case Specification::Progress:        return "Progress";
        case Specification::DownloadState:   return "DownloadState";
        case Specification::DownloadedBytes: return "DownloadedBytes";
        case Specification::TotalBytes:      return "TotalBytes";
        case Specification::DownloadSpeed:   return "DownloadSpeed";
        case Specification::Eta:             return "Eta";
        default:                             return "Unknown";
    }
}
