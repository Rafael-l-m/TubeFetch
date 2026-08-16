
#ifndef YOUTUBEMEDIADOWNLOADER_NOTIFICATIONWINDOW_H
#define YOUTUBEMEDIADOWNLOADER_NOTIFICATIONWINDOW_H

#include <QCoreApplication>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

class NotificationWindow final : public QDialog {
    Q_OBJECT
public:
    static bool confirmation(const QString& title, const QString& inform, QWidget* parent = nullptr);

    static void information(const QString& title, const QString& inform, QWidget* parent = nullptr);

    static void warning(const QString& title, const QString& inform, QWidget* parent = nullptr);

    static void critical(const QString& title, const QString& inform, QWidget* parent = nullptr);

private:
    enum class NotificationType {
        CONFIRMATION,
        INFORMATION,
        WARNING,
        CRITICAL
    };

    NotificationWindow(QString title, QString info, NotificationType type, const QString& windowIconPath = "", const QString& showIconPath = "", QWidget *parent = nullptr);

    const QString title;
    const QIcon windowIcon;
    QIcon showIcon;
    const QString info;
    const NotificationType type;

    QVBoxLayout* mainLayout;
    QHBoxLayout* infoLayout;
    QLabel* iconLabel;
    QLabel* textLabel;
    QHBoxLayout* buttonLayout;
    QPushButton* acceptBtn;
    QPushButton* rejectBtn;

    const int showIconWidth = 48, showIconHeight = 48;
    const int spacerWidth = 40, spacerHeight = 20;

    const QString style = R"(
        QDialog {
            background-color: #ffffff;
        }

        QLabel {
            font-size: 12px;
        }

        QPushButton {
            font-size: 11px;
            color: #333333;
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                              stop:0 #ffffff,
                                              stop:1 #dcdcdc);
            border: 1px solid #b0b0b0;
            border-radius: 4px;
            padding: 4px 8px;
            min-width: 60px;
            min-height: 24px;
        }

        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                              stop:0 #f0f0f0,
                                              stop:1 #c0c0c0);
            border: 1px solid #999999;
        }

        QPushButton:pressed {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                              stop:0 #e0e0e0,
                                              stop:1 #a0a0a0);
            border: 1px solid #888888;
            padding-left: 6px;
            padding-top: 5px;
        }

        QPushButton:disabled {
            background-color: #f5f5f5;
            color: #aaaaaa;
            border: 1px solid #dddddd;
        }
    )";
};

#endif //YOUTUBEMEDIADOWNLOADER_NOTIFICATIONWINDOW_H
