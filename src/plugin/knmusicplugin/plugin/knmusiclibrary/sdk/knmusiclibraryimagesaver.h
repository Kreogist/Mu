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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICLIBRARYIMAGESAVER_H
#define KNMUSICLIBRARYIMAGESAVER_H

#include <QVariant>
#include <QHash>

#include <QStringList>

#include <QObject>

class KNMusicLibraryImageSaver : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLibraryImageSaver(QObject *parent = 0);

    QHash<QString, QVariant> *hashAlbumArt() const;
    void setHashAlbumArt(QHash<QString, QVariant> *hashAlbumArt);

    QString imageFolderPath() const;
    void setImageFolderPath(const QString &imageFolderPath);

signals:
    void requireSaveNext();

public slots:
    void saveImage(const QString &imageHashKey);

private slots:
    void onActionSaveNext();

private:
    QStringList m_savingQueue;
    QString m_imageFolderPath;
    QHash<QString, QVariant> *m_hashAlbumArt;
};

#endif // KNMUSICLIBRARYIMAGESAVER_H
