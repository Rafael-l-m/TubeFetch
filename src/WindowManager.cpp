#include <core/APP.hpp>
#include <WindowManager.hpp>
#include <QQmlComponent>


// Public

WindowManager::WindowManager(QQmlApplicationEngine* engine) : m_engine(engine) {}

void WindowManager::setMainWindow(QObject* window) {
    auto* mainWindow = qobject_cast<QQuickWindow*>(window);

    if (!mainWindow) {
        APP::messageCenter()->sendWarning("SetMainWindow: QObject is not a QQuickWindow");
        return;
    }

    this->m_mainWindow = mainWindow;

    if (this->m_windowStack.isEmpty()) { this->m_windowStack.push(this->m_mainWindow); }

    else { this->m_windowStack[0] = this->m_mainWindow; }

    APP::messageCenter()->sendDebug(QString("Stack size: %1").arg(this->m_windowStack.size()));
}

QQuickWindow* WindowManager::getMainWindow() { return this->m_mainWindow; }

void WindowManager::backToMainWindow() {
    while (this->m_windowStack.size() > 1) {
        auto current = this->m_windowStack.pop();

        if (current) {
            current->close();
            current->deleteLater();
        }
    }

    if (!this->m_windowStack.isEmpty()) {
        auto mainWindow = this->m_windowStack.top();

        if (mainWindow) {
            mainWindow->show();
            mainWindow->raise();
            mainWindow->requestActivate();
        }
    }
}

void WindowManager::switchToAnotherWindow(
    const QString& fileName,
    const bool appModal,
    const bool staysOnTop,
    const bool hideCurrent,
    const QVariantMap& params)
{
    if (!this->m_engine) { return; }

    if (this->m_windowStack.isEmpty()) {
        if (this->m_mainWindow) { this->m_windowStack.push(this->m_mainWindow); }
        else { return; }
    }

    QQuickWindow* currentWindow = this->m_windowStack.top();

    // ----------------------------------------
    // Create new window
    // ----------------------------------------

    auto* newWindow = this->createWindow(fileName, appModal, staysOnTop, currentWindow, params);

    if (!newWindow) { return; }

    // ----------------------------------------
    // Hide current
    // ----------------------------------------

    if (hideCurrent && currentWindow) { currentWindow->hide(); }

    // ----------------------------------------
    // Push
    // ----------------------------------------

    this->m_windowStack.push(newWindow);

    // ----------------------------------------
    // Destroy handling
    // ----------------------------------------

    connect(newWindow, &QObject::destroyed, this, [this, newWindow](){
        if (newWindow) {
            APP::messageCenter()->sendDebug(
                QString("Window destroyed: %1")
                    .arg(reinterpret_cast<quintptr>(newWindow),
                         QT_POINTER_SIZE * 2,
                         16,
                         QChar('0')
                         )
                );
        }
    });

    // ----------------------------------------
    // Show
    // ----------------------------------------

    newWindow->show();
    newWindow->raise();
    newWindow->requestActivate();

    APP::messageCenter()->sendDebug(QString("Switch window: %1").arg(fileName));
    APP::messageCenter()->sendDebug(QString("Stack size: %1").arg(this->m_windowStack.size()));
}

void WindowManager::switchToIndependentWindow(
    const QString& fileName,
    const bool appModal,
    const bool staysOnTop,
    const QVariantMap& params)
{
    auto* window = this->createWindow(fileName, appModal, staysOnTop, this->m_mainWindow, params);

    if (!window) { return; }

    connect(window, &QQuickWindow::closing, window, [this, window](QQuickCloseEvent*){
        if (window) {
            APP::messageCenter()->sendDebug(
                QString("Independent Window destroyed: %1")
                    .arg(reinterpret_cast<quintptr>(window),
                         QT_POINTER_SIZE * 2,
                         16,
                         QChar('0')
                         )
                );

            window->deleteLater();
        }
    });

    window->show();
    window->raise();
    window->requestActivate();
}

void WindowManager::goBack() {
    if (this->m_windowStack.size() <= 1) { return; }

    auto current = this->m_windowStack.pop();
    auto previous = this->m_windowStack.top();

    APP::messageCenter()->sendDebug(
        QString("Go back: current = %1; previous = %2; stack size = %3")
            .arg(reinterpret_cast<quintptr>(current.data()),
                 QT_POINTER_SIZE * 2,
                 16,
                 QChar('0'))
            .arg(reinterpret_cast<quintptr>(previous.data()),
                 QT_POINTER_SIZE * 2,
                 16,
                 QChar('0'))
            .arg(this->m_windowStack.size()
        )
    );

    // ----------------------------------------
    // Close current
    // ----------------------------------------

    if (current) { current->close(); current->deleteLater(); }

    // ----------------------------------------
    // Show previous
    // ----------------------------------------

    if (previous) {
        previous->show();
        previous->raise();
        previous->requestActivate();
    }

    else { this->goBack(); }
}

void WindowManager::clearWindowStack() {
    while (this->m_windowStack.size() > 1) {
        auto window = this->m_windowStack.pop();

        if (window) {
            window->close();
            window->deleteLater();
        }
    }

    auto mainWindow = this->m_windowStack.top();

    if (!this->m_windowStack.isEmpty() && mainWindow) {
        mainWindow->show();
        mainWindow->raise();
        mainWindow->requestActivate();
    }
}


// Public slots

void WindowManager::activateMainWindow() {
    if (!this->m_mainWindow) { return; }

    this->m_mainWindow->show();
    this->m_mainWindow->raise();
    this->m_mainWindow->requestActivate();
}


// Private

QQuickWindow* WindowManager::createWindow(
    const QString& fileName,
    const bool appModal,
    const bool staysOnTop,
    QQuickWindow* transientParent,
    const QVariantMap& params)
{
    const QUrl url(QStringLiteral("qrc:/qt/qml/TubeFetch/qml/window/%1").arg(fileName));

    QQmlComponent component(this->m_engine, url);

    if (component.status() == QQmlComponent::Error) {
        APP::messageCenter()->sendError(QString("Failed to load window: %1").arg(fileName));
        APP::messageCenter()->sendError(QString("Error: %1").arg(component.errorString()));
        return nullptr;
    }

    auto* obj = component.createWithInitialProperties(params);

    if (!obj) {
        APP::messageCenter()->sendError(QString("Failed to create window: %1").arg(fileName));
        APP::messageCenter()->sendError(QString("Error: %1").arg(component.errorString()));
        return nullptr;
    }

    auto* window = qobject_cast<QQuickWindow*>(obj);

    if (!window) {
        APP::messageCenter()->sendError(
            QString("Target object is not QQuickWindow: 0x%1")
                .arg(reinterpret_cast<quintptr>(window),
                     QT_POINTER_SIZE * 2,
                     16,
                     QChar('0')
            )
        );

        obj->deleteLater();
        return nullptr;
    }

    // ----------------------------------------
    // Window flags
    // ----------------------------------------

    #ifdef Q_OS_WIN
        Q_UNUSED(staysOnTop)
    #else
        if (staysOnTop) { window->setFlags(window->flags() | Qt::WindowStaysOnTopHint); }
    #endif

    // ----------------------------------------
    // Modal
    // ----------------------------------------

    if (appModal) {
        if (transientParent) { window->setTransientParent(transientParent); }

        window->setModality(Qt::ApplicationModal);
    }

    else { window->setModality(Qt::NonModal); }

    return window;
}
