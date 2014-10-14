/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QThread>
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
    ~KNMusicGlobal();
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

signals:
    void rowCountChanged();
    void requireAnalysisFiles(QStringList urls);

public slots:
    virtual void appendMusicRow(const QList<QStandardItem *> &musicRow);

private:
    KNMusicSearcher *m_searcher;
    KNMusicAnalysisCache *m_analysisCache;
    KNMusicGlobal *m_musicGlobal;
    qint64 m_totalDuration=0;
};

#endif // KNMUSICMODEL_H
