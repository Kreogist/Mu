/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KNMUSICCATEGORYMODEL_H
#define KNMUSICCATEGORYMODEL_H

#include <QStandardItemModel>

#include "knmusicglobal.h"

using namespace KNMusic;

class KNMusicCategoryModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicCategoryModel(QObject *parent = 0);
    QString noCategoryText() const;
    void setNoCategoryText(const QString &noCategoryText);
    int categoryIndex() const;
    void setCategoryIndex(int categoryIndex);
    QPixmap noAlbumIcon() const;
    void setNoAlbumIcon(const QPixmap &noAlbumIcon);

signals:

public slots:
    virtual void onCategoryAdded(const QString &categoryText);
    virtual void onCategoryRemoved(const QString &categoryText);
    virtual void onCoverImageUpdate(const QString &categoryText,
                                    const QString &imageKey,
                                    const QPixmap &image);

protected:
    virtual QStandardItem *generateItem(const QString &itemText,
                                        const QPixmap &itemIcon=QPixmap());
    QSize iconSize() const;

private:
    void resetModel();
    int m_categoryIndex=-1;
    QSize m_iconSize=QSize(40, 40);
    QPixmap m_noAlbumIcon;
    QString m_noCategoryText;
};

#endif // KNMUSICCATEGORYMODEL_H
