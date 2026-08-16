
#include "NotificationWindow.h"
#include "ToolsWindow.h"
#include <QDirIterator>
#include <QFileDialog>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>


// Public

ToolsWindow::ToolsWindow(QIcon windowIcon, QWidget *parent) : QDialog(parent), windowIcon(std::move(windowIcon)) {
    this->grid = new QGridLayout(this);

    // Row 1
    this->lab1 = new QLabel(tr("yt-dlp: "), this);

    this->lab1->setFixedSize(this->labSize.first, this->labSize.second);

    this->dir1 = new QLineEdit(this);

    this->dir1->setFixedSize(this->dirSize.first, this->dirSize.second);

    this->dir1->setReadOnly(true);

    #ifdef Q_OS_WIN
        const QString yt_dlp = "yt-dlp.exe";
    #else
        const QString yt_dlp = "yt-dlp";
    #endif

    if (const auto dir1Path = findToolPath(yt_dlp); !dir1Path.isEmpty()) this->dir1->setText(dir1Path);

    this->btn1 = new QPushButton(tr("Search"), this);

    this->grid->addWidget(this->lab1, 0, 0);
    this->grid->addWidget(this->dir1, 0, 1);
    this->grid->addWidget(this->btn1, 0, 2);

    // Row 2
    this->lab2 = new QLabel(tr("ffmpeg: "), this);

    this->lab2->setFixedSize(this->labSize.first, this->labSize.second);

    this->dir2 = new QLineEdit(this);

    this->dir2->setFixedSize(this->dirSize.first, this->dirSize.second);

    this->dir2->setReadOnly(true);

    #ifdef Q_OS_WIN
        const QString ffmpeg = "ffmpeg.exe";
    #else
        const QString ffmpeg = "ffmpeg";
    #endif

    if (const auto dir2Path = findToolPath(ffmpeg); !dir2Path.isEmpty()) this->dir2->setText(dir2Path);

    this->btn2 = new QPushButton(tr("Search"), this);

    this->grid->addWidget(this->lab2, 1, 0);
    this->grid->addWidget(this->dir2, 1, 1);
    this->grid->addWidget(this->btn2, 1, 2);

    // Row 3
    this->lab3 = new QLabel(tr("node.js: "), this);

    this->lab3->setFixedSize(this->labSize.first, this->labSize.second);

    this->dir3 = new QLineEdit(this);
    this->dir3->setFixedSize(this->dirSize.first, this->dirSize.second);
    this->dir3->setReadOnly(true);

    #ifdef Q_OS_WIN
        const QString node_js = "node.exe";
    #else
        const QString node_js = "node";
    #endif

    if (const auto dir3Path = findToolPath(node_js); !dir3Path.isEmpty()) this->dir3->setText(dir3Path);

    this->btn3 = new QPushButton(tr("Search"), this);

    this->grid->addWidget(this->lab3, 2, 0);
    this->grid->addWidget(this->dir3, 2, 1);
    this->grid->addWidget(this->btn3, 2, 2);

    // Row 4
    this->okBtn = new QPushButton(tr("Confirm"), this);
    this->grid->addWidget(this->okBtn, 3, 1);

    // Dialog settings
    this->setWindowTitle(this->windowTitle);
    this->setWindowIcon(windowIcon);
    this->setFixedSize(this->windowSize.first, this->windowSize.second);
    this->setStyleSheet(this->styles);

    const QString filter =
        #ifdef Q_OS_WIN
            tr("Executable File(*.exe)");
        #elif defined(Q_OS_MAC)
                tr("Executable File(*);;App Store File(*.app)");
        #else
                    tr("Executable File(*)");
        #endif

    connect(this->btn1, &QPushButton::clicked, this, [this, filter]() {this->changeToolPath("yt-dlp", this->dir1, tr("Choose yt-dlp"), filter);});
    connect(this->btn2, &QPushButton::clicked, this, [this, filter]() {this->changeToolPath("ffmpeg", this->dir2, tr("Choose ffmpeg"), filter);});
    connect(this->btn3, &QPushButton::clicked, this, [this, filter]() {this->changeToolPath("node", this->dir3, tr("Choose node.js"), filter);});
    connect(this->okBtn, &QPushButton::clicked, this, [this]() {
        this->saveSettings();
        this->close();
    });
}

