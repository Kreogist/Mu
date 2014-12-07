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
#ifndef KNMUSICLIBRARYIMAGEMANAGER_H
#define KNMUSICLIBRARYIMAGEMANAGER_H

#include <QStringList>

#include <QObject>

class KNHashPixmapList;
class KNMusicLibraryImageManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLibraryImageManager(QObject *parent = 0);
    KNHashPixmapList *pixmapList() const;
    void setPixmapList(KNHashPixmapList *pixmapList);
    void recoverFromFolder();
    void removeImage(const QString &imageHash);
    QString imageFolderPath() const;
    void setImageFolderPath(const QString &imageFolderPath);

signals:
    void requireSaveNext();
    void recoverComplete();

public slots:
    void saveImage(const QString &imageHash);

private slots:
    void onActionSaveNext();

private:
    QStringList m_imageHashList;
    QString m_imageFolderPath;
    KNHashPixmapList *m_pixmapList=nullptr;
};

#endif // KNMUSICLIBRARYIMAGEMANAGER_H
