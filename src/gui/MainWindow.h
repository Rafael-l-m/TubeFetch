
#ifndef DOWNLOADHIGHQUALITYAUDIO_MAINWINDOW_H
#define DOWNLOADHIGHQUALITYAUDIO_MAINWINDOW_H

#include "../external/json.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDebug>
#include <QEventLoop>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHash>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QQueue>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QTextEdit>
#include <QTimer>
#include <QTranslator>
#include <QVBoxLayout>
#include <QWidget>

template <typename T = QString>
class SelectionURL {
public:
    SelectionURL() = default;

    [[nodiscard]] const T& getURL() const {return this->URL;}

    [[nodiscard]] const T& getPath() const {return this->path;}

    [[nodiscard]] const T& getDefaultName() const {return this->defaultName;}

    [[nodiscard]] const T& getName() const {return this->rename;}

    [[nodiscard]] const T& getFormat() const {return this->docSuf;}

    void setURL(const T& newURL) {this->URL = newURL;}

    void setPath(const T& newPath) {this->path = newPath;}

    void setDefaultName(const T& newDefaultName) {this->defaultName = newDefaultName;}

    void setName(const T& newName) {this->rename = newName;}

    void setFormat(const T& newFormat) {this->docSuf = newFormat;}

private:
    T URL;
    T path;
    T defaultName;
    T rename;
    T docSuf;
};

class URLManager {
public:
    static auto isValidURL(const QString& url) {
        qDebug() << "Begin URLManager->isValidURL";

        static const QRegularExpression pattern(R"(^(https?|ftp)://[^\s/$.?#].[^\s]*$)", QRegularExpression::CaseInsensitiveOption);

        qDebug() << "Finish URLManager->isValidURL";

        return pattern.match(url).hasMatch();
    }

    static auto cutURL(const QString& url) {
        qDebug() << "Begin URLManager->cutURL";

        const auto pos = url.indexOf('&');

        qDebug() << "Finish URLManager->cutURL";

        if (pos != -1) return url.left(pos); return url;
    }

    static nlohmann::json getInfo(const QString& yt_dlp_path, const QString& url, const QString& referer) {
        qDebug() << "Begin URLManager->getInfo";

        QProcess process;

        const QString userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                                  "AppleWebKit/537.36 (KHTML, like Gecko) "
                                  "Chrome/115.0.0.0 Safari/537.36";

        QStringList arguments;

        arguments << "--dump-json" << url
                  << "--user-agent" << userAgent
                  << "--referer" << referer;

        qDebug() << "1";
        process.start(yt_dlp_path, arguments);

        qDebug() << "2";
        const bool finished = process.waitForFinished(60000); // 最多等待60000秒

        if (!finished) {
            qDebug() << "-1";
            process.kill();

            qDebug() << "Finish URLManager->getInfo";
            return {};
        }

        qDebug() << "3";
        const auto output = process.readAllStandardOutput();
        const auto errorOutPut = process.readAllStandardError();
        qDebug() << "4";

        if (!errorOutPut.isEmpty()) {
            qDebug() << "-2";

            qWarning() << "Error OutPut" << errorOutPut;
        }

        try {
            nlohmann::json j = nlohmann::json::parse(output.toStdString());
            qDebug() << "6";

            qDebug() << "Finish URLManager->getInfo";

            return j;
        }

        catch (const std::exception& err) {
            qDebug() << "-3";
            qWarning() << "Parsing json failed" << err.what();
            qDebug() << "Debug content: " << output;
        }

        qDebug() << "-4";
        qDebug() << "Finish URLManager->getInfo";

        return {};
    }
};

