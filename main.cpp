
#include "src/gui/MainWindow.h"
#include "src/Prepare In Advance.h"


const QString toolsFolder = QDir::currentPath() + "/" + ".tools";
constexpr bool toolsFolderHidden = true;

const QString yt_dlpFolder = toolsFolder + "/" + ".yt_dlp";
constexpr bool yt_dlpFolderHidden = true;

const QString ffmpegFolder = toolsFolder + "/" + ".ffmpeg";
constexpr bool ffmpegFolderHidden = true;

const QString icoPath = QDir::currentPath() + "/.ico/" + "ico.png";


#ifdef Q_OS_WIN
const QString yt_dlpPath = yt_dlpFolder + "/" + "yt-dlp.exe";
const QString ffmpegPath = ffmpegFolder + "/" + "ffmpeg.exe";
const QString ffmpegZipPath = ffmpegFolder + "/" + "f.zip";
const QString ffmpegUnzipPath = ffmpegFolder + "/" + "ffmpeg-8.0-essentials_build";
const QUrl yt_dlpURL("https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe");
const QUrl ffmpegURL("https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip");

#elif defined(Q_OS_LINUX)
const QString yt_dlpPath = yt_dlpFolder + "/" + "yt-dlp";
const QString ffmpegPath = ffmpegFolder + "/" + "ffmpeg";
const QString ffmpegZipPath = ffmpegFolder + "/" + "ffmpeg.tar.xz";
const QString ffmpegUnzipPath = ffmpegFolder + "/" + "ffmpeg-master-latest-linux64-gpl";
const QUrl yt_dlpURL("https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp_linux");
const QUrl ffmpegURL("https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-linux64-gpl.tar.xz");

#else
#error "Unsupported OS"

#endif


void first_time_execution() {
    qDebug() << "Begin first_time_execution";

    const auto flag1 = createFolder(toolsFolder, toolsFolderHidden);
    const auto flag2 = createFolder(yt_dlpFolder, yt_dlpFolderHidden);
    const auto flag3 = createFolder(ffmpegFolder, ffmpegFolderHidden);

    qDebug() << "1";
    if (flag1 && flag2 && flag3) {
        qDebug() << "2";
        if (!ifExistsFile(yt_dlpPath)) {
            qDebug() << "3";
            downloadFile(yt_dlpURL, yt_dlpPath, "Download yt-dlp");

            #ifdef Q_OS_LINUX
            qDebug() << "4";
            runCommand("chmod", {"+x", yt_dlpPath});
            #endif
        }

        qDebug() << "5";
        if (!ifExistsFile(ffmpegPath)) {
            qDebug() << "6";
            downloadFile(ffmpegURL, ffmpegZipPath, "Download ffmpeg");

            #ifdef Q_OS_WIN
            qDebug() << "6.1";
            QStringList args;

            qDebug() << "6.1.1";
            args << "-Command"
                    << QString("Expand-Archive -Path '%1' -DestinationPath '%2' -Force").arg(ffmpegZipPath, ffmpegFolder);

            qDebug() << "6.1.2";
            runCommand("powershell", args);

            qDebug() << "6.1.3";
            moveFolderContents(ffmpegUnzipPath + "/" + "bin", ffmpegFolder);

            qDebug() << "6.1.4";
            args.clear();

            qDebug() << "6.1.5";
            args << "-Command"
                    << QString("Remove-Item -Path '%1', '%2' -Recurse -Force").arg(ffmpegZipPath, ffmpegUnzipPath);

            qDebug() << "6.1.6";
            runCommand("powershell", args);

            #elif defined(Q_OS_LINUX)
            qDebug() << "6.2";
            runCommand("tar", {"-xf", ffmpegTarXzPath, "-C", ffmpegFolder});

            qDebug() << "6.2.1";
            moveFolderContents(ffmpegUnzipPath + "/" + "bin", ffmpegFolder);

            qDebug() << "6.2.2";
            runCommand("rm", {"-rf", ffmpegTarXzPath, ffmpegUnzipPath});

            #endif
        }

        qDebug() << "7";
        runCommand(yt_dlpPath, {"-U"});
    }

    qDebug() << "Finish first_time_execution";
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    if (translator.load("zh_CN.qm", "translations")) QApplication::installTranslator(&translator);
    else qDebug() << "Error load translator";

    first_time_execution();  // Needs to be executed at the beginning

    mw w(nullptr, icoPath, yt_dlpPath, ffmpegPath);
    w.show();

    return QApplication::exec();
}
