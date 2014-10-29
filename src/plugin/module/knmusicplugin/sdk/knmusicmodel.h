/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QPixmap>
#include <QStringList>

#include "knmusicglobal.h"

#include <QStandardItemModel>

using namespace KNMusic;

class KNMusicSearcher;
class KNMusicAnalysisCache;
class KNMusicModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);
    ~KNMusicModel();
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent);
    qint64 totalDuration() const;
    QString filePathFromRow(const int &row);
    QString filePathFromIndex(const QModelIndex &index);
    QModelIndexList indexFromFilePath(const QString &filePath);
    QString itemText(const int &row, const int &column) const;
    QVariant roleData(int row, int column, int role) const;
    void setRoleData(const int &row,
                     const int &column,
                     const int &role,
                     const QVariant &value);
    QList<QStandardItem *> songRow(const int &row) const;
    QVariant rowProperty(const int &row, const int &propertyRole);
    virtual QPixmap songAlbumArt(const int &row);
    qint64 songDuration(const int &row);
    virtual int playingItemColumn();

signals:
    void rowCountChanged();
    void requireAnalysisFiles(QStringList urls);

public slots:
    virtual void addFiles(const QStringList &fileList);
    virtual void appendDragMusicRows();
    virtual void appendMusicRow(const QList<QStandardItem *> &musicRow);
    virtual void removeMusicRow(const int &row);
    virtual void clearMusicRow();

protected:
    virtual void blockAddFile(const QString &filePath);

private:
    KNMusicSearcher *m_searcher;
    KNMusicAnalysisCache *m_analysisCache;
    KNMusicGlobal *m_musicGlobal;
    qint64 m_totalDuration=0;
};

#endif // KNMUSICMODEL_H
