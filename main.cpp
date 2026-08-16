#include "Backend.hpp"
#include "core/BasicTools.hpp"
#include "core/SingleApplication.hpp"
#include "LanguageManager.hpp"
#include "MessageCenter.hpp"
#include "update/UpdateChecker.hpp"
#include "url/URLInfoManager.hpp"
#include "WindowManager.hpp"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SingleApplication singleApp(MAIN_CPP::SERVER_NAME);

    if (singleApp.isRunning()) { return -2; }

    // Do not use native menubar
    // QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    // Load Modules

    MessageCenter mc(&app);
    engine.rootContext()->setContextProperty("messageCenter", &mc);

    DownloadModel dm(&engine);
    engine.rootContext()->setContextProperty("downloadModel", &dm);

    LanguageManager lm(&engine);
    engine.rootContext()->setContextProperty("languageManager", &lm);

    UpdateChecker uc(&engine);
    engine.rootContext()->setContextProperty("updateChecker", &uc);

    URLInfoManager uim(&engine);
    engine.rootContext()->setContextProperty("urlInfoManager", &uim);

    WindowManager wm(&engine);
    engine.rootContext()->setContextProperty("windowManager", &wm);

    Backend backend(&engine);
    engine.rootContext()->setContextProperty("backend", &backend);


    // Load Language

    const auto _language = SYS_INFO::CONFIG::readConfig<QString>("languages").trimmed();

    if (_language.isEmpty()) {
        SYS_INFO::CONFIG::writeConfig( { {"languages", "en_US"} } );
        lm.setLanguage("en_US");
    }

    else { lm.setLanguage(_language); }

    engine.loadFromModule("YoutubeMediaDownloader", "Main");

    if (engine.rootObjects().isEmpty()) { return -3; }


    // Single Instance Protection

    const auto _mw = wm.getMainWindow();

    if (!_mw) {
        auto* mw = qobject_cast<QWindow*>(engine.rootObjects().constFirst());
        if (mw) { wm.setMainWindow(mw); }
    }

    QObject::connect(&singleApp, &SingleApplication::activateMainWindowRequest, &wm, &WindowManager::activateMainWindow);


    // Load Data

    backend.setDownloadModel(&dm);
    backend.loadDatabase();

    return QGuiApplication::exec();
}
