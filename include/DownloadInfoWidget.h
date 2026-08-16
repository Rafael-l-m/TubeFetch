
#ifndef YOUTUBEMEDIADOWNLOADER_DOWNLOADINFOWIDGET_H
#define YOUTUBEMEDIADOWNLOADER_DOWNLOADINFOWIDGET_H

#include "DataBaseManager.h"
#include "json.hpp"
#include "NotificationWindow.h"
#include "ShowCodes.h"
#include <QComboBox>
#include <QProcess>

class DownloadInfoWidget final : public QDialog {
    Q_OBJECT
public:
    explicit DownloadInfoWidget(QString yt_dlpPath, QString ffmpegPath, QString nodePath, QIcon windowIcon = QIcon(""), QWidget *parent = nullptr);

    [[nodiscard]] DownloadItem* getURLInfo() const;

    void requestEdit(const QString& url, const QString& oldTitle, const QString& newTitle, const std::pair<int, int> &codes, bool with_metadata, const QString& fp, const QString& suf);

    [[nodiscard]] QSet<QString> getSupportedAudioTypes() const;

    [[nodiscard]] QSet<QString> getSupportedVideoTypes() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void Combo1Changed();
    void Combo2Changed();
    void checkCodes();
    void handleDebounceTimer1(const QString& text);
    void handleDebounceTimer2(const QString& text);
    void confirmButton();

private:
    void selectedPath();
    static void sleepForMilliseconds(int msecs);

    const QString windowTitle = tr("Download Info");

    const int waitIntervalForURLInputs = 1000, waitIntervalForCodesInput = 1500;  // Time interval in milliseconds
    const int sleepTime = 1000;  // Sleep time
    const std::pair<int, int> labelSize {65, 25};
    const std::pair<int, int> lESize {600, 40};
    const std::pair<int, int> comboSize {170, 25};
    const std::pair<int, int> windowSize {750, 550};

    QGridLayout* grid;
    QLabel* urlLabel;
    QTimer* debounceTimer1;
    QLineEdit* urlInput;
    QLabel* titleLabel;
    QLineEdit* titleLE;
    QLabel* typeLabel;
    QHBoxLayout* row3;
    QComboBox* combo1;
    QPushButton* checkCodesBtn;
    QProcess* process;
    ShowCodes* showCodes;
    QLabel* codeLabel;
    QTimer* debounceTimer2;
    QLineEdit* inputCodeLE;
    QLabel* MDLabel;
    QComboBox* combo2;
    QLabel* pathLabel;
    QLineEdit* pathLE;
    QHBoxLayout* buttonLayout;
    QPushButton* confirmBtn;

    QIcon windowIcon;
    QString yt_dlp_path;
    QString ffmpeg_path;
    QString node_path;

    QString URL;
    QString originalTitle;
    QString newTitle;
    int audioCode = 0, videoCode = 0;
    bool withMetadata = false;
    QString savePath;
    QString suffix;

    DownloadItem* downloadItem;
    bool normalMode = true;

    const QSet<int> mediaCodes {
        18, 22, 91, 92, 93, 94, 95, 96, 133, 134, 135, 136, 137, 139, 140, 141, 160,
        242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260,
        261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280,
        281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300,
        301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315,
        394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405
    };

    const QString filterAudio =
        "WAV (*.wav);;"
        "MP3 (*.mp3);;"
        "FLAC (*.flac);;"
        "M4A (*.m4a);;"
        "AAC (*.aac);;"
        "Opus (*.opus);;"
        "Vorbis (*ogg);;"
        "ALAC (*.alac);;"
        "All Files (*)";

    const QString filterVideo =
        "MKV (*.mkv);;"
        "MP4 (*.mp4);;"
        "Webm (*.webm);;"
        "FLV (*.flv);;"
        "Ogg (.ogg);;"
        "All Files (*);;";

    const QSet<QString> supportedAudioTypes = {"wav", "mp3", "flac", "m4a", "aac", "opus", "ogg", "alac"};

    const QSet<QString> supportedVideoTypes = {"mkv", "mp4", "webm", "flv", "ogg"};

    const QString mainStyle = R"(
        QDialog {
            background-color: qlineargradient(
                x1: 0, y1: 0, x2: 1, y2: 1,
                stop: 0 #a3d5f7,
                stop: 0.5 #c0e0f0,
                stop: 1 #a3d5f7
            );
            font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif;
            font-size: 12px;
        }

        QLabel {
            color: #000000;
            font-weight: bold;
            font-size: 12px;
        }

        QLineEdit {
            background-color: #e5f4f6;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 5px;
            padding: 6px;
            font-size: 12px;
        }

        QLineEdit:disabled {
            background-color: #686868;
            color: #F2F2F2;
        }

        QComboBox {
            border: 1px solid #a3d5f7;
            border-radius: 6px;
            padding: 4px 8px;
            background-color: #eaf6fc;
            color: #2c3e50;
            font-weight: bold;
        }

        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 20px;
            border-left: 1px solid #a3d5f7;
            background-color: #d6f0ff;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }

        QComboBox QAbstractItemView {
            background-color: #f0f8fc;
            border: 1px solid #a3d5f7;
            selection-background-color: #b5dff0;
            selection-color: #1c2e40;
            padding: 4px;
            outline: 0px;
        }

        QComboBox:disabled {
            color: #F2F2F2;
            background-color: #686868;
        }

        QComboBox:hover {
            background-color: #d6f0ff;
            border: 1px solid #89c2e0;
        }

        QComboBox:pressed {
            background-color: #b0d5eb;
            border: 1px solid #5ca6c8;
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

        QPushButton::disabled {
            color: #F2F2F2;
            background-color: #686868;
            border: 1px solid #cccccc;
        }

        QListView, QTreeView {
            background-color: #dff0fa;
            color: #2c3e50;
            border: 1px solid #a3d5f7;
            border-radius: 6px;
        }

        QListView::item:selected, QTreeView::item:selected {
            background-color: #a3d5f7;
            color: #1c2e40;
            border-radius: 4px;
        }

        QListView::item:hover, QTreeView::item:hover {
            background-color: #c0e0f0;
            border-radius: 4px;
        }

        QHeaderView::section {
            background-color: #cde7f5;
            color: #2c3e50;
            border: 1px solid #a3d5f7;
            padding: 4px;
            font-weight: bold;
        }

        QToolButton {
            background-color: transparent;
            color: #2c3e50;
            border: none;
            border-radius: 5px;
            padding: 4px;
        }

        QToolButton:hover {
            background-color: #c0e0f0;
            border: 1px solid #a3d5f7;
        }

        QToolButton:pressed {
            background-color: #a3d5f7;
        }

        QListWidget {
            background-color: #cde7f5;
            color: #2c3e50;
            border: none;
            border-right: 1px solid #a3d5f7;
        }

        QListWidget::item:selected {
            background-color: #a3d5f7;
            color: #1c2e40;
            border-radius: 4px;
        }

        QListWidget::item:hover {
            background-color: #c0e0f0;
            border-radius: 4px;
        }

        QScrollBar:vertical, QScrollBar:horizontal {
            background: #d6f0ff;
            width: 8px;
            height: 8px;
            border-radius: 4px;
            margin: 0px;
        }

        QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
            background: #a3d5f7;
            border-radius: 4px;
            min-height: 20px;
            min-width: 20px;
        }

        QScrollBar::handle:hover {
            background: #89c2e0;
        }

        QScrollBar::add-line, QScrollBar::sub-line {
            height: 0px;
            width: 0px;
        }
    )";
};

#endif //YOUTUBEMEDIADOWNLOADER_DOWNLOADINFOWIDGET_H
