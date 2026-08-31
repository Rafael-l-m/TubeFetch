#include <core/APP.hpp>
#include <core/BasicTools.hpp>
#include <LanguageManager.hpp>


// Public

LanguageManager::LanguageManager(QObject* parent) : QObject(parent) {}

QString LanguageManager::getLanguage() const { return this->m_language; }


// Public Slots

void LanguageManager::setLanguage(const QString& newLanguage) {
    const auto _newLanguage = newLanguage.trimmed();

    if (this->m_language.compare(_newLanguage, Qt::CaseInsensitive) == 0) {
        APP::messageCenter()->sendWarning("Target language is the same as current language");
        return;
    }

    qApp->removeTranslator(&this->m_translator);

    #ifdef Q_OS_MACOS
        const auto translationPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../Resources/translations");
    #else
        const auto translationPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/resources/translations");
    #endif

    const auto _fileName = _newLanguage + ".qm";
    const auto fp = translationPath  + "/" + _fileName;

    APP::messageCenter()->sendDebug(QString("New language file: %1").arg(_fileName));

    if (this->m_translator.load(fp)) {
        APP::messageCenter()->sendDebug("New language successfully loaded");

        qApp->installTranslator(&this->m_translator);

        this->m_language = _newLanguage;

        CONFIG::writeConfig( { {"languages", _newLanguage} } );

        emit languageChanged();
    }

    else { APP::messageCenter()->sendError(QString("New language load failed: %1").arg(_fileName)); }
}
