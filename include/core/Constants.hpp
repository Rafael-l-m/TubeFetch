#pragma once

#include <QDir>
#include <QStandardPaths>
#include <QUrl>

namespace DOWNLOAD {
    inline const int TIME_WAIT_MIN {1000};
    inline const int TIME_WAIT_MAX {4000};
    inline const int RATE_MIN {350};
    inline const int RATE_MAX {600};
}

namespace DOWNLOAD_MANAGER {
    inline const int MAX_CONCURRENT {3};
    inline const int WAIT_TIME_TOO_MANY_REQUESTS {300};
}

namespace JSON_FORMAT {
    inline const QString ID {"id"};
    inline const QString INTERNAL_ID {"internalId"};
    inline const QString URL {"url"};
    inline const QString TITLE {"title"};
    inline const QString AUDIO_CODE {"audioCode"};
    inline const QString VIDEO_CODE {"videoCode"};
    inline const QString NON_DASH_CODE {"nonDashCode"};
    inline const QString BEST_AUDIO {"bestAudio"};
    inline const QString BEST_VIDEO {"bestVideo"};
    inline const QString SAVE_PATH {"savePath"};
    inline const QString SUFFIX {"suffix"};
    inline const QString METADATA {"metadata"};
    inline const QString SUBTITLES {"subtitles"};
    inline const QString PROGRESS {"progress"};
    inline const QString DOWNLOAD_STATUS {"downloadStatus"};
    inline const QString DOWNLOADED_BYTES {"downloadedBytes"};
    inline const QString TOTAL_BYTES {"totalBytes"};
    inline const QString DOWNLOAD_SPEED {"downloadSpeed"};
    inline const QString ETA {"eta"};
    inline const QString TIME_LEFT {"timeLeft"};
    inline const QString WEIGHTS {"weights"};
    inline const QString OPERATION {"operation"};
    inline const QString SPECIFICATION {"specification"};
    inline const QString PARAMETERS {"params"};
    inline const QString MESSAGE {"message"};
    inline const QString SUCCESS {"success"};
    inline const QString ERROR {"error"};
    inline const QString STATUS {"status"};
    inline const QString STATUS_ACCEPTED {"accepted"};
    inline const QString STATUS_REFUSED {"refused"};
}

namespace MAIN_CPP { inline const QString SERVER_NAME {"YouTubeMediaDownloader"}; }

namespace MediaCodecFilters {
    inline const QSet<int> NON_DASH_FORMAT_IDS {5, 6, 13, 17, 18, 22, 34, 35, 36, 37, 38, 43, 44, 45, 46};

    struct ContainerSupport {
        QStringList videoCodecs;
        QStringList audioCodecs;
    };

    inline const QMap<QString, ContainerSupport> containerSupport {
        { "mp4",  { {"h264", "h265", "av1"},   {"aac", "mp3", "alac", "opus"} } },
        { "webm", { {"vp8", "vp9", "av1"},     {"opus", "vorbis"}             } },
        { "mkv",  { {"any"},                   {"any"}                        } },
        { "flv",  { {"h264"},                  {"aac", "mp3"}                 } },
        { "avi",  { {"mpeg4", "h264"},         {"mp3", "pcm"}                 } },
        { "mov",  { {"h264", "h265"},          {"aac", "alac"}                } },
        { "ts",   { {"h264", "h265", "mprg2"}, {"aac", "ac3", "mp3"}          } },
        { "ogg",  { {"theora"},                {"vorbis", "opus"}             } },
        { "3gp",  { {"h263", "mpeg4"},         {"amr"}                        } },
        { "asf",  { {"wmv2", "wmv3"},          {"wma"}                        } },
        { "mpeg", { {"mpeg2"},                 {"mp2"}                        } },
        { "vob",  { {"mpeg2"},                 {"ac3", "mp2"}                 } },
        { "f4v",  { {"h264"},                  {"aac"}                        } },
        { "rm",   { {"rv40"},                  {"cook"}                       } },
        { "divx", { {"mpeg4"},                 {"mp3"}                        } }
    };