class DownloadItemManager final : public QDialog {
    Q_OBJECT
public:
    explicit DownloadItemManager(QWidget* parent = nullptr, QString yt_dlpPath = "") : QDialog(parent), yt_dlp_path(std::move(yt_dlpPath)) {
        auto* mainLayout = new QVBoxLayout(this);

        auto* row1 = new QHBoxLayout(this);
        this->urlLabel = new QLabel(tr("URL: "));
        this->urlLabel->setStyleSheet(this->labelStyle);  // Set style
        this->urlInput = new QLineEdit(this);
        this->urlInput->setFixedSize(this->defaultLDWidth, this->defaultLDHeight);  // Set size
        this->urlInput->setPlaceholderText(tr("Input download address ..."));  // Set default text
        this->urlInput->setStyleSheet(this->lineEditStyle);  // Set style

        auto* debounceTimer = new QTimer(this);
        debounceTimer->setInterval(this->waitInterval);  // Set interval
        debounceTimer->setSingleShot(true);  // Single shot

        row1->addStretch();  // Add stretch
        row1->addWidget(this->urlLabel);  // Add widget
        row1->addSpacerItem(new QSpacerItem(this->spacerItemWidth, this->spacerItemHeight, QSizePolicy::Expanding, QSizePolicy::Minimum));  // Add spacer
        row1->addWidget(this->urlInput);  // Add widget
        row1->addStretch();  // Add stretch

        auto* row2 = new QHBoxLayout(this);
        this->titleLab = new QLabel(tr("Title: "));
        this->titleLab->setStyleSheet(this->labelStyle);  // Set style
        this->titleLD = new QLineEdit(this);
        this->titleLD->setFixedSize(this->defaultLDWidth, this->defaultLDHeight);  // Set size
        this->titleLD->setEnabled(false);  // Not interactive
        this->titleLD->setStyleSheet(this->lineEditStyle);  // Set style

        row2->addStretch();  // Add stretch
        row2->addWidget(this->titleLab);  // Add widget
        row2->addWidget(this->titleLD);  // Add widget
        row2->addStretch();  // Add stretch

        auto* row3 = new QHBoxLayout(this);
        this->pushButton = new QPushButton(tr("OK"));
        this->pushButton->setStyleSheet(this->pushButtonStyle);  // Set style

        row3->addStretch();  // Add stretch
        row3->addWidget(this->pushButton);  // Add widget
        row3->addStretch();  // Add stretch

        mainLayout->addLayout(row1);  // Add layout
        mainLayout->addLayout(row2);  // Add layout
        mainLayout->addLayout(row3);  // Add layout

        this->setWindowTitle(this->windowTitle);  // Set window title
        this->resize(this->defaultWindowWidth, this->defaultWindowHeight);  // Set size
        this->setStyleSheet(this->dialogStyle);  // Set style

        connect(this->urlInput, &QLineEdit::textChanged, this, [=](const QString&) {debounceTimer->start();});
        connect(debounceTimer, &QTimer::timeout, this, [=, this]() {handleDebounceTimer(this->urlInput->text());});
        connect(this->pushButton, &QPushButton::clicked, this, &DownloadItemManager::handleClickButton);
    }

    [[nodiscard]] auto getInfo() const {return this->su;}

private slots:
    void handleDebounceTimer(const QString& text) {
        qDebug() << "Begin DownloadItemManager->handleDebounceTimer";

        if (!text.isEmpty()) {
            qDebug() << "1";

            if (URLManager::isValidURL(text)) {
                qDebug() << "2";
                this->URL = URLManager::cutURL(text);

                qDebug() << "3";
                this->urlInput->setEnabled(false);  // Disable lineEdit
                this->pushButton->setEnabled(false);  // Disable button

                qDebug() << "4";
                sleepForMilliseconds(this->sleepTime);  // Pause

                qDebug() << "5";
                try {
                    const nlohmann::json infoJson = URLManager::getInfo(this->yt_dlp_path, this->URL, this->youtubeReferer);
                    qDebug() << "6";

                    const auto title = QString::fromStdString(infoJson.value("title", "Failed"));
                    qDebug() << "7";

                    this->titleLD->setText(title);  // Set title
                    qDebug() << "8";
                }

                catch (...) {
                    qDebug() << "-3";
                    this->titleLD->setText("Failed");
                }

                this->urlInput->setEnabled(true);  // Enable lineEdit
                this->pushButton->setEnabled(true);  // Enable button
                qDebug() << "9";
            }

            else {
                qDebug() << "-2";
                QMessageBox::warning(this, tr("Warning"), tr("Invalid URL"));  // Invalid URL
            }
        }

        else {
            qDebug() << "-1";
            QMessageBox::warning(this, tr("Warning"), tr("No URL provided"));  // Invalid character
        }

        qDebug() << "Finish DownloadItemManager->handleDebounceTimer";
    };

