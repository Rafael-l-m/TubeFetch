
#include "DownloadInfoWidget.h"
#include "URLManager.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QTimer>


// Public

DownloadInfoWidget::DownloadInfoWidget(QString yt_dlpPath, QString ffmpegPath, QString nodePath, QIcon windowIcon, QWidget *parent) : QDialog(parent), windowIcon(std::move(windowIcon)), yt_dlp_path(std::move(yt_dlpPath)), ffmpeg_path(std::move(ffmpegPath)), node_path(std::move(nodePath)) {
    this->grid = new QGridLayout(this);

    // Timer
    this->debounceTimer1 = new QTimer(this);
    this->debounceTimer1->setInterval(this->waitIntervalForURLInputs);  // Sets the time interval in milliseconds between timer timeouts
    this->debounceTimer1->setSingleShot(true);  // Makes the timer emit the timeout signal only once, then stop automatically

    this->debounceTimer2 = new QTimer(this);
    this->debounceTimer2->setInterval(this->waitIntervalForCodesInput);  // Sets the time interval in milliseconds between timer timeouts
    this->debounceTimer2->setSingleShot(true);  // Makes the timer emit the timeout signal only once, then stop automatically

    // Row 1
    this->urlLabel = new QLabel(tr("URL: "), this);
    this->urlLabel->setFixedSize(this->labelSize.first,this->labelSize.second);
    this->urlInput = new QLineEdit(this);
    this->urlInput->setFixedSize(this->lESize.first, this->lESize.second);
    this->urlInput->setPlaceholderText(tr("Input download address ..."));  // Sets a hint text that appears inside the input field when it is empty

    this->grid->addWidget(this->urlLabel, 0, 0);
    this->grid->addWidget(this->urlInput, 0, 1);

    // Row 2
    this->titleLabel = new QLabel(tr("Title: "), this);
    this->titleLabel->setFixedSize(this->labelSize.first, this->labelSize.second);
    this->titleLE = new QLineEdit(this);
    this->titleLE->setFixedSize(this->lESize.first, this->lESize.second);
    this->titleLE->setEnabled(false);

    this->grid->addWidget(this->titleLabel, 1, 0);
    this->grid->addWidget(this->titleLE, 1, 1);

    // Row 3
    this->typeLabel = new QLabel(tr("Type: "), this);
    this->typeLabel->setFixedSize(this->labelSize.first, this->labelSize.second);

    this->row3 = new QHBoxLayout(this);

    this->combo1 = new QComboBox(this);
    this->combo1->addItem(tr("Best Video"));
    this->combo1->addItem(tr("Best Audio"));
    this->combo1->addItem(tr("Manually"));
    this->combo1->setFixedSize(this->comboSize.first, this->comboSize.second);
    this->combo1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->checkCodesBtn = new QPushButton(tr("Show More Information"), this);
    this->checkCodesBtn->setFixedHeight(this->comboSize.second);
    this->checkCodesBtn->setEnabled(false);

    this->row3->addWidget(this->combo1);
    this->row3->addWidget(this->checkCodesBtn);
    this->row3->setAlignment(Qt::AlignLeft);

    this->grid->addWidget(this->typeLabel, 2, 0);
    this->grid->addLayout(this->row3, 2, 1);

    // Row 4
    this->codeLabel = new QLabel(tr("Code: "), this);
    this->codeLabel->setFixedSize(this->labelSize.first, this->labelSize.second);
    this->codeLabel->setVisible(false);

    this->inputCodeLE = new QLineEdit(this);
    this->inputCodeLE->setFixedSize(this->lESize.first, this->lESize.second);
    this->inputCodeLE->setPlaceholderText(tr("Format: audio ID (Example: 251) / audio ID+video ID (Example: 251+401)"));
    this->inputCodeLE->setVisible(false);

    this->grid->addWidget(this->codeLabel, 3, 0);
    this->grid->addWidget(this->inputCodeLE, 3, 1);

    // Row 5
    this->MDLabel = new QLabel(tr("Metadata: "), this);
    this->MDLabel->setFixedSize(this->labelSize.first, this->labelSize.second);

    this->combo2 = new QComboBox(this);
    this->combo2->addItem(tr("Without Metadata"));
    this->combo2->addItem(tr("With Metadata"));
    this->combo2->setFixedSize(this->comboSize.first, this->comboSize.second);
    this->combo2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->grid->addWidget(this->MDLabel, 4, 0);
    this->grid->addWidget(this->combo2, 4, 1);

    // Row 6
    this->pathLabel = new QLabel(tr("Path: "), this);
    this->pathLabel->setFixedSize(this->labelSize.first, this->labelSize.second);

    this->pathLE = new QLineEdit(this);
    this->pathLE->setReadOnly(true);
    this->pathLE->installEventFilter(this);  // Rewrite EventFilter
    this->pathLE->setFixedSize(this->lESize.first, this->lESize.second);

    this->grid->addWidget(this->pathLabel, 5, 0);
    this->grid->addWidget(this->pathLE, 5, 1);

    // Row 7
    this->confirmBtn = new QPushButton(tr("Confirm"), this);

    this->buttonLayout = new QHBoxLayout;
    this->buttonLayout->addStretch();
    this->buttonLayout->addWidget(this->confirmBtn);
    this->buttonLayout->addStretch();

    this->grid->addLayout(this->buttonLayout, 6, 0, 1, 2);
    this->grid->setColumnStretch(1,0);

    // Unused
    this->downloadItem = new DownloadItem{};
    this->process = new QProcess(this);

    // Others
    this->showCodes = new ShowCodes(tr("Show All Possible Codes"), this->windowIcon);

    this->setWindowTitle(this->windowTitle);
    this->setWindowIcon(this->windowIcon);
    this->setFixedSize(this->windowSize.first, this->windowSize.second);
    this->setStyleSheet(this->mainStyle);

    // Signal and Slot
    connect(this->urlInput, &QLineEdit::textChanged, this, [=, this](const QString&) {this->debounceTimer1->start();});
    connect(debounceTimer1, &QTimer::timeout, this, [=, this]() {handleDebounceTimer1(this->urlInput->text());});
    connect(this->inputCodeLE, &QLineEdit::textChanged, this, [=, this](const QString&) {this->debounceTimer2->start();});
    connect(debounceTimer2, &QTimer::timeout, this, [=, this](){handleDebounceTimer2(this->inputCodeLE->text());});
    connect(this->combo1, &QComboBox::currentIndexChanged, this, &DownloadInfoWidget::Combo1Changed);
    connect(this->checkCodesBtn, &QPushButton::clicked, this, &DownloadInfoWidget::checkCodes);
    connect(this->combo2, &QComboBox::currentIndexChanged, this, &DownloadInfoWidget::Combo2Changed);
    connect(this->confirmBtn, &QPushButton::clicked, this, &DownloadInfoWidget::confirmButton);
}

