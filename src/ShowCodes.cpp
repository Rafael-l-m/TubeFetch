
#include "ShowCodes.h"


// Public

ShowCodes::ShowCodes(const QString& title, const QIcon& ico, QWidget *parent) : QDialog(parent) {
    this->textEdit = new QPlainTextEdit(this);
    this->textEdit->setGeometry(this->rect());  // Original size

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);  // Simular CMD
    font.setBold(true);
    this->textEdit->setFont(font);

    this->setFixedSize(this->windowSize.first, this->windowSize.second);
    this->setWindowTitle(title);
    this->setWindowIcon(ico);

    this->setStyleSheet(this->mainStyle);
}

void ShowCodes::setText(const QString& text) const {this->textEdit->setPlainText(text);}


// Protected

void ShowCodes::resizeEvent(QResizeEvent* event) {
    this->textEdit->setGeometry(this->rect());
    QDialog::resizeEvent(event);
}
