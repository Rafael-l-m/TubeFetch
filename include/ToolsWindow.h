
#ifndef YOUTUBEMEDIADOWNLOADER_TOOLSWINDOW_H
#define YOUTUBEMEDIADOWNLOADER_TOOLSWINDOW_H

#include <QLineEdit>

class ToolsWindow final : public QDialog {
    Q_OBJECT
public:
    static inline const QString defaultOrganization = "rafael";
    static inline const QString defaultApplication = "YouTubeMediaDownloaderTools";

    explicit ToolsWindow(QIcon windowIcon = QIcon(""), QWidget *parent = nullptr);

    static QString readToolPath(const QString& key, const QString& organization = defaultOrganization, const QString& application = defaultApplication);
    static bool writeToolPath(QMap<QString, QString> &map, const QString &organization = defaultOrganization, const QString &application = defaultApplication);

private slots:
    void changeToolPath(const QString& toolName, QLineEdit *lineEdit, const QString& content, const QString& filter);
    void saveSettings() const;

private:
    static QString findToolPath(const QString& program);
    static QString checkExecutable(const QString& program);
    const QString windowTitle = tr("Tools");
    const std::pair<int, int> labSize {65, 25};
    const std::pair<int, int> spacerItemSize {30, 10};
    const std::pair<int, int> dirSize {600, 40};
    const std::pair<int, int> windowSize {860, 550};

    QIcon windowIcon;

    QGridLayout* grid;
    QLabel* lab1;
    QLineEdit* dir1;
    QPushButton* btn1;
    QLabel* lab2;
    QLineEdit* dir2;
    QPushButton* btn2;
    QLabel* lab3;
    QLineEdit* dir3;
    QPushButton* btn3;
    QPushButton* okBtn;

    const QString styles = R"(
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

        QLineEdit[readOnly="true"] {
            background-color: #686868;
            color: #F2F2F2;
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
            background-color: #e5f4f6;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 5px;
        }

        QListView::item:selected, QTreeView::item:selected {
            background-color: #a3d5f7;
            color: #1c2e40;
        }

        QListView::item:hover, QTreeView::item:hover {
            background-color: #c0e0f0;
        }

        QComboBox {
            background-color: #e5f4f6;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 5px;
            padding: 4px 8px;
            font-size: 12px;
        }

        QComboBox QAbstractItemView {
            background-color: #e5f4f6;
            color: #333333;
            selection-background-color: #a3d5f7;
        }

        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 20px;
            border-left: 1px solid #cccccc;
            border-top-right-radius: 5px;
            border-bottom-right-radius: 5px;
        }

        QComboBox::down-arrow {
            width: 7px;
            height: 10px;
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
        }

        QToolButton:pressed {
            background-color: #a3d5f7;
        }

        QScrollBar:vertical {
            background-color: #c0e0f0;
            width: 8px;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical {
            background-color: #a3d5f7;
            border-radius: 4px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #89c2e0;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )";
};

#endif //YOUTUBEMEDIADOWNLOADER_TOOLSWINDOW_H
