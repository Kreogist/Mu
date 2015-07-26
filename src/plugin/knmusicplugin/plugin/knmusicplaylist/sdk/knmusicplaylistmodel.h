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

#ifndef KNMUSICPLAYLISTMODEL_H
#define KNMUSICPLAYLISTMODEL_H

#include <QJsonArray>

#include "knmusicmodel.h"

class KNMusicPlaylistModel : public KNMusicModel
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistModel(QObject *parent = 0);

    QString title() const;
    void setTitle(const QString &title);

    QString filePath() const;
    void setFilePath(const QString &filePath);

    bool isBuilt() const;
    void buildModel();

    QJsonArray contentData() const;
    void setContentData(const QJsonArray &contentData);

    bool changed() const;
    void setChanged(bool changed);

signals:

public slots:

private:
    QString m_title, m_filePath;
    QJsonArray m_contentData;
    bool m_built, m_changed;
};

#endif // KNMUSICPLAYLISTMODEL_H
