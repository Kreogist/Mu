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

/*!
 * \brief The KNMusicPlaylistModel class is used to describe a playlist. It
 * contains title and file path. It can be built from a json array. You can set
 * the json array and called the buildModel() function to build a playlist.
 */
class KNMusicPlaylistModel : public KNMusicModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistModel class.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistModel(QObject *parent = 0);

    /*!
     * \brief Get the title of the playlist.
     * \return The title string.
     */
    QString title() const;

    /*!
     * \brief Set the title of the playlist model.
     * \param title
     */
    void setTitle(const QString &title);

    QString filePath() const;
    void setFilePath(const QString &filePath);

    bool isBuilt() const;
    void buildModel();

    QJsonArray contentData() const;
    void setContentData(const QJsonArray &contentData);

    bool changed() const;
    void setChanged(bool changed);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

signals:
    void titleChanged(const QString &title);

public slots:

private slots:
    void onActionModelChanged();

private:
    QString m_title, m_filePath;
    QJsonArray m_contentData;
    bool m_built, m_changed;
};

#endif // KNMUSICPLAYLISTMODEL_H