    void handleClickButton() {
        qDebug() << "Begin DownloadItemManager->handleClickButton";

        if (this->titleLD->text().isEmpty() || this->titleLD->text() == "Failed") {
            qDebug() << "-1";
            QMessageBox::warning(this, tr("Warning"), tr("No valid audio URL"));
            return;
        }

        this->urlInput->setEnabled(false);  // Disable lineEdit

        QString selectedFilter;

        const QString filePath = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                QDir::homePath() + "/" + this->titleLD->text(),
                this->filter,
                &selectedFilter
                );

        qDebug() << "1";
        const QFileInfo inf(filePath);

        qDebug() << "2";
        this->su.setURL(this->URL);
        this->su.setPath(inf.absoluteFilePath());
        this->su.setDefaultName(this->titleLD->text());
        this->su.setName(inf.fileName());
        this->su.setFormat(inf.suffix());

        qDebug() << "3";
        #ifdef Q_OS_LINUX
        if (inf.suffix().isEmpty()) {
            qDebug() << "3.1";

            if (selectedFilter.contains("*.mkv")) {
                qDebug() << "3.2";
                this->su.setFormat("mkv");
            }

            else if (selectedFilter.contains("*.mp4")) {
                qDebug() << "3.3";
                this->su.setFormat("mp4");
            }

            else if (selectedFilter.contains("*.mov")) {
                qDebug() << "3.4";
                this->su.setFormat("mov");
            }

            else if (selectedFilter.contains("*.mxf")) {
                qDebug() << "3.5";
                this->su.setFormat("mxf");
            }

            else if (selectedFilter.contains("*.m4a")) {
                qDebug() << "3.6";
                this->su.setFormat("m4a");
            }

            else if (selectedFilter.contains("*.avi")) {
                qDebug() << "3.7";
                this->su.setFormat("avi");
            }

            else if (selectedFilter.contains("*.flv")) {
                qDebug() << "3.7";
                this->su.setFormat("flv");
            }

            else if (selectedFilter.contains("*.m4v")) {
                qDebug() << "3.7";
                this->su.setFormat("m4v");
            }

            else if (selectedFilter.contains("*.ts")) {
                qDebug() << "3.7";
                this->su.setFormat("ts");
            }

            else if (selectedFilter.contains("*.ogv")) {
                qDebug() << "3.7";
                this->su.setFormat("ogv");
            }

            else if (selectedFilter.contains("*.3gp")) {
                qDebug() << "3.7";
                this->su.setFormat("3gp");
            }

            else {
                qDebug() << "3.8";
                this->su.setFormat("mp3");
            }
        }
        #endif

        qDebug() << "4";
        accept();

        this->close();

        qDebug() << "Finish DownloadItemManager->handleClickButton";
    }

