#pragma once

#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QStack>

class WindowManager final : public QObject {
    Q_OBJECT

public:
    explicit WindowManager(QQmlApplicationEngine* engine);

    Q_INVOKABLE void setMainWindow(QObject* window);

    Q_INVOKABLE QQuickWindow* getMainWindow();

    Q_INVOKABLE void backToMainWindow();

    Q_INVOKABLE void switchToAnotherWindow(
        const QString& fileName,
        bool appModal = true,
        bool staysOnTop = true,
        bool hideCurrent = true,
        const QVariantMap& params = {}
    );

    Q_INVOKABLE void switchToIndependentWindow(
        const QString& fileName,
        bool appModal = true,
        bool staysOnTop = true,
        const QVariantMap& params = {}
    );

    Q_INVOKABLE void goBack();

    Q_INVOKABLE void clearWindowStack();

public slots:
    void activateMainWindow();

signals:
    void quitApp();

private:
    QQuickWindow* createWindow(
        const QString& fileName,
        bool appModal,
        bool staysOnTop,
        QQuickWindow* transientParent,
        const QVariantMap& params = {}
    );

private:
    QQmlApplicationEngine* m_engine = nullptr;
    QQuickWindow* m_mainWindow = nullptr;
    QStack<QPointer<QQuickWindow>> m_windowStack;
};