    inline bool canMux(const QString& audioCodec, const QString& videoCodec, const QString& container) {
        const auto audioCodecTrimmed = audioCodec.trimmed().toLower();
        const auto videoCodecTrimmed = videoCodec.trimmed().toLower();

        if (audioCodecTrimmed.isEmpty() || videoCodecTrimmed.isEmpty()) { return false; }

        const auto it = containerSupport.constFind(container.trimmed().toLower());

        if (it == containerSupport.cend()) { return false; }

        const auto& rules = *it;

        if (!audioCodecTrimmed.isEmpty() && !rules.audioCodecs.contains("any")) {
            if (!rules.audioCodecs.contains(audioCodecTrimmed)) { return false; }
        }

        if (!videoCodecTrimmed.isEmpty() && !rules.videoCodecs.contains("any")) {
            if (!rules.videoCodecs.contains(videoCodecTrimmed)) { return false; }
        }

        return true;
    }

    inline QString parseAudioCodec(const QString& youtubeCodec) {
        const auto youtubeCodecTrimmed = youtubeCodec.trimmed().toLower();

        if (youtubeCodecTrimmed.isEmpty() || youtubeCodecTrimmed == "none" || youtubeCodecTrimmed.startsWith("unknown", Qt::CaseInsensitive)) return {"unknown"};
        else if (youtubeCodecTrimmed.startsWith("mp4a.40.", Qt::CaseInsensitive)) return "aac";
        else if (youtubeCodecTrimmed.startsWith("opus", Qt::CaseInsensitive)) return "opus";
        else if (youtubeCodecTrimmed.startsWith("vorbis", Qt::CaseInsensitive)) return "vorbis";
        else return youtubeCodecTrimmed;
    };

    inline QString parseVideoCodec(const QString& youtubeCodec) {
        const auto youtubeCodecTrimmed = youtubeCodec.trimmed().toLower();

        if (youtubeCodecTrimmed.isEmpty() || youtubeCodecTrimmed == "none") return {};
        else if (youtubeCodecTrimmed.startsWith("avc1", Qt::CaseInsensitive)) return "h264";
        else if (youtubeCodecTrimmed.startsWith("hev1", Qt::CaseInsensitive) || youtubeCodecTrimmed.startsWith("hvc1", Qt::CaseInsensitive)) return "h265";
        else if (youtubeCodecTrimmed.startsWith("vp9", Qt::CaseInsensitive) || youtubeCodecTrimmed.startsWith("vp09", Qt::CaseInsensitive)) return "vp9";
        else if (youtubeCodecTrimmed.startsWith("av01", Qt::CaseInsensitive)) return "av1";
        else if (youtubeCodecTrimmed.startsWith("vp8", Qt::CaseInsensitive) || youtubeCodecTrimmed.startsWith("vp08", Qt::CaseInsensitive)) return "vp8";
        else return youtubeCodecTrimmed;
    };

    inline const QStringList audioFilters {
        "WAV (*.wav)",
        "MP3 (*.mp3)",
        "AAC (*.aac)",
        "M4A (*.m4a)",
        "Opus (*.opus)",
        "Vorbis (*.ogg)",
        "FLAC (*.flac)",
        "ALAC (*.alac)"
    };

    inline const QStringList videoFilters {
        "MKV (*.mkv)",
        "MP4 (*.mp4)",
        "WEBM (*.webm)",
        "FLV (*.flv)",
        "MOV (*.mov)"
    };
}

namespace SYS_CONFIG {
    inline const QString KEY_PATH_YT_DLP {"tools/yt_dlp_path"};
    inline const QString KEY_PATH_FFMPEG {"tools/ffmpeg_path"};
    inline const QString KEY_PATH_NODE {"tools/node_js_path"};

    namespace DOWNLOAD_SETTINGS {
        inline const QString KEY_MESSAGE_BEFORE_SAVING {"download/add/message_before_saving"};
    }

    namespace GENERAL_SETTINGS {
        inline const QString KEY_LIGHTMODE_AUTOMATIC {"general/lightmode/automatic"};
        inline const QString KEY_LIGHTMODE {"general/lightmode"};
        inline const QString KEY_SELF_CHECK_WHEN_EXECUTE {"general/self_check"};
        inline const QString KEY_USE_RATELIMIT {"general/enable_rate_limit"};
    }