QString ToolsWindow::readToolPath(const QString& key, const QString& organization, const QString& application) {
    const QSettings sysSettings(QSettings::SystemScope, organization, application);
    const QSettings userSettings(QSettings::UserScope, organization, application);

    if (sysSettings.contains(key)) return sysSettings.value(key).toString();

    if (userSettings.contains(key)) return userSettings.value(key).toString();

    return {};
}

bool ToolsWindow::writeToolPath(QMap<QString, QString> &map, const QString &organization, const QString &application) {
    QSettings userSettings(QSettings::UserScope, organization, application);

    for (const auto& [key, value] : map.asKeyValueRange()) {userSettings.setValue(key, value);}

    userSettings.sync();

    return userSettings.status() == QSettings::NoError;
}


// Private slots

void ToolsWindow::changeToolPath(const QString& toolName, QLineEdit *lineEdit, const QString& content, const QString& filter) {
    int i = 1;

    if (const auto path = lineEdit->text(); !path.isEmpty()) {
        ++i;

        if (NotificationWindow::confirmation(tr("Modify Executable File"), tr("The program has detected the executable file path. Do you wish to modify it?"))) ++i;
    }

    if (i == 1 || i == 3) {
        QFileDialog dialog (
            this,
            content,
            QDir::homePath(),
            filter
        );

        #ifdef Q_OS_MAC
            dialog.setOption(QFileDialog::DontUseNativeDialog, true);
        #endif

        dialog.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

        QString filePath;

        if (dialog.exec() == Accepted) filePath = dialog.selectedFiles().first();

        if (const auto res = checkExecutable(filePath); res.isEmpty()) NotificationWindow::critical(tr("Error"), tr("Invalid executable file.\nPlease try again"));

        else NotificationWindow::information(tr("Success"), tr("Path changed correctly"));

        lineEdit->setText(filePath);
    }
}

void ToolsWindow::saveSettings() const {
    QMap<QString, QString> params;

    params.insert("tools/yt_dlp_path", this->dir1->text());
    params.insert("tools/ffmpeg_path", this->dir2->text());
    params.insert("tools/node_js_path", this->dir3->text());

    if (writeToolPath(params)) NotificationWindow::information(tr("Success"), tr("Path saved correctly"));

    else NotificationWindow::warning(tr("Failed"), tr("Oops, something went wrong\nPlease try again"));
}


// Private

QString ToolsWindow::findToolPath(const QString &program) {
    if (const auto path = QStandardPaths::findExecutable(program); !path.isEmpty()) return path;

    QStringList searchPaths;

    #ifdef Q_OS_MAC
        searchPaths << "/opt/homebrew/bin"
                    << "/usr/local/bin"
                    << "/usr/bin"
                    << "/bin"
                    << "/opt/local/bin";
    #elif defined(Q_OS_LINUX)
        searchPaths << "/usr/bin"
                    << "/usr/local/bin"
                    << "/snap/bin"
                    << "/bin";
    #elif defined(Q_OS_WIN)
        searchPaths << "C:/Program Files"
                    << "C:/Program Files (x86)"
                    << QDir::homePath() + "/AppData/Local/Programs";
    #endif

    for (const auto &dirPath : searchPaths) {
        QDirIterator it(dirPath,
                        QDir::Files | QDir::NoSymLinks,
                        QDirIterator::Subdirectories);

        while (it.hasNext()) {
            it.next();
            QFileInfo fi(it.filePath());

            #ifdef Q_OS_WIN
                if (fi.fileName().compare(program, Qt::CaseInsensitive) == 0) return fi.absoluteFilePath();
            #else
                if (fi.fileName() == program) return fi.absoluteFilePath();
            #endif
        }
    }

    return checkExecutable(program);
}

QString ToolsWindow::checkExecutable(const QString &program) {
    QProcess process;
    process.setProgram(program);
    process.setArguments({});

    if (process.startDetached()) return program;

    process.start();
    if (process.waitForStarted(3000)) {
        process.kill();
        process.waitForFinished(3000);
        return program;
    }

    return {};
}
