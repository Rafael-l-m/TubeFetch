#include <Backend.hpp>
#include <core/BasicTools.hpp>
#include <core/SingleApplication.hpp>
#include <db/DataBase.hpp>
#include <LanguageManager.hpp>
#include <MessageCenter.hpp>
#include <WindowManager.hpp>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SingleApplication singleApp(MAIN_CPP::SERVER_NAME);

    if (singleApp.isRunning()) { return -1; }

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

    WindowManager wm(&engine);
    engine.rootContext()->setContextProperty("windowManager", &wm);

    Backend backend(&engine);
    engine.rootContext()->setContextProperty("backend", &backend);

    // Load Language
    const auto lang = CONFIG::readConfig<QString>("languages").trimmed();

    if (lang.isEmpty()) {
        CONFIG::writeConfig({ {"languages", "en_US"} });
        lm.setLanguage("en_US");
    }

    else { lm.setLanguage(lang); }

    // Compare Versions
    const auto currentVersion = UPDATE::CURRENT_VERSION.trimmed();
    const auto registeredVersion = CONFIG::readConfig<QString>("versions").trimmed();

    if (UpdateChecker::versionComparator(currentVersion, registeredVersion)) {
        const auto appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        clearDirectory(appDataDir.trimmed());
        CONFIG::writeConfig({ {"versions", currentVersion} });
        backend.writeDefaultSettings();
    }

    engine.loadFromModule("TubeFetch", "Main");

    if (engine.rootObjects().isEmpty()) { return -2; }

    // Single Instance Protection
    const auto _mw = wm.getMainWindow();

    if (!_mw) {
        auto* mw = qobject_cast<QWindow*>(engine.rootObjects().constFirst());
        if (mw) { wm.setMainWindow(mw); }
    }

    QObject::connect(&singleApp, &SingleApplication::activateMainWindowRequest, &wm, &WindowManager::activateMainWindow);

    // Set Download Model
    backend.setDownloadModel(&dm);

    return QGuiApplication::exec();
}
