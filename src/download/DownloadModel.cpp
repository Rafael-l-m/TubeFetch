#include "download/DownloadModel.hpp"


// Public

DownloadModel::DownloadModel(QObject* parent) : QAbstractListModel(parent) {}

void DownloadModel::setDownloads(const QSharedPointer<QList<QSharedPointer<Download>>>& downloads) {
    beginResetModel();
    this->m_downloads = downloads;
    endResetModel();
}

int DownloadModel::rowCount(const QModelIndex&) const {
    return this->m_downloads ? static_cast<int>(this->m_downloads->size()) : 0;
}

QVariant DownloadModel::data(const QModelIndex& index, const int role) const {
    if (!index.isValid()) return {};

    const auto d = this->m_downloads->at(index.row());

    switch (role) {
        case InternalIdRole:      return d->getInternalId();
        case TitleRole:           return d->getSavePath().trimmed();
        case ProgressRole:        return d->getProgress();
        case DownloadedBytesRole: return QString::number(d->getDownloadedBytes(), 'f', 2);
        case TotalBytesRole:      return QString::number(d->getTotalBytes(), 'f', 2);
        case SpeedRole:           return QString::number(d->getDownloadSpeed(), 'f', 2);
        case RemainTextRole:      return d->getEta().trimmed();
        case StatusRole:          return downloadStatusToStr(d->getDownloadStatus()).trimmed();
        default:                  return {};
    }
}

QHash<int, QByteArray> DownloadModel::roleNames() const {
    return {
        { InternalIdRole,      "internalId"      },
        { TitleRole,           "title"           },
        { ProgressRole,        "progress"        },
        { DownloadedBytesRole, "downloadedBytes" },
        { TotalBytesRole,      "totalBytes"      },
        { SpeedRole,           "speed"           },
        { RemainTextRole,      "remain"          },
        { StatusRole,          "status"          }
    };
}

void DownloadModel::append(const QSharedPointer<Download>& download) {
    beginInsertRows(QModelIndex(), rowCount({}), rowCount({}));
    this->m_downloads->append(download);
    endInsertRows();
}

void DownloadModel::remove(const int row) {
    beginRemoveRows(QModelIndex(), row, row);
    this->m_downloads->removeAt(row);
    endRemoveRows();
}

void DownloadModel::removeByInternalId(const qint64 internalId) {
    if (!this->m_downloads) { return; }

    const auto row = std::find_if(
        this->m_downloads->cbegin(),
        this->m_downloads->cend(),
        [internalId](const auto& task) {
            return task && task->getInternalId() == internalId;
        }
    );

    if (row == this->m_downloads->cend()) { return; }

    const auto rowIndex = static_cast<int>(std::distance(this->m_downloads->cbegin(), row));

    beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
    this->m_downloads->removeAt(rowIndex);
    endRemoveRows();
}

void DownloadModel::clear() {
    if (!this->m_downloads || this->m_downloads->isEmpty()) { return; }

    beginResetModel();
    this->m_downloads->clear();
    endResetModel();
}

void DownloadModel::refresh(const QSharedPointer<Download>& task) {
    if (!this->m_downloads || !task) { return; }

    const auto row = m_downloads->indexOf(task);

    if (row < 0) { return; }

    const auto modelIndex = index(row);

    emit dataChanged(
        modelIndex,
        modelIndex,
        {
            ProgressRole,
            DownloadedBytesRole,
            TotalBytesRole,
            SpeedRole,
            RemainTextRole,
            StatusRole
        }
    );
}