private:
    const QString windowTitle = tr("Download Info");
    const QString youtubeReferer = "https://www.youtube.com/";
    const int waitInterval = 1000, sleepTime = 2000;
    const int spacerItemWidth = 30, spacerItemHeight = 10;
    const int defaultWindowWidth = 260, defaultWindowHeight = 260;
    const int defaultLDWidth = 600, defaultLDHeight = 40;

    QString yt_dlp_path;

    QString URL;
    QLabel* urlLabel;
    QLineEdit* urlInput;

    QLabel* titleLab;
    QLineEdit* titleLD;

    QPushButton* pushButton;

    SelectionURL<> su {};

    static void sleepForMilliseconds(const int msecs) {
        QEventLoop loop;
        QTimer::singleShot(msecs, &loop, &QEventLoop::quit);
        loop.exec();
    }

    const QString filter =
            "MKV (*.mkv);;"
            "MP4 (*.mp4);;"
            "MOV (*.mov);;"
            "MXF (*.mxf);;"
            "M4A (*.m4a);;"
            "AVI (*.avi);;"
            "FLV (*.flv);;"
            "M4V (.m4v);;"
            "TS (.ts);;"
            "OGV (.ogv);;"
            "3GP (.3gp);;"
            "All Files (*)";

    const QString dialogStyle = R"(
        QDialog {
            background-color: #fffaf0;
            font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif;
            font-size: 12px;
        }

        QMessageBox QLabel {
            color: #222;
            font-size: 13px;
        }
    )";

    const QString labelStyle = R"(
        QLabel {
            color: #000000;
            font-weight: bold;
            font-size: 12px;
        }
    )";

    const QString lineEditStyle = R"(
        QLineEdit {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 5px;
            padding: 6px;
            font-size: 12px;
        }

        QLineEdit:disabled {
            background-color: #aaaaaa;
            color: #ff2700;
        }
    )";

    const QString pushButtonStyle = R"(
        QPushButton {
            color: #333333;
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f9d976, stop:1 #f39f86);
            border: 1px solid #dcae7e;
            border-radius: 8px;
            padding: 8px 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fceabb, stop:1 #f8b88b);
        }

        QPushButton:pressed {
            background-color: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:0 #f39f86, stop:1 #fceabb);
            padding-top: 10px;
            padding-bottom: 6px;
        }

        QPushButton::disabled {
            color: #aaaaaa;
            background-color: #f0e8d8;
            border: 1px solid #cccccc;
        }
    )";
};

class DownloadItemWidget final : public QWidget {
    Q_OBJECT
public:
    explicit DownloadItemWidget(QWidget* parent = nullptr, const QString& title = "", const QString& rename = "", const int progressValue = 0) : QWidget(parent) {
        auto* layout = new QHBoxLayout(this);

        this->titleLab = new QLabel(title, this);
        this->titleLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);  // Fixed height, preferred width
        this->titleLab->setMinimumSize(this->widthLB, this->heightLB);  // Set min size
        this->titleLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // Left-aligned text, vertically centered
        this->titleLab->setWordWrap(false);  // Disable automatic line break
        this->titleLab->setStyleSheet(this->labelStyle);  // Set style

        this->titleSc = new QScrollArea(this);
        this->titleSc->setWidget(this->titleLab);
        this->titleSc->setWidgetResizable(false);  // Disable automatic scaling to fill scrollable region
        this->titleSc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable horizontal scrolling permanently
        this->titleSc->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable vertical scrolling permanently
        this->titleSc->setFixedSize(this->widthSB, this->heightSB);  // Set default size
        this->titleSc->setFrameShape(QFrame::NoFrame);  // No Border
        this->titleSc->horizontalScrollBar()->setStyleSheet(this->scrollBarStyle);  // Set style