    namespace PAGE_SETTINGS {

    }
}

namespace SYS_PATHS {
    inline const QStringList& SEARCH_PATHS() {
        static const QStringList searchPaths = [] {
            QStringList paths;

            #ifdef Q_OS_MACOS
                    paths << "/opt/homebrew/bin"
                          << "/opt/local/bin"
                          << "/usr/local/bin"
                          << "/usr/bin"
                          << "/bin";
            #elif defined(Q_OS_LINUX)
                    paths << "/usr/bin"
                          << "/usr/local/bin"
                          << "/snap/bin"
                          << "snap/local/bin"
                          << "/bin"
                          << "/home/linuxbrew/.linuxbrew/bin";
            #elif defined(Q_OS_WIN)
                    paths << "C:/Program Files"
                          << "C:/Program Files (x86)"
                          << "C:/ProgramData"
                          << QDir::homePath() + "/scoop/apps";
            #endif

            return paths;
        }();

        return searchPaths;
    }
}

namespace TOOLS {
    inline const QString ytDlpFilePath() {
        const auto appConfigPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        const auto savePath = appConfigPath + QDir::separator() + "yt-dlp";

        #ifdef Q_OS_UNIX
            return savePath;
        #elif defined(Q_OS_WIN)
            return savePath + ".exe";
        #endif
    }

    inline const QString poTokenProviderPath() {
        const auto appConfigPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        const auto savePath = appConfigPath + QDir::separator() + "bgutil-pot-provider";

        #ifdef Q_OS_MACOS
            return savePath;
        #elif defined(Q_OS_WIN)
            return savePath + ".exe";
        #endif
    }

    inline const QString poTokenProviderRsPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "bgutil-ytdlp-pot-provider-rs.zip";
    }

    inline const QString poTokenProviderRsUnzipPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "yt-dlp-plugins" + QDir::separator() + "bgutil-ytdlp-pot-provider";
    }

    inline const QUrl ytDlpDownloadUrl() {
        #ifdef Q_OS_MACOS
            return {"https://github.com/yt-dlp/yt-dlp/releases/download/2026.07.04/yt-dlp_macos"};
        #elif defined(Q_OS_WIN)
            return {"https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe"};
        #elif defined(Q_OS_LINUX)
            return "https://github.com/yt-dlp/yt-dlp/releases/download/2026.07.04/yt-dlp_linux";
        #else
            return "https://github.com/yt-dlp/yt-dlp/releases/download/2026.07.04/yt-dlp";
        #endif
    }

    inline const QUrl poTokenProviderUrl() {
        #ifdef Q_OS_MAC
            #ifdef Q_PROCESSOR_ARM
                return {"https://github.com/jim60105/bgutil-ytdlp-pot-provider-rs/releases/download/v0.8.1/bgutil-pot-macos-aarch64"};
            #elif defined(Q_PROCESSOR_X86)
                return {"https://github.com/jim60105/bgutil-ytdlp-pot-provider-rs/releases/download/v0.8.1/bgutil-pot-macos-x86_64"};
            #endif
        #elif defined(Q_OS_WIN)
            return {"https://github.com/jim60105/bgutil-ytdlp-pot-provider-rs/releases/download/v0.8.1/bgutil-pot-windows-x86_64.exe"};
        #elif defined(Q_OS_LINUX)
            return {"https://github.com/jim60105/bgutil-ytdlp-pot-provider-rs/releases/download/v0.8.1/bgutil-pot-linux-x86_64"};
        #endif
    }

    inline const QUrl poTokenProviderRsUrl{"https://github.com/jim60105/bgutil-ytdlp-pot-provider-rs/releases/download/v0.8.1/bgutil-ytdlp-pot-provider-rs.zip"};
}

namespace UPDATE {
    inline const QUrl URL_REPO {"https://github.com/Rafael-l-m/TubeFetch"};
    inline const QUrl URL_CHECK_UPDATE {"https://api.github.com/repos/Rafael-l-m/TubeFetch/releases/latest"};
    inline const QUrl URL_REPORT_ISSUES {"https://github.com/Rafael-l-m/TubeFetch/issues/new"};
    inline const QString CURRENT_VERSION {"v2.1.0"};
}
