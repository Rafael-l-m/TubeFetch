
#include "NotificationWindow.h"
#include <QPushButton>


// Public

bool NotificationWindow::confirmation(const QString& title, const QString& inform, QWidget* parent) {
    const auto EXECUTABLE_PATH = QCoreApplication::applicationDirPath();

    #ifdef Q_OS_MAC
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/../Resources/icons";
    #elif defined(Q_OS_WIN)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #elif defined(Q_OS_LINUX)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #endif

    const auto ICON_PATH = RESOURCES_PATH + "/Video.png";
    const auto CONFIRMATION_PATH = RESOURCES_PATH + "/Confirmation.png";

    NotificationWindow box(title, inform, NotificationType::CONFIRMATION, ICON_PATH, CONFIRMATION_PATH, parent);
    return box.exec() == Accepted;
}

void NotificationWindow::information(const QString& title, const QString& inform, QWidget* parent) {
    const auto EXECUTABLE_PATH = QCoreApplication::applicationDirPath();

    #ifdef Q_OS_MAC
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/../Resources/icons";
    #elif defined(Q_OS_WIN)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #elif defined(Q_OS_LINUX)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #endif

    const auto ICON_PATH = RESOURCES_PATH + "/Video.png";
    const auto INFORMATION_PATH = RESOURCES_PATH + "/Information.png";

    NotificationWindow box(title, inform, NotificationType::INFORMATION, ICON_PATH, INFORMATION_PATH, parent);
    box.exec();
}

void NotificationWindow::warning(const QString& title, const QString& inform, QWidget* parent) {
    const auto EXECUTABLE_PATH = QCoreApplication::applicationDirPath();

    #ifdef Q_OS_MAC
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/../Resources/icons";
    #elif defined(Q_OS_WIN)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #elif defined(Q_OS_LINUX)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #endif

    const auto ICON_PATH = RESOURCES_PATH + "/Video.png";
    const auto WARNING_PATH = RESOURCES_PATH + "/Warning.png";

    NotificationWindow box(title, inform, NotificationType::WARNING, ICON_PATH, WARNING_PATH, parent);
    box.exec();
}

void NotificationWindow::critical(const QString& title, const QString& inform, QWidget* parent) {
    const auto EXECUTABLE_PATH = QCoreApplication::applicationDirPath();

    #ifdef Q_OS_MAC
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/../Resources/icons";
    #elif defined(Q_OS_WIN)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #elif defined(Q_OS_LINUX)
        const auto RESOURCES_PATH = EXECUTABLE_PATH + "/resources/icons";
    #endif

    const auto ICON_PATH = RESOURCES_PATH + "/Video.png";
    const auto CRITICAL_PATH = RESOURCES_PATH + "/Critical.png";

    NotificationWindow box(title, inform, NotificationType::CRITICAL, ICON_PATH, CRITICAL_PATH, parent);
    box.exec();
}


// Private

NotificationWindow::NotificationWindow(QString title, QString info, const NotificationType type, const QString& windowIconPath, const QString& showIconPath, QWidget* parent) : QDialog(parent), title(std::move(title)), windowIcon(QIcon(windowIconPath)), showIcon(QIcon(showIconPath)), info(std::move(info)), type(type), mainLayout(nullptr) {
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    this->setWindowTitle(this->title);

    if (!this->windowIcon.isNull()) this->setWindowIcon(this->windowIcon);

    this->mainLayout = new QVBoxLayout;
    this->infoLayout = new QHBoxLayout;

    if (!this->showIcon.isNull()) {
        this->iconLabel = new QLabel(this);
        this->iconLabel->setPixmap(this->showIcon.pixmap(this->showIconWidth, this->showIconHeight));
        this->infoLayout->addWidget(this->iconLabel);
    }

    this->textLabel = new QLabel(this->info, this);
    this->textLabel->setWordWrap(true);  // Automatic line break
    this->infoLayout->addWidget(this->textLabel);
    this->mainLayout->addLayout(this->infoLayout);

    this->buttonLayout = new QHBoxLayout;
    this->buttonLayout->addItem(new QSpacerItem(this->spacerWidth, this->spacerHeight, QSizePolicy::Expanding, QSizePolicy::Minimum));

    if (this->type == NotificationType::CONFIRMATION) {
        this->acceptBtn = new QPushButton(tr("Yes"), this);
        this->rejectBtn = new QPushButton(tr("No"), this);
        this->buttonLayout->addWidget(this->acceptBtn);
        this->buttonLayout->addWidget(this->rejectBtn);

        connect(this->acceptBtn, &QPushButton::clicked, this, &NotificationWindow::accept);
        connect(this->rejectBtn, &QPushButton::clicked, this, &NotificationWindow::reject);
    }

    else {
        this->acceptBtn = new QPushButton(tr("Yes"), this);
        this->buttonLayout->addWidget(this->acceptBtn);

        connect(this->acceptBtn, &QPushButton::clicked, this, &NotificationWindow::accept);
    }

    this->mainLayout->addLayout(this->buttonLayout);

    this->setModal(true);  // modal window: disable interaction with the main window
    this->setLayout(this->mainLayout);
    this->setFixedSize(this->QDialog::sizeHint());  // Can not resize
    this->setStyleSheet(this->style);
}