        this->filenameLab = new QLabel(rename, this);
        this->filenameLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);  // Fixed height, preferred width
        this->filenameLab->setMinimumSize(this->widthLB, this->heightLB);  // Set min size
        this->filenameLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // Left-aligned text, vertically centered
        this->filenameLab->setWordWrap(false);  // Disable automatic line break
        this->filenameLab->setStyleSheet(this->labelStyle);  // Set style

        this->filenameSc = new QScrollArea(this);
        this->filenameSc->setWidget(this->filenameLab);
        this->filenameSc->setWidgetResizable(false);  // Disable automatic scaling to fill scrollable region
        this->filenameSc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable horizontal scrolling permanently
        this->filenameSc->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable vertical scrolling permanently
        this->filenameSc->setFixedSize(this->widthSB, this->heightSB);  // Set default size
        this->filenameSc->setFrameShape(QFrame::NoFrame);  // No Border
        this->filenameSc->horizontalScrollBar()->setStyleSheet(this->scrollBarStyle);  // Set style

        this->progress = new QProgressBar(this);
        this->progress->setValue(progressValue);  // Set default value
        this->progress->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);  // Resizable horizontally, fixed height vertically
        this->progress->setStyleSheet(this->progressBarStyle);  // Set style

        layout->addWidget(this->titleSc);  // Add widget
        layout->addWidget(this->filenameSc);  // Add widget
        layout->addWidget(this->progress);  // Add widget

        layout->setSpacing(this->distWG);  // Set distance between widget

        this->setStyleSheet(this->style);  // Set style
        this->setFixedHeight(this->heightQW);  // Set default height
    }

    void setTitle(const QString& t) const {
        this->titleLab->setText(t);  // Set title
    }

    void setFileName(const QString& fn) const {
        this->filenameLab->setText(fn);  // Set FileName
    }

    void setProgress(const int percent) const {
        this->progress->setValue(percent);  // Set progress
    }

private:
    QLabel* titleLab;
    QScrollArea* titleSc;
    QLabel* filenameLab;
    QScrollArea* filenameSc;
    QProgressBar* progress;

    const int heightLB = 48, widthLB = 150;
    const int heightSB = 60, widthSB = 150;
    const int distWG = 15;
    const int heightQW = 80;

    const QString style = R"(
        QWidget {
            background-color: #fffaf0;
            border: 1px solid #ddd;
            padding: 5px;
            border-radius: 10px;
        }
    )";

    const QString labelStyle = R"(
        QLabel {
            background-color: #fffaf0;
            color: #5c4330;
            border: none;
            padding: 4px;
            font-size: 12px;
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

    const QString progressBarStyle = R"(
        QProgressBar {
            border: 1px solid #ccc;
            border-radius: 6px;
            background-color: #e6e6e6;
            text-align: center;
            height: 18px;
        }

        QProgressBar::chunk {
            background-color: #4da6ff;
            border-radius: 6px;
        }
    )";
};

