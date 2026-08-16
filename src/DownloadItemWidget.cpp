
#include "DownloadItemWidget.h"
#include <QPropertyAnimation>
#include <QScrollBar>


// Public

DownloadItemWidget::DownloadItemWidget(const int id, const QString& title, const QString& rename, const int progressValue, QWidget* parent) : QWidget(parent), id(id) {
    this->grid = new QGridLayout(this);

    this->titleLab = new QLabel(title, this);
    this->titleLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);  // Fixed height, preferred width
    this->titleLab->setMinimumSize(std::get<0>(this->labSize), std::get<1>(this->labSize));  // Set min size
    this->titleLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // Left-aligned text, vertically centered
    this->titleLab->setWordWrap(false);  // Disable automatic line break

    this->titleSc = new QScrollArea(this);
    this->titleSc->setWidget(this->titleLab);
    this->titleSc->setWidgetResizable(false);  // Disable automatic scaling to fill scrollable region
    this->titleSc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable horizontal scrolling permanently
    this->titleSc->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable vertical scrolling permanently
    this->titleSc->setFixedSize(std::get<0>(this->scrollBarSize), std::get<1>(this->scrollBarSize));  // Set default size
    this->titleSc->setFrameShape(QFrame::NoFrame);  // No Border
    this->titleSc->horizontalScrollBar()->setStyleSheet(this->scrollBarStyle);  // Set style

    this->filenameLab = new QLabel(rename, this);
    this->filenameLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);  // Fixed height, preferred width
    this->filenameLab->setMinimumSize(std::get<0>(this->labSize), std::get<1>(this->labSize));  // Set min size
    this->filenameLab->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // Left-aligned text, vertically centered
    this->filenameLab->setWordWrap(false);  // Disable automatic line break

    this->filenameSc = new QScrollArea(this);
    this->filenameSc->setWidget(this->filenameLab);
    this->filenameSc->setWidgetResizable(false);  // Disable automatic scaling to fill scrollable region
    this->filenameSc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable horizontal scrolling permanently
    this->filenameSc->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // Disable vertical scrolling permanently
    this->filenameSc->setFixedSize(std::get<0>(this->scrollBarSize), std::get<1>(this->scrollBarSize));  // Set default size
    this->filenameSc->setFrameShape(QFrame::NoFrame);  // No Border
    this->filenameSc->horizontalScrollBar()->setStyleSheet(this->scrollBarStyle);  // Set style

    this->progress = new QProgressBar(this);
    this->progress->setRange(0, 1000);
    this->progress->setValue(progressValue);  // Set default value
    this->progress->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);  // Resizable horizontally, fixed height vertically
    this->progress->setTextVisible(false);  // No visible text

    this->modifyBtn = new QPushButton(tr("Modify"), this);
    this->deleteBtn = new QPushButton(tr("Delete"), this);

    this->grid->addWidget(this->titleSc, 0, 0);
    this->grid->addWidget(this->filenameSc, 0, 1);
    this->grid->addWidget(this->progress, 0, 2);
    this->grid->addWidget(this->modifyBtn, 0, 3);
    this->grid->addWidget(this->deleteBtn, 0, 4);

    this->setFixedHeight(this->heightQWidget);
    this->setStyleSheet(this->mainStyle);

    // Signals and slots
    connect(this->modifyBtn, &QPushButton::clicked, this, [this]() {emit requestEdit(this);});
    connect(this->deleteBtn, &QPushButton::clicked, this, [this](){emit requestDelete(this);});
}

int DownloadItemWidget::getId() const {return this->id;}

int DownloadItemWidget::getProgress() const {return this->progress->value();}

void DownloadItemWidget::setFileName(const QString& fn) const {this->filenameLab->setText(fn);}

void DownloadItemWidget::setProgress(const int percent) const {
    auto* animation = new QPropertyAnimation(this->progress, "value");
    animation->setDuration(800);
    animation->setStartValue(this->progress->value());
    animation->setEndValue(percent);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void DownloadItemWidget::enabledW() const {
    this->modifyBtn->setEnabled(true);
    this->deleteBtn->setEnabled(true);
}

void DownloadItemWidget::disabledW() const {
    this->modifyBtn->setEnabled(false);
    this->deleteBtn->setEnabled(false);
}
