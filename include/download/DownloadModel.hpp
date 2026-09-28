#pragma once

#include <download/Download.hpp>
#include <QAbstractListModel>

class DownloadModel final : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        InternalIdRole,
        TitleRole,
        ProgressRole,
        DownloadedBytesRole,
        TotalBytesRole,
        SpeedRole,
        RemainTextRole,
        StatusRole
    };

    explicit DownloadModel(QObject* parent = nullptr);

    void setDownloads(const QSharedPointer<QList<QSharedPointer<Download>>>& downloads);

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void append(const QSharedPointer<Download>& download);

    void remove(int row);

    void removeByInternalId(qint64 internalId);

    void clear();

    void refresh(const QSharedPointer<Download>& task);

private:
    QSharedPointer<QList<QSharedPointer<Download>>> m_downloads;
};
