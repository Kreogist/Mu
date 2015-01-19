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
class KNMusicAnalysisExtend;
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
    KNMusicDetailInfo detailInfoFromRow(const int &row);
    inline QString filePathFromRow(const int &row)
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //Return the file path role data.
        return data(index(row, Name), FilePathRole).toString();
    }
    inline QString itemText(const int &row, const int &column) const
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for text easy access.
        return data(index(row, column), Qt::DisplayRole).toString();
    }
    virtual void updateItemText(const int &row, const int &column, const QString &text)
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for text easy access, only update the different text.
        if(itemText(row, column)!=text)
        {
            qDebug("Here");
            setData(index(row, column), text, Qt::DisplayRole);
        }
    }
    virtual void setItemText(const int &row, const int &column, const QString &text)
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for text easy access.
        setData(index(row, column), text, Qt::DisplayRole);
    }
    inline QVariant roleData(int row, int column, int role) const
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for easy access.
        return data(index(row, column), role);
    }
    inline void updateRoleData(const int &row,
                               const int &column,
                               const int &role,
                               const QVariant &value)
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for easy access.
        if(data(index(row, column), role)!=value)
        {
            setData(index(row, column), value, role);
        }
    }
    inline void setRoleData(const int &row,
                            const int &column,
                            const int &role,
                            const QVariant &value)
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for easy access.
        setData(index(row, column), value, role);
    }
    inline QList<QStandardItem *> songRow(const int &row) const
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //Get all the items, add to a list.
        QList<QStandardItem *> songRowData;
        for(int i=0; i<MusicDataCount; i++)
        {
            //Using clone to do data copy.
            songRowData.append(item(row, i)->clone());
        }
        return songRowData;
    }
    inline QVariant rowProperty(const int &row, const int &propertyRole)
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //All the property of a song is stored in the first item.
        return roleData(row, 0, propertyRole);
    }
    inline virtual void updateRowProperty(const int &row,
                                          const int &propertyRole,
                                          const QVariant &value)
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //All the property of a song is stored in the first item.
        if(value!=data(index(row, 0)))
        {
            setData(index(row, 0), value, propertyRole);
        }
    }
    inline virtual void setRowProperty(const int &row,
                                       const int &propertyRole,
                                       const QVariant &value)
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //All the property of a song is stored in the first item.
        setData(index(row, 0), value, propertyRole);
    }

    virtual QPixmap songAlbumArt(const int &row);
    qint64 songDuration(const int &row);
    virtual int playingItemColumn();

signals:
    void rowCountChanged();
    void requireAnalysisFiles(QStringList urls);

public slots:
    virtual void addFiles(const QStringList &fileList);
    virtual void appendMusicRow(const QList<QStandardItem *> &musicRow);
    virtual void updateMusicRow(const int &row,
                                const KNMusicDetailInfo &detailInfo);
    virtual void removeMusicRow(const int &row);
    virtual void clearMusicRow();

protected:
    virtual void blockAddFile(const QString &filePath);
    virtual void setHeaderSortFlag();
    KNMusicAnalysisExtend *analysisExtend() const;
    void setAnalysisExtend(KNMusicAnalysisExtend *analysisExtend);

private slots:
    void onActionFileNameChanged(const QString &originalPath,
                                 const QString &currentPath,
                                 const QString &currentFileName);

private:
    KNMusicSearcher *m_searcher;
    KNMusicAnalysisCache *m_analysisCache;
    KNMusicAnalysisExtend *m_analysisExtend=nullptr;
    KNMusicGlobal *m_musicGlobal;
    qint64 m_totalDuration=0;
};

#endif // KNMUSICMODEL_H
