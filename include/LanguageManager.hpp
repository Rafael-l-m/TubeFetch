#pragma once

#include <QObject>
#include <QTranslator>

class LanguageManager final : public QObject {
    Q_OBJECT

public:
    explicit LanguageManager(QObject* parent = nullptr);

    Q_INVOKABLE QString getLanguage() const;

public slots:
    void setLanguage(const QString& language);

signals:
    void languageChanged();

private:
    QString m_language;
    QTranslator m_translator;
};
