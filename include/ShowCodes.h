
#ifndef YOUTUBEMEDIADOWNLOADER_SHOWCODES_H
#define YOUTUBEMEDIADOWNLOADER_SHOWCODES_H

#include <QDialog>
#include <QPlainTextEdit>

class ShowCodes final : public QDialog {
    Q_OBJECT
public:
    explicit ShowCodes(const QString& title = "", const QIcon& ico = QIcon(""), QWidget *parent = nullptr);

    void setText(const QString& text) const;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    const std::pair<int, int> windowSize {1000, 450};

    QPlainTextEdit* textEdit;

    const QString mainStyle = R"(
        QPlainTextEdit {
            background-color: qlineargradient(
                x1: 0, y1: 0, x2: 1, y2: 1,
                stop: 0 #a3d5f7,
                stop: 0.5 #c0e0f0,
                stop: 1 #a3d5f7
            );
            font-size: 14px;
            color: #4B0082;
        }
    )";
};

#endif //YOUTUBEMEDIADOWNLOADER_SHOWCODES_H
