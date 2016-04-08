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

#ifndef KNMUSICSTORELISTWIDGET_H
#define KNMUSICSTORELISTWIDGET_H

#include <QVariant>

#include "knmusicstorepanel.h"

class QLabel;
class KNHighLightLabel;
class KNMusicStoreBackend;
class KNMusicStoreAlbumModel;
class KNMusicStoreAlbumTreeView;
/*!
 * \brief The KNMusicStoreListWidget class provides the widget which could
 * display the album data.
 */
class KNMusicStoreListWidget : public KNMusicStorePanel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreListWidget widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreListWidget(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStorePanel::setBackend().
     */
    void setBackend(KNMusicStoreBackend *backend) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionRefresh();
    void onActionUpdateInfo(int category);
    void onActionShowSongInfo(const QModelIndex &currentIndex);

private:
    enum PropertyNames
    {
        PropertyArtist,
        PropertyReleaseDate,
        PropertyCompany,
        PropertyCount
    };
    QLabel *m_properties[PropertyCount], *m_propertiesLabel[PropertyCount],
            *m_titleLabel;
    KNHighLightLabel *m_albumArt;
    KNMusicStoreAlbumModel *m_albumDetailModel;
    KNMusicStoreAlbumTreeView *m_albumSongView;
};

#endif // KNMUSICSTORELISTWIDGET_H
