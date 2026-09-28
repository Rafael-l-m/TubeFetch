#include <core/APP.hpp>
#include <provider/ProviderManager.hpp>
#include <QFileInfo>
#include <QNetworkReply>
#include <QThread>
#include <QTimer>


// Public

ProviderManager::ProviderManager(QObject* parent) : QObject(parent) {
    this->process = new QProcess(this);
    this->networkManager = new QNetworkAccessManager(this);

    connect(this->process, &QProcess::readyReadStandardOutput, this, [this](){
        APP::messageCenter()->sendOutput(QString("[provider] %1").arg(process->readAllStandardOutput()));
    });

    connect(this->process, &QProcess::readyReadStandardError, this, [this](){
        APP::messageCenter()->sendError(QString("[provider-err] %1").arg(process->readAllStandardError()));
    });

    connect(process, &QProcess::errorOccurred, this, [this](const QProcess::ProcessError& error) {
        APP::messageCenter()->sendError(QString("Provider process error: %1").arg(static_cast<int>(error)));}
    );

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this](const int exitCode, const QProcess::ExitStatus& exitStatus) {
        APP::messageCenter()->sendDebug(QString("Provider server exited. \tCode=%1, Status=%2").arg(exitCode).arg(static_cast<int>(exitStatus)));}
    );
}

ProviderManager::~ProviderManager() { this->stopProvider(); }

qint64 ProviderManager::startProvider(const QString& exePath, const QStringList& args) {
    const auto exePathTrimmed = exePath.trimmed();

    if (exePathTrimmed.isEmpty()) {
        APP::messageCenter()->sendWarning("Provider executable path is empty");
        emit providerStarted(false);
        return 0;
    }

    const QFileInfo fi(exePathTrimmed);

    if (!fi.exists() || !fi.isFile()) {
        APP::messageCenter()->sendWarning(QString("Provider executable does not exist: %1").arg(exePathTrimmed));
        emit providerStarted(false);
        return -1;
    }

    if (this->process->state() != QProcess::NotRunning) {
        APP::messageCenter()->sendWarning("Provider server is already running");
        emit providerStarted(false);
        return ProviderPort;
    }

    this->process->start(exePathTrimmed, args);

    if (!process->waitForStarted(5000)) {
        APP::messageCenter()->sendError(QString("Failed to start provider server: %1").arg(this->process->errorString()));
        emit providerStarted(false);
        return -2;
    }

    APP::messageCenter()->sendDebug("Waiting for provider server ...");

    if (!this->waitForProviderReady()) {
        APP::messageCenter()->sendError("Provider server failed health check");
        this->stopProvider();
        emit providerStarted(false);
        return -3;
    }

    APP::messageCenter()->sendDebug(QString("Provider server started on port: %1").arg(ProviderPort));

    emit providerStarted(true);

    return ProviderPort;
}

void ProviderManager::stopProvider() {
    if (!this->process) { return; }

    if (this->process->state() == QProcess::NotRunning) { return; }

    APP::messageCenter()->sendDebug("Stopping provider server ...");

    this->process->terminate();

    if (!this->process->waitForFinished(3000)) {
        APP::messageCenter()->sendWarning("Provider did not terminate gracefully, killing ...");

        this->process->kill();
        this->process->waitForFinished(3000);
    }

    APP::messageCenter()->sendDebug("Provider server stopped");
}


// Private

bool ProviderManager::waitForProviderReady(int timeoutMs) {
    const QUrl url(QString("http://127.0.0.1:%1/ping").arg(ProviderPort));

    const auto deadline = QDateTime::currentMSecsSinceEpoch() + timeoutMs;

    while (QDateTime::currentMSecsSinceEpoch() < deadline) {
        QNetworkRequest req(url);

        auto* reply = this->networkManager->get(req);

        QEventLoop loop;
        QTimer timer;

        timer.setSingleShot(true);

        connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        timer.start(500);
        loop.exec();

        const auto success = reply->error() == QNetworkReply::NoError;
        const auto statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        reply->deleteLater();

        if (success && statusCode >= 200 && statusCode < 300) { return true; }

        if (process->state() == QProcess::NotRunning) {
            APP::messageCenter()->sendError("Provider process exited before coming ready");
            return false;
        }

        QThread::msleep(100);
    }

    return false;
}
