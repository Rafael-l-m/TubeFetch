
#ifndef YOUTUBEMEDIADOWNLOADER_DOWNLOADITEMWIDGET_H
#define YOUTUBEMEDIADOWNLOADER_DOWNLOADITEMWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>

class DownloadItemWidget final : public QWidget {
    Q_OBJECT
public:
    explicit DownloadItemWidget(int id, const QString& title = "", const QString& rename = "", int progressValue = 0, QWidget* parent = nullptr);

    [[nodiscard]] int getId() const;
    [[nodiscard]] int getProgress() const;

    void setFileName(const QString& fn) const;
    void setProgress(int percent) const;

    void enabledW() const;
    void disabledW() const;

signals:
    void requestEdit(DownloadItemWidget* self);
    void requestDelete(DownloadItemWidget* self);

private:
    const int id;
    const std::pair<int, int> labSize {150, 48};
    const std::pair<int, int> scrollBarSize {150, 60};
    const int distWidget = 15;
    const int heightQWidget = 80;

    QGridLayout* grid;
    QLabel* titleLab;
    QScrollArea* titleSc;
    QLabel* filenameLab;
    QScrollArea* filenameSc;
    QProgressBar* progress;
    QPushButton* modifyBtn;
    QPushButton* deleteBtn;

    const QString mainStyle = R"(
        QWidget {
            background-color: #8fc0f1;
            border: 1px solid #ddd;
            padding: 5px;
            border-radius: 10px;
        }

        QLabel {
            background-color: #8fc0f1;
            color: #0e0c0c;
            border: none;
            padding: 4px;
            font-size: 12px;
        }

        QProgressBar {
            border: 1px solid #ccc;
            border-radius: 6px;
            background-color: #eaf6fc;
            text-align: center;
            height: 18px;
            color: #ff6f61;
            font-weight: bold;
        }

        QProgressBar {
            border: none;
            background-color: #e6e6e6;
            border-radius: 10px;
            height: 22px;
            color: #080505;
            font-weight: bold;
            text-align: center;
            padding:0px
        }

        QProgressBar::chunk {
            border-radius: 10px;
            background: qlineargradient(
                x1: 0, y1: 0, x2: 1, y2: 0,
                stop: 0 #4facfe,
                stop: 1 #00f2fe
            );
            background-size: 200% 100%;
        }

        QPushButton {
            background-color: #a7d3f5;
            color: #344643;
            border: 1px solid #7fbce0;
            border-radius: 8px;
            padding: 6px 12px;
            font-size: 12px;
        }

        QPushButton:hover {
            background-color: #c2e4fc;
            border: 1px solid #5aaee0;
        }

        QPushButton:pressed {
            background-color: #8fc0f1;
            border: 1px solid #3c8ed6;
            color: #2c3e50;
        }

        QPushButton:disabled {
            background-color: #dbeefb;
            color: #999;
            border: 1px solid #ccc;
        }
    )";

    const QString scrollBarStyle = R"(
        QScrollBar:horizontal {
            height: 0px;
            background: transparent;
        }

        QScrollBar::handle:horizontal,
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            border: none;
            width: 0px;
            margin: 0;
            padding: 0;
            background: transparent;
        }
    )";
};

#endif //YOUTUBEMEDIADOWNLOADER_DOWNLOADITEMWIDGET_H