class mw final : public QMainWindow {
    Q_OBJECT
public:
    explicit mw(QWidget* parent = nullptr, const QString& ico_path = "", QString yt_dlp_path = "", QString ffmpeg_path = "") : QMainWindow(parent), windowIco(QIcon(ico_path)), yt_dlp_path(std::move(yt_dlp_path)), ffmpeg_path(std::move(ffmpeg_path)) {
        auto* central = new QWidget(this);
        auto* mainLayout = new QVBoxLayout(central);

        auto* shadow = new QGraphicsDropShadowEffect(this);
        shadow->setOffset(this->shadowOffsetHor, this->shadowOffsetVer);  // Set shadow offset
        shadow->setBlurRadius(this->shadowBlurRadius);  // Set blur radius
        shadow->setColor(this->shadowColor);  // Set shadow color

        auto* buttonLayout1 = new QHBoxLayout(this);

        this->addBtn = new QPushButton(tr("Add task"));
        this->addBtn->setStyleSheet(this->pushButtonStyle);  // Set style
        this->addBtn->setGraphicsEffect(shadow);  // Set shadow

        this->delBtn = new QPushButton(tr("Remove last task"));
        this->delBtn->setStyleSheet(this->pushButtonStyle);  // Set style
        this->delBtn->setGraphicsEffect(shadow);  // Set shadow

        buttonLayout1->addStretch();  // Add stretch
        buttonLayout1->addWidget(this->addBtn);  // Add widget
        buttonLayout1->addSpacerItem(new QSpacerItem(this->spacerItemWidth, this->spacerItemHeight, QSizePolicy::Expanding, QSizePolicy::Minimum));  // Add spacer
        buttonLayout1->addWidget(this->delBtn);  // Add widget
        buttonLayout1->addStretch();  // Add stretch

        auto* buttonLayout2 = new QHBoxLayout(this);

        this->startBtn = new QPushButton(tr("Start"));
        this->startBtn->setStyleSheet(this->pushButtonStyle);  // Set style
        this->startBtn->setGraphicsEffect(shadow);  // Set shadow

        buttonLayout2->addStretch();  // Add stretch
        buttonLayout2->addWidget(this->startBtn);  // Add widget
        buttonLayout2->addStretch();  // Add stretch

        auto* line1 = new QFrame(this);
        line1->setFrameShape(QFrame::HLine);  // Set shape
        line1->setFrameShadow(QFrame::Sunken);  // Set shadow
        line1->setStyleSheet(this->frameStyle);  // Set style

        auto* line2 = new QFrame(this);
        line2->setFrameShape(QFrame::HLine);  // Set shape
        line2->setFrameShadow(QFrame::Sunken);  // Set shadow
        line2->setStyleSheet(this->frameStyle);  // Set style

        this->scrollArea = new QScrollArea(this);
        this->scrollArea->setWidgetResizable(true);  // Resize automatically

        this->scrollContainer = new QWidget(this);

        this->scrollLayout = new QVBoxLayout(this->scrollContainer);
        this->scrollLayout->setAlignment(Qt::AlignTop);  // Set to the top

        this->scrollArea->setWidget(this->scrollContainer);
        this->scrollArea->viewport()->setStyleSheet(this->scrollBarViewPortStyle);  // Set style to visible area

        mainLayout->addLayout(buttonLayout1);  // Add layout
        mainLayout->addWidget(line1);  // Add widget
        mainLayout->addWidget(this->scrollArea);  // Add widget
        mainLayout->addWidget(line2);  // Add widget
        mainLayout->addLayout(buttonLayout2);  // Add layout

        this->setWindowIcon(QIcon(ico_path));  // Set window icon
        this->setCentralWidget(central);  // Set central widget
        this->setStyleSheet(this->widgetStyle);  // Set style
        this->setWindowTitle(this->windowTitle);  // Set window title
        this->resize(this->windowWidth, this->windowHeight);  // Set size

        connect(this->addBtn, &QPushButton::clicked, this, &mw::addElement);
        connect(this->delBtn, &QPushButton::clicked, this, &mw::removeLast);
        connect(this->startBtn, &QPushButton::clicked, this, &mw::startProcess);
    }

    static auto generateRandomKString(const int minK, const int maxK) {
        const auto numberK = QRandomGenerator::global()->bounded(minK, maxK + 1);  // Generate random format: int + "k"
        return QString::number(numberK) + "k";
    }

private slots:
    void addElement() {
        qDebug() << "Begin MainWindow->addElement";

        DownloadItemManager dm(this, yt_dlp_path);

        qDebug() << "1";
        if (dm.exec() == QDialog::Accepted) {
            const auto info = dm.getInfo(); // info: SelectionURL

            qDebug() << "2";
            const auto& title = info.getDefaultName();
            const auto& rename = info.getName();

            qDebug() << "3";
            if (!info.getURL().isEmpty() &&
                !info.getPath().isEmpty() &&
                !title.isEmpty() &&
                !rename.isEmpty() &&
                !info.getFormat().isEmpty()) {
                qDebug() << "4";
                auto* dw = new DownloadItemWidget(this, title, rename);

                qDebug() << "5";
                scrollLayout->addWidget(dw);  // Add widget

                selection[counter] = info; // Store in container

                ++counter; // Increase the counter
                qDebug() << "6";
            }

            else qDebug() << "-2";
        }

        else qDebug() << "-1";

        qDebug() << "Finish MainWindow->addElement";
    };

