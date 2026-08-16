
#include "MainWindow.h"
#include "ToolsWindow.h"
#include <QMenuBar>
#include <utility>


// Public

MW::MW(const QString& dataBasePath, QString defaultTranslaterKey, QString defaultOrganization, QString defaultAppName, const QString& ico_path, QString yt_dlp_path, QString ffmpeg_path, QString node_path, QWidget* parent) : QMainWindow(parent), defaultTranslatorKey(std::move(defaultTranslaterKey)), defaultOrganization(std::move(defaultOrganization)), defaultAppName(std::move(defaultAppName)) ,windowIcon(QIcon(ico_path)), yt_dlp_path(std::move(yt_dlp_path)), ffmpeg_path(std::move(ffmpeg_path)), node_path(std::move(node_path)) {
    // MenuBar
    this->bar = this->menuBar();

    this->languageMenu = this->bar->addMenu(tr("Language"));
    this->englishAction = new QAction(tr("English"), this);
    this->chineseAction = new QAction(tr("Chinese"), this);
    this->spanishAction = new QAction(tr("Spanish"), this);
    this->languageMenu->addAction(this->englishAction);
    this->languageMenu->addAction(this->chineseAction);
    this->languageMenu->addAction(this->spanishAction);

    this->toolMenu = this->bar->addMenu(tr("Tools"));
    this->toolConfigAction = new QAction(tr("Tools Configuration"), this);
    this->toolMenu->addAction(this->toolConfigAction);

    // Widgets
    this->central = new QWidget(this);
    this->mainLayout = new QVBoxLayout(this->central);

    // Row 1
    this->topButtonLayout = new QHBoxLayout(this);

    this->startBtn = new QPushButton(tr("Start"));
    this->stopBtn = new QPushButton(tr("Stop"));

    this->topButtonLayout->addWidget(this->startBtn);
    this->topButtonLayout->addWidget(this->stopBtn);
    this->topButtonLayout->setSpacing(this->spacingButtonLayout);
    this->topButtonLayout->setContentsMargins(this->contentWidgetsButtonLayout[0], this->contentWidgetsButtonLayout[1], this->contentWidgetsButtonLayout[2], this->contentWidgetsButtonLayout[3]);

    this->mainLayout->addLayout(this->topButtonLayout);

    // Row 2
    this->scrollArea = new QScrollArea(this);
    this->scrollArea->setWidgetResizable(true);

    this->scrollContent = new QWidget(this);
    this->scrollLayout = new QVBoxLayout(this->scrollContent);
    this->scrollLayout->setAlignment(Qt::AlignTop);
    this->scrollContent->setLayout(this->scrollLayout);
    this->scrollArea->setWidget(this->scrollContent);

    this->mainLayout->addWidget(this->scrollArea);

    // Row 3
    this->bottomButtonLayout = new QHBoxLayout(this);

    this->addBtn = new QPushButton(tr("Add new download"));
    this->removeBtn = new QPushButton(tr("Remove all downloads"));

    this->bottomButtonLayout->addWidget(this->addBtn);
    this->bottomButtonLayout->addWidget(this->removeBtn);
    this->bottomButtonLayout->setSpacing(this->spacingButtonLayout);
    this->bottomButtonLayout->setContentsMargins(contentWidgetsButtonLayout[0], contentWidgetsButtonLayout[1], contentWidgetsButtonLayout[2], contentWidgetsButtonLayout[3]);

    this->mainLayout->addLayout(this->bottomButtonLayout);

    this->setCentralWidget(this->central);
    this->setWindowTitle(this->windowTitle);
    this->setWindowIcon(this->windowIcon);
    this->resize(this->windowSize.first, this->windowSize.second);
    this->setStyleSheet(this->mainStyle);

    this->dm = new DownloadManager(this->yt_dlp_path, this->ffmpeg_path, this->node_path, dataBasePath);

    // Restore the data stored in the database if exists
    this->restoreData();

    // Signals and slots
    connect(this->addBtn, &QPushButton::clicked, this, &MW::addTask);
    connect(this->removeBtn, &QPushButton::clicked, this, &MW::removeAllTasks);
    connect(this->startBtn, &QPushButton::clicked, this, &MW::handleStartRequest);
    connect(this->stopBtn, &QPushButton::clicked, this, &MW::handleStopRequest);
    connect(this->dm, &DownloadManager::finishedDownload, [this]() {
        this->startBtn->setEnabled(true);
        this->removeBtn->setEnabled(true);
    });
    connect(this->dm, &DownloadManager::renewWidgets, this, [this](QPair<int, QList<DownloadItemWidget *>> data) {
        const auto [first, second] = std::move(data);

        clearLayout(this->scrollLayout);

        for (auto* dwWidget : second) {
            connect(dwWidget, &DownloadItemWidget::requestEdit, this, &MW::handleEditRequest);
            connect(dwWidget, &DownloadItemWidget::requestDelete, this, &MW::handleDeleteRequest);
            this->scrollLayout->addWidget(dwWidget);
        }
    });
    connect(this->englishAction, &QAction::triggered, [this]() {this->changeLanguage("en_US.qm");});
    connect(this->chineseAction, &QAction::triggered, [this]() {this->changeLanguage("zh_CN.qm");});
    connect(this->spanishAction, &QAction::triggered, [this]() {this->changeLanguage("es_ES.qm");});
    connect(this->toolConfigAction, &QAction::triggered, this, &MW::toolConfigActionManager);
}


// Private Slots