DownloadItem* DownloadInfoWidget::getURLInfo() const {return this->downloadItem;}

void DownloadInfoWidget::requestEdit(const QString& url, const QString& oldTitle, const QString& nT, const std::pair<int, int>& codes, const bool with_metadata, const QString& fp, const QString& suf) {
    this->normalMode = false;
    this->urlInput->setText(url);
    this->URL = url;
    this->titleLE->setText(oldTitle);
    this->originalTitle = oldTitle;
    this->newTitle = nT;

    if (const auto&[c1, c2] = codes; c1 == 0 && c2 == 0) {
        if (this->supportedAudioTypes.contains(suf)) this->combo1->setCurrentIndex(1);
    }

    else if (c1 == 0) {
        this->combo1->setCurrentIndex(2);
        this->inputCodeLE->setText(QString("%1").arg(c2));
    }

    else if (c2 == 0) {
        this->combo1->setCurrentIndex(2);
        this->inputCodeLE->setText(QString("%1").arg(c1));
    }

    else {
        this->combo1->setCurrentIndex(2);
        this->inputCodeLE->setText(QString("%1+%2").arg(c1).arg(c2));
    }

    this->audioCode = codes.first;
    this->videoCode = codes.second;

    if (with_metadata) this->combo2->setCurrentIndex(1);

    this->withMetadata = with_metadata;

    this->pathLE->setText(fp);
    this->savePath = fp;

    this->suffix = suf;

    this->urlInput->setEnabled(false);
}

QSet<QString> DownloadInfoWidget::getSupportedAudioTypes() const {return this->supportedAudioTypes;}

QSet<QString> DownloadInfoWidget::getSupportedVideoTypes() const {return this->supportedVideoTypes;}


// Protected