    void removeLast() {
        qDebug() << "Begin MainWindow->removeLast";

        if (counter == 0) {
            qDebug() << "-1";
            QMessageBox::warning(this, tr("Failed"), tr("Deleted failed"));
        }

        else {
            qDebug() << "1";

            if (const int lastIndex = this->scrollLayout->count() - 1; lastIndex >= 0) {
                qDebug() << "2";
                const auto* item = this->scrollLayout->takeAt(lastIndex);

                qDebug() << "3";
                if (item) {
                    qDebug() << "4";
                    auto* widget = item->widget();

                    qDebug() << "5";
                    if (widget) {
                        qDebug() << "6";
                        widget->hide();
                        widget->setParent(nullptr);
                        delete widget;
                    }

                    else qDebug() << "-4";

                    qDebug() << "7";
                    delete item;
                }

                else qDebug() << "-3";
            }

            else qDebug() << "-2";

            qDebug() << "8";
            selection.remove(--counter);
        }

        qDebug() << "Finish MainWindow->removeLast";
    }

    void startProcess() {
        qDebug() << "Begin MainWindow->startProcess";

        if (this->counter > 0) {
            qDebug() << "1";

            this->addBtn->setEnabled(false);  // Disable button
            this->delBtn->setEnabled(false);  // Disable button
            this->startBtn->setEnabled(false);  // Disable button
            this->finishedCount = 0;  // Reset counter
            this->downloadQueue.clear();  // Clear Queue

            qDebug() << "2";
            qApp->processEvents();  // Refresh

            qDebug() << "3";
            for (uint64_t i = 0; i < this->counter; ++i) {
                this->downloadQueue.enqueue(selection[i]);
            }

            qDebug() << "4";
            startNextDownload(0);
        }

        else {
            qDebug() << "-1";
            QMessageBox::warning(this, tr("Failed"), tr("No valid tasks to process"));
        }

        qDebug() << "Finish MainWindow->startProcess";
    }

    void startNextDownload(uint64_t num) {
        qDebug() << "Begin MainWindow->startNextDownload";

        if (this->downloadQueue.isEmpty()) {
            qDebug() << "-1";
            QMessageBox::information(this, tr("Finished"), tr("All downloads completed"));

            this->addBtn->setEnabled(true);
            this->delBtn->setEnabled(true);
            this->startBtn->setEnabled(true);

            qDebug() << "-2";
            this->selection.clear();
            this->counter = 0;

            qDebug() << "-3";
            while (const auto* item = this->scrollLayout->takeAt(0)) {
                if (auto* widget = item->widget())
                    widget->deleteLater();
                delete item;
            }

            qDebug() << "-4";
            return;
        }

        const auto task = this->downloadQueue.dequeue();

        qDebug() << "1";
        const auto* item = this->scrollLayout->itemAt(static_cast<int>(num));

        qDebug() << "2";
        if (!item) return startNextDownload(++num);

        qDebug() << "3";
        auto* widget = item->widget();

        qDebug() << "4";
        if (!widget) return startNextDownload(++num);;

        qDebug() << "5";
        const auto* downloadWidget = qobject_cast<DownloadItemWidget*>(widget);

        qDebug() << "6";
        if (!downloadWidget) return startNextDownload(++num);

        qDebug() << "7";
        const int randomDelayMs = QRandomGenerator::global()->bounded(7000, 12000 + 1);

        qDebug() << "8";
        QTimer::singleShot(randomDelayMs, this, [=, this]() {
            auto *process = new QProcess(this);

            qDebug() << "8";
            QStringList arguments;
            arguments << "--ffmpeg-location" << ffmpeg_path
                      << "-f" << "bv+ba"
                      << "--no-playlist"
                      << "--merge-output-format" << task.getFormat()
                      << "--limit-rate" << generateRandomKString(this->minRandom, this->maxRandom)
                      << "--output" << task.getPath()
                      << task.getURL();
            qDebug() << "9";

            connect(process, &QProcess::readyReadStandardOutput, [=]() {
                #ifdef Q_OS_WIN
                    qDebug() << "9.1";
                    const auto text = QString::fromLocal8Bit(process->readAllStandardOutput());
                #else
                    qDebug() << "9.2";
                    const auto text = QString::fromUtf8(process->readAllStandardOutput());
                #endif

                qDebug() << text;

                static const QRegularExpression re(R"(\[download\]\s+(\d+(\.\d+)?)%)");

                if (const auto match = re.match(text); match.hasMatch()) {
                    const auto progress = match.captured(1).toDouble();
                    downloadWidget->setProgress(static_cast<int>(progress));
                    qDebug() << static_cast<int>(progress);
                }
            });

            connect(process, &QProcess::readyReadStandardError, [=, this]() {
                qDebug() << "-5";
                QMessageBox::warning(this, tr("Download Failed"),
                                     QString::fromLocal8Bit(process->readAllStandardError()));  // Show Error
            });

            connect(process, &QProcess::finished, this, [=, this]() mutable {
                qDebug() << "-6";
                process->deleteLater();
                ++this->finishedCount;
                startNextDownload(++num);
            });

            process->start(yt_dlp_path, arguments);
        });

        qDebug() << "Finish MainWindow->startNextDownload";
    }

private:
    const QString windowTitle = tr("Audio Downloader");
    const int shadowOffsetHor = 2, shadowOffsetVer = 2, shadowBlurRadius = 10;
    const QColor shadowColor = QColor(0, 0, 0, 100);
    const int windowWidth = 800, windowHeight = 600;
    const int minRandom = 275, maxRandom = 550;
    const int spacerItemWidth = 30, spacerItemHeight = 20;

