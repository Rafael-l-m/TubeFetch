#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <core/BasicTools.hpp>
#include <core/Constants.hpp>
#include <core/MessageCenter.hpp>
#include <core/SingleAppProtection.hpp>
#include <download/DownloadModel.hpp>
#include <Backend.hpp>
#include <update/UpdateChecker.hpp>
#include <WindowManager.hpp>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Touch Simulation Activated
    qputenv("QT_QPA_TOUCHPOINTS", "1");

    // Single App Protection
    SingleAppProtection singleApp(MAIN_CPP::SERVER_NAME);

    if (singleApp.isRunning()) { return -1; }

    // Do not use native menubar
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    // Load Engine
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



    WindowManager wm(&engine);
    engine.rootContext()->setContextProperty("windowManager", &wm);

    Backend backend(&engine);
    engine.rootContext()->setContextProperty("backend", &backend);

    // Compare Versions: registeredVersion < currentVersion => clear directory (only for less than v3.0.0)
    const auto _currentVersion = "v3.0.0";
    const auto _registeredVersion = CONFIG::readConfig<QString>("versions").trimmed();

    if (UpdateChecker::versionComparator(_currentVersion, _registeredVersion)) {
        const auto appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        clearDirectory(appDataDir.trimmed());

        CONFIG::writeConfig({ {"versions", _currentVersion} });

        // Overwrite Settings
        CONFIG::writeConfig(
            {
                { SYS_CONFIG::GENERAL_SETTINGS::SELF_CHECK_WHEN_EXECUTE, true },
                { SYS_CONFIG::DOWNLOAD_SETTINGS::SHOW_MESSAGE_BEFORE_SAVING, true }
            }
        );
    }

    // Load Language



    // Load Main Module
    engine.loadFromModule("TubeFetch", "Main");

    if (engine.rootObjects().isEmpty()) { return -2; }

    // Single Instance Protection
    const auto _mw = wm.getMainWindow();

    if (!_mw) {
        auto* mw = qobject_cast<QWindow*>(engine.rootObjects().constFirst());
        if (mw) { wm.setMainWindow(mw); }
    }

    QObject::connect(&singleApp, &SingleAppProtection::activateMainWindowRequest, &wm, &WindowManager::activateMainWindow);

    // Set Download Model
    backend.setDownloadModel(&dm);

    return QGuiApplication::exec();
}