bool DownloadInfoWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == this->pathLE) {

        if (event->type() == QEvent::MouseButtonDblClick) {
            this->selectedPath();
            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}


// Private Slots

void DownloadInfoWidget::handleDebounceTimer1(const QString& text) {
    if (this->normalMode) {
        if (!text.isEmpty()) {
            if (URLManager::isValidURL(text)) {
                this->URL = URLManager::cutURL(text);
                this->urlInput->setEnabled(false);
                this->combo1->setEnabled(false);
                this->checkCodesBtn->setEnabled(false);
                this->inputCodeLE->setEnabled(false);
                this->combo2->setEnabled(false);
                this->pathLE->setEnabled(false);
                this->confirmBtn->setEnabled(false);

                sleepForMilliseconds(this->sleepTime);  // Pause

                try {
                    const nlohmann::json infoJson = URLManager::getInfo(this->yt_dlp_path, this->ffmpeg_path, this->URL);

                    if (const auto title = QString::fromStdString(infoJson.value("title", std::string("Failed"))); title == "Failed") {
                        this->originalTitle = tr("Failed");
                        this->titleLE->setText(tr("Failed"));
                    }

                    else {
                        this->originalTitle = title;
                        this->titleLE->setText(title);
                    }
                }

                catch (...) {this->titleLE->setText(tr("Failed"));}

                if (this->normalMode) this->urlInput->setEnabled(true);

                this->combo1->setEnabled(true);
                this->inputCodeLE->setEnabled(true);
                this->combo2->setEnabled(true);
                this->pathLE->setEnabled(true);
                this->confirmBtn->setEnabled(true);

                if (this->combo1->currentText().compare(tr("Manually"), Qt::CaseInsensitive) == 0) this->checkCodesBtn->setEnabled(true);
            }

            else NotificationWindow::warning(tr("Warning"), tr("Invalid URL: %1").arg(text), this);
        }
    }
}

void DownloadInfoWidget::handleDebounceTimer2(const QString& text) {
    if (!text.isEmpty()) {
        static const QRegularExpression re(R"(^\d+(\+\d+)?$)");

        if (re.match(text).hasMatch()) {
            this->audioCode = 0;
            this->videoCode = 0;

            QStringList parts = text.split('+');

            bool ok1, ok2;
            const auto num1 = parts[0].toInt(&ok1);

            if (!ok1 || num1 < 0) {
                NotificationWindow::warning(tr("Warning"), tr("Code %1 not allowed").arg(num1), this);
                return;
            }

            if (!this->mediaCodes.contains(num1)) NotificationWindow::warning(tr("Warning"), tr("%1 is not in the available list").arg(num1), this);

            this->audioCode = num1;

            if (parts.size() == 2) {
                const auto num2 = parts[1].toInt(&ok2);

                if (!ok2 || num2 < 0) {
                    NotificationWindow::warning(tr("Warning"), tr("Code %1 not allowed").arg(num2), this);
                    return;
                }

                if (!this->mediaCodes.contains(num2)) NotificationWindow::warning(tr("Warning"), tr("%1 is not in the available list").arg(num2), this);

                this->videoCode = num2;
            }
        }

        else NotificationWindow::critical(tr("Critical"), tr("Does not meet the requirements: %1").arg(text), this);
    }
}

void DownloadInfoWidget::Combo1Changed() {
    if (this->combo1->currentText().compare(tr("Manually"), Qt::CaseInsensitive) == 0) {
        this->checkCodesBtn->setEnabled(true);
        this->codeLabel->setVisible(true);
        this->inputCodeLE->setVisible(true);
    }

    else {
        this->checkCodesBtn->setEnabled(false);
        this->codeLabel->setVisible(false);
        this->inputCodeLE->setVisible(false);
        this->audioCode = 0;
        this->videoCode = 0;
    }
}

void DownloadInfoWidget::Combo2Changed() {
    if (this->combo2->currentText().compare(tr("With Metadata"), Qt::CaseInsensitive) == 0) this->withMetadata = true;

    else this->withMetadata = false;
}

void DownloadInfoWidget::checkCodes() {
    if (!this->urlInput->text().isEmpty()) {
        if (const auto tmp = this->titleLE->text(); tmp.isEmpty() || tmp == tr("Failed")) {
            NotificationWindow::warning(tr("Warning"), tr("Invalid information from URL: %1").arg(this->URL));
            return;
        }

        this->process = new QProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("PATH", this->node_path + QDir::listSeparator() + env.value("PATH"));

        this->process->setProcessEnvironment(env);
        this->process->setProgram(this->yt_dlp_path);
        this->process->setArguments({"--no-playlist", "-F", this->URL});

        connect(this->process, &QProcess::finished, this->process, &QObject::deleteLater);

        connect(this->process, &QProcess::readyReadStandardOutput, this, [this]() {
            const QString output = this->process->readAllStandardOutput();
            this->showCodes->setText(output);
        });

        connect(this->process, &QProcess::readyReadStandardError, this, [this]() {
            const QString error = this->process->readAllStandardError();
            this->showCodes->setText(error);
        });

        connect(this->showCodes, &QDialog::finished, this->process, &QObject::deleteLater);

        this->process->start();

        this->showCodes->exec();
    }

    else NotificationWindow::critical(tr("Failed"), tr("No valid URL provided"));
}

void DownloadInfoWidget::confirmButton() {
    if (this->urlInput->text().isEmpty() || this->titleLE->text().isEmpty() || this->titleLE->text().compare(tr("Failed"), Qt::CaseInsensitive) == 0) {
        NotificationWindow::warning(tr("Warning"), tr("A valid URL is required first"), this);
        return;
    }

    if (this->combo1->currentText().compare(tr("Manually"), Qt::CaseInsensitive) == 0 && this->audioCode == 0) {
        NotificationWindow::warning(tr("Warning"), tr("If you choose Manually, you have to provide at least an audio code"), this);
        return;
    }

    if (this->pathLE->text().isEmpty()) {
        NotificationWindow::warning(tr("Warning"), tr("Have to provide a save path first"), this);
        return;
    }

    if (this->combo2->isEnabled()) {
        NotificationWindow::warning(tr("Warning"), tr("Have to change the suffix in the save path"), this);
        return;
    }

    this->downloadItem = new DownloadItem {-1, this->URL, this->audioCode, this->videoCode, this->savePath, this->suffix, this->originalTitle, this->newTitle, 0, false, this->withMetadata};

    accept();
}


// Private

void DownloadInfoWidget::selectedPath() {
    bool temp = true;

    if (this->urlInput->text().isEmpty() || this->titleLE->text().isEmpty() || this->titleLE->text().compare(tr("Failed"), Qt::CaseInsensitive) == 0) {
        NotificationWindow::warning(tr("Warning"), tr("A valid URL is required first"), this);
        return;
    }

    if (this->combo1->currentText().compare(tr("Manually"), Qt::CaseInsensitive) == 0 && (this->audioCode == 0 || (this->audioCode == 0 && this->videoCode == this->audioCode))) {
        NotificationWindow::warning(tr("Warning"), tr("Code is required first (Audio_Code / Audio_Code+Video_Code)"), this);
        return;
    }

    this->urlInput->setEnabled(false);
    this->combo1->setEnabled(false);
    this->checkCodesBtn->setEnabled(false);
    this->inputCodeLE->setEnabled(false);
    this->combo2->setEnabled(false);

    if (const auto cT = this->combo1->currentText(); cT.compare(tr("Best Audio"), Qt::CaseInsensitive) == 0) temp = false;

    else {if (cT.compare(tr("Manually"), Qt::CaseInsensitive) == 0 && this->videoCode == 0) temp = false;}

    QString filePath;
    QString selectedFilter;

    QFileDialog dialog {
        this,
        tr("Save As"),
        QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QDir::separator() + this->titleLE->text(),
        temp ? this->filterVideo : this->filterAudio,
    };

    #ifdef Q_OS_MAC
        dialog.setOptions(QFileDialog::DontUseNativeDialog);
    #endif

    dialog.setFilter(dialog.filter() | QDir::Hidden);

    if (dialog.exec() == Accepted) {
        filePath = dialog.selectedFiles().first();
        selectedFilter = dialog.selectedNameFilter();
    }

    if (!filePath.isEmpty()) {
        this->savePath = filePath;
        this->pathLE->setText(filePath);

        const QFileInfo inf(filePath);
        this->newTitle = inf.fileName();
    }

    else NotificationWindow::warning(tr("Warning"), tr("No valid file path provided"), this);

    static const QRegularExpression suffixRe(R"(\*\.(\w+))");

    if (const QRegularExpressionMatch match = suffixRe.match(selectedFilter); match.hasMatch()) this->suffix = match.captured(1);

    if (this->suffix.isEmpty()) {
        static const QRegularExpression re(R"(\.(\w+)$)");

        if (const QRegularExpressionMatch match = re.match(this->savePath); match.hasMatch()) this->suffix = match.captured(1);

        else this->suffix = "mkv";
    }

    this->newTitle = this->newTitle + "." + this->suffix;
    this->savePath = this->savePath + "." + this->suffix;
    this->pathLE->setText(this->savePath);
}

void DownloadInfoWidget::sleepForMilliseconds(const int msecs) {
    QEventLoop loop;
    QTimer::singleShot(msecs, &loop, &QEventLoop::quit);
    loop.exec();
}