    QIcon windowIco;
    QTranslator translator;
    QPushButton* addBtn;
    QPushButton* delBtn;
    QPushButton* startBtn;
    QScrollArea* scrollArea;
    QWidget* scrollContainer;
    QVBoxLayout* scrollLayout;
    QVector<DownloadItemWidget*> items;

    QString yt_dlp_path;
    QString ffmpeg_path;

    uint64_t counter = 0;
    QHash<uint64_t, SelectionURL<>> selection{};

    uint64_t finishedCount = 0;
    QQueue<SelectionURL<>> downloadQueue{};

    void closeEvent(QCloseEvent* event) override {
        const auto reply = QMessageBox::question(this,
                                                 tr("Exit Application"),
                                                 tr("Are you sure you want to exit?"),
                                                 QMessageBox::Yes | QMessageBox::No,
                                                 QMessageBox::No);

        reply == QMessageBox::Yes ? event->accept() : event->ignore();
    }

    const QString widgetStyle = R"(
        QMainWindow {
            background-color: #202a30;
        }
    )";

    const QString frameStyle = R"(
        QFrame {
            background-color: #2c3e50;
            border: 1px solid #1f2a33;
            border-radius: 6px;
        }
    )";

    const QString scrollBarStyle = R"(
        QScrollBar:vertical {
            border: none;
            background: #2b3942;
            width: 10px;
            margin: 2px 0 2px 0;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical {
            background: #4ca1af;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar::handle:vertical:hover {
            background: #5fb4be;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
            subcontrol-origin: margin;
        }

        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
    )";

    const QString scrollBarViewPortStyle = R"(
        background-color: #2c3e50;
        border-radius: 6px;
    )";

    const QString pushButtonStyle = R"(
        QPushButton {
            color: white;
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4ca1af, stop:1 #2c3e50);
            border: 1px solid #2c3e50;
            border-radius: 8px;
            padding: 8px 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5fb4be, stop:1 #34495e);
        }

        QPushButton:pressed {
            background-color: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:0 #34495e, stop:1 #5fb4be);
            padding-top: 10px;
            padding-bottom: 6px;
        }

        QPushButton::disabled {
            color: #aaaaaa;
            background-color: #2a2f36;
            border: 1px solid #1f242a;
        }
    )";
};

#endif //DOWNLOADHIGHQUALITYAUDIO_MAINWINDOW_H
