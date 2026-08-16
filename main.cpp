
#include "MainWindow.h"
#include "PathManager.h"
#include "ToolsWindow.h"
#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QStandardPaths>
#include <QTranslator>

QLocalServer *server = nullptr;

bool sendMessageToRunningInstance(const QString& serverName, const QString& message) {
    QLocalSocket socket;
    socket.connectToServer(serverName);

    if (!socket.waitForConnected(200)) return false;

    socket.write(message.toUtf8());
    socket.flush();
    socket.waitForBytesWritten(200);
    socket.disconnectFromServer();

    return true;
}

void setupServer(const QString& serverName, MW *m) {
    server = new QLocalServer(m);

    QLocalServer::removeServer(serverName);

    server->listen(serverName);

    QObject::connect(server, &QLocalServer::newConnection, [m]() {
        QLocalSocket *client = server->nextPendingConnection();

        QObject::connect(client, &QLocalSocket::readyRead, [client, m] {
            if (const QByteArray msg = client->readAll(); msg.contains("show")) {
                m->showNormal();
                m->raise();
                m->activateWindow();
            }
        });
    });
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    #ifdef Q_OS_MAC
        const auto DEFAULT_RESOURCES_PATH = QCoreApplication::applicationDirPath() + "/../Resources/icons";
    #elif defined(Q_OS_WIN)
        const auto DEFAULT_RESOURCES_PATH = QCoreApplication::applicationDirPath() + "/resources/icons";
    #else
        const auto DEFAULT_RESOURCES_PATH = QCoreApplication::applicationDirPath() + "/resources/icons";
    #endif

    const auto DEFAULT_ICONS_PATH = DEFAULT_RESOURCES_PATH + "/Video.png";

    const QString DEFAULT_YT_DLP_KEY = "tools/yt_dlp_path";
    const QString DEFAULT_FFMPEG_KEY = "tools/ffmpeg_path";
    const QString DEFAULT_NODE_JS_KEY = "tools/node_js_path";
    const QString DEFAULT_TRANSLATOR_KEY = "translator/default_language";
    const QString DEFAULT_LANGUAGE_FILE = "en_US.qm";
    const QString DEFAULT_ORGANIZATION = "rafael";
    const QString DEFAULT_APP_NAME = "YouTubeMediaDownloaderTranslator";
    const QString DEFAULT_LOCAL_SERVER = "YouTubeMediaDownloaderAppServer";

    if (sendMessageToRunningInstance(DEFAULT_LOCAL_SERVER, "show")) return 0;

    const auto DEFAULT_DATABASE_PATH = PathManager::instance().dataBasePath();

    QString yt_dlp_path = ToolsWindow::readToolPath(DEFAULT_YT_DLP_KEY);
    QString ffmpeg_path = ToolsWindow::readToolPath(DEFAULT_FFMPEG_KEY);
    QString node_js_path = ToolsWindow::readToolPath(DEFAULT_NODE_JS_KEY);
    QString defaultLanguage = ToolsWindow::readToolPath(DEFAULT_TRANSLATOR_KEY, DEFAULT_ORGANIZATION, DEFAULT_APP_NAME);

    if (yt_dlp_path.isEmpty() | ffmpeg_path.isEmpty() | node_js_path.isEmpty()) {
        auto* tw = new ToolsWindow();
        tw->setAttribute(Qt::WA_DeleteOnClose);
        tw->exec();

        yt_dlp_path = ToolsWindow::readToolPath(DEFAULT_YT_DLP_KEY);
        ffmpeg_path = ToolsWindow::readToolPath(DEFAULT_FFMPEG_KEY);
        node_js_path = ToolsWindow::readToolPath(DEFAULT_NODE_JS_KEY);
    }

    if (yt_dlp_path.isEmpty() | ffmpeg_path.isEmpty()) {
        NotificationWindow::critical(QObject::tr("Critical"), QObject::tr("Have to provide at least path of yt-dlp and ffmpeg"));
        return 0;
    }

    if (defaultLanguage.isEmpty()) {
        QMap<QString, QString> tmp;
        tmp.insert(DEFAULT_TRANSLATOR_KEY, DEFAULT_LANGUAGE_FILE);
        (void)ToolsWindow::writeToolPath(tmp, DEFAULT_ORGANIZATION, DEFAULT_APP_NAME);
        defaultLanguage = DEFAULT_LANGUAGE_FILE;
    }

    #if defined(Q_OS_MACOS)
        const QString translationPath = QCoreApplication::applicationDirPath() + "/../Resources/translations";
    #elif defined(Q_OS_WIN)
        const QString translationPath = QCoreApplication::applicationDirPath() + "/translations";
    #else
        const QString translationPath = QCoreApplication::applicationDirPath() + "/translations";
    #endif

    const QStringList searchPaths = {
        translationPath,
        "../translations",
        ":/translations",
    };

    QTranslator translator;

    for (const QString& path : searchPaths) {
        if (translator.load(defaultLanguage, path)) {
            QApplication::installTranslator(&translator);
            break;
        }
    }

    (void)DownloadManager::upgradeYtDlp(yt_dlp_path);

    MW mw{DEFAULT_DATABASE_PATH, DEFAULT_TRANSLATOR_KEY, DEFAULT_ORGANIZATION, DEFAULT_APP_NAME, DEFAULT_ICONS_PATH, yt_dlp_path, ffmpeg_path, node_js_path};

    setupServer(DEFAULT_LOCAL_SERVER, &mw);

    QObject::connect(&mw, &MW::restartAppRequest, [&]() {
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QCoreApplication::arguments());
        QCoreApplication::quit();
    });

    mw.show();

    return QApplication::exec();
}
