
#ifndef YOUTUBEMEDIADOWNLOADER_MAINWINDOW_H
#define YOUTUBEMEDIADOWNLOADER_MAINWINDOW_H

#include "DownloadManager.h"
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

class MW final : public QMainWindow {
    Q_OBJECT
public:
    explicit MW(const QString& dataBasePath, QString defaultTranslaterKey, QString defaultOrganization, QString defaultAppName, const QString& ico_path = "", QString yt_dlp_path = "", QString ffmpeg_path = "", QString node_path = "", QWidget* parent = nullptr);

signals:
    void restartAppRequest() const;

private slots:
    void changeLanguage(const QString& newLanguage);
    void addTask();
    void removeAllTasks() const;
    void handleEditRequest() const;
    void handleDeleteRequest();
    void handleStartRequest() const;
    void handleStopRequest();
    void toolConfigActionManager();

private:
    const QString windowTitle = tr("YouTube Media Downloader (v1.0.0-beta.1)");

    const QString defaultTranslatorKey;
    const QString defaultOrganization;
    const QString defaultAppName;

    const std::pair<int, int> windowSize {800, 600};
    const int spacingButtonLayout = 100;
    const std::array<int, 4> contentWidgetsButtonLayout {50, 5, 50, 5};

    QMenuBar* bar;
    QMenu* languageMenu;
    QAction* englishAction;
    QAction* chineseAction;
    QAction* spanishAction;
    QMenu* toolMenu;
    QAction* toolConfigAction;

    QWidget* central;
    QVBoxLayout* mainLayout;
    QHBoxLayout* topButtonLayout;
    QPushButton* startBtn;
    QPushButton* stopBtn;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QVBoxLayout* scrollLayout;
    QHBoxLayout* bottomButtonLayout;
    QPushButton* addBtn;
    QPushButton* removeBtn;

    QIcon windowIcon;
    QString yt_dlp_path;
    QString ffmpeg_path;
    QString node_path;

    DownloadManager* dm;

    inline static int id = 0;
    bool m_isRestarting = false;

    void restoreData();

    static void clearLayout(QLayout* layout);

    void closeEvent(QCloseEvent* event) override {
        if (!this->m_isRestarting) {
            if (const auto flag = NotificationWindow::confirmation(tr("Quit App"), tr("Are you sure you want to quit?"), this)) {
                this->dm->stopAll();
                event->accept();
            }

            else event->ignore();
        }

        else event->accept();
    }

    const QString mainStyle = R"(
        QMainWindow {
            background-color: qlineargradient(
                x1: 0, y1: 0, x2: 1, y2: 1,
                stop: 0 #a3d5f7,
                stop: 0.5 #c0e0f0,
                stop: 1 #a3d5f7
            );
        }

        QMenu {
            background-color: #e3f3fc;
            color: #2c3e50;
            border: 1px solid #a3d5f7;
            border-radius: 6px;
        }

        QMenu::item {
            padding: 6px 20px;
            border-radius: 4px;
        }

        QMenu::item:selected {
            background-color: #b5dff0;
            color: #1c2e40;
        }

        QMenuBar {
            background-color: #cde7f5;
            color: #2c3e50;
            border-bottom: 1px solid #a3d5f7;
            font-weight: bold;
            padding: 4px;
        }

        QMenuBar::item {
            background: transparent;
            padding: 6px 12px;
            border-radius: 6px;
        }

        QMenuBar::item:selected {
            background-color: #a3d5f7;
            color: #1c2e40;
        }

        QPushButton {
            color: #2c3e50;
            background-color: qlineargradient(
                x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #d6f0ff,
                stop: 1 #a3d5f7
            );
            border: 1px solid #a3d5f7;
            border-radius: 8px;
            padding: 6px 12px;
            font-size: 12px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: qlineargradient(
                x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #c0e0f0,
                stop: 1 #89c2e0
            );
            border: 1px solid #89c2e0;
            color: #1c2e40;
        }

        QPushButton:pressed {
            background-color: qlineargradient(
                x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #89c2e0,
                stop: 1 #a3d5f7
            );
            border: 1px solid #5ca6c8;
            color: #1c2e40;
        }

        QPushButton:disabled {
            color: #a0b0bb;
            background-color: qlineargradient(
                x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #e8f4fb,
                stop: 1 #cce5f5
            );
            border: 1px solid #cce5f5;
            border-radius: 8px;
            padding: 6px 12px;
            font-size: 12px;
            font-weight: bold;
        }

        QScrollArea {
            background-color: #b8def1;
            border: 1px solid #a3d5f7;
            border-radius: 8px;
        }

        QScrollBar:vertical {
            background: #d6f0ff;
            width: 12px;
            margin: 2px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background: #a3d5f7;
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            background: none;
            height: 0px;
        }

        QScrollArea QWidget {
            background-color: #b8def1;
            border: none;
        }
    )";
};

#endif //YOUTUBEMEDIADOWNLOADER_MAINWINDOW_H