void MW::changeLanguage(const QString& newLanguage) {
    QMap<QString, QString> tmp;
    tmp.insert(this->defaultTranslatorKey, newLanguage);

    if (ToolsWindow::writeToolPath(tmp, this->defaultOrganization, this->defaultAppName)) {
        this->m_isRestarting = true;
        emit restartAppRequest();
    }

    else NotificationWindow::warning(tr("Critical"), tr("Failed to change language: %1\nPlease contact with the developer").arg(newLanguage));
}

void MW::addTask() {
    if (auto* dw = new DownloadInfoWidget{this->yt_dlp_path, this->ffmpeg_path, this->node_path, this->windowIcon}; dw->exec() == QDialog::Accepted) {
        const auto* info = dw->getURLInfo();

        auto* dwWidget = new DownloadItemWidget {id, info->oldTitle, info->newTitle};

        if (const auto response = this->dm->addDownload(id, info, dwWidget); !response) {
            NotificationWindow::warning(tr("Warning"), tr("Info failed to save: %1. Please try again").arg(info->url), this);
            return;
        }

        connect(dwWidget, &DownloadItemWidget::requestEdit, this, &MW::handleEditRequest);
        connect(dwWidget, &DownloadItemWidget::requestDelete, this, &MW::handleDeleteRequest);

        this->scrollLayout->addWidget(dwWidget);

        ++id;

        delete dw;
    }
}

void MW::removeAllTasks() const {
    if (this->dm->haveAvailableWidget()) {
        if (NotificationWindow::confirmation(tr("Confirmation"), tr("Are you sure you want to delete all of them?")) == QDialog::Accepted) {
            if (this->dm->removeAllDownloads()) clearLayout(this->scrollLayout);
        }
    }

    else NotificationWindow::warning(tr("Warning"), tr("There are no available widgets"));
}

void MW::handleEditRequest() const {
    const auto* widget = qobject_cast<DownloadItemWidget*>(sender());

    const auto tmp_id = widget->getId();

    const auto* tmp_download = this->dm->readDownload(tmp_id);

    auto* tmp = new DownloadInfoWidget {this->yt_dlp_path, this->ffmpeg_path, this->node_path, this->windowIcon};

    tmp->requestEdit(tmp_download->url, tmp_download->oldTitle, tmp_download->newTitle, std::pair{tmp_download->audio_code, tmp_download->video_code}, tmp_download->withMetadata, tmp_download->file_path, tmp_download->suffix);

    if (tmp->exec() == QDialog::Accepted) {
        const auto newInfo = tmp->getURLInfo();

        const auto f1 = this->dm->updateAudioCode(tmp_id, newInfo->audio_code);
        const auto f2 = this->dm->updateVideoCode(tmp_id, newInfo->video_code);
        const auto f3 = this->dm->updateIfMetadata(tmp_id, newInfo->withMetadata);
        const auto f4 = this->dm->updateFilePath(tmp_id, newInfo->file_path);
        const auto f5 = this->dm->updateSuffix(tmp_id, newInfo->suffix);
        const auto f6 = this->dm->updateNewTitle(tmp_id, newInfo->newTitle);

        if (f1 && f2 && f3 && f4 && f5 && f6) NotificationWindow::information(tr("Success"), tr("New Information updated successfully"));

        else NotificationWindow::critical(tr("Failed"), tr("We can not update the new information\nPlease try later"));

        widget->setFileName(newInfo->newTitle);

        delete newInfo;
    }
}

void MW::handleDeleteRequest() {
    auto* widget = qobject_cast<DownloadItemWidget*>(sender());

    if (!widget) {
        NotificationWindow::warning(tr("Warning"), tr("Can not find the widget"), this);
        return;
    }

    const auto tmp = widget->getId();

    if (const auto response = this->dm->removeDownload(tmp); !response) {
        NotificationWindow::warning(tr("Warning"), tr("Can not find ID: %1 from database").arg(tmp), this);
        return;
    }

    this->scrollLayout->removeWidget(widget);

    widget->deleteLater();
}

void MW::handleStartRequest() const {
    if (this->dm->haveAvailableDownload()) {
        this->startBtn->setEnabled(false);
        this->removeBtn->setEnabled(false);
        this->dm->startNextDownload();
    }

    else NotificationWindow::warning(tr("Warning"), tr("There are no available downloads"));
}

void MW::handleStopRequest() {
    if (this->dm->haveActiveProcesses()) {
        if (const auto flag = NotificationWindow::confirmation(tr("Stop Process"), tr("Do you wanna stop all processes?")); flag) {
            this->dm->stopAll();
            this->m_isRestarting = true;
            emit restartAppRequest();
        }
    }

    else NotificationWindow::warning(tr("Warning"), tr("There are no downloads in progress"));
}

void MW::toolConfigActionManager() {
    auto* t = new ToolsWindow{this->windowIcon, this};
    t->exec();
}


// Private

void MW::restoreData() {
    const auto [first, second] = this->dm->restoreData();

    id = first;

    for (auto* dwWidget : second) {
        connect(dwWidget, &DownloadItemWidget::requestEdit, this, &MW::handleEditRequest);
        connect(dwWidget, &DownloadItemWidget::requestDelete, this, &MW::handleDeleteRequest);
        this->scrollLayout->addWidget(dwWidget);
    }
}

void MW::clearLayout(QLayout* layout) {
    if (!layout) return;

    QLayoutItem* item;

    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();

        else if (item->layout()) clearLayout(item->layout());

        delete item;
    }
}
