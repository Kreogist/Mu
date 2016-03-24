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

#ifndef KNMUSICSTORESINGLESONGWIDGET_H
#define KNMUSICSTORESINGLESONGWIDGET_H

#include <QWidget>

class QLabel;
class QBoxLayout;
class KNHighLightLabel;
class KNMusicStoreBackend;
class KNMusicStoreSongDetailInfo;
/*!
 * \brief The KNMusicStoreSingleSongWidget class provides the a widget which
 * could show the song.
 */
class KNMusicStoreSingleSongWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreSingleSongWidget widget
     * \param parent The parent widget pointer.
     */
    explicit KNMusicStoreSingleSongWidget(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set backend to home widget.
     * \param backend The music store backend.
     */
    void setBackend(KNMusicStoreBackend *backend);

private slots:
    void retranslate();
    void onActionDataUpdate();

private:
    enum PropertyNames
    {
        PropertyArtist,
        PropertyAlbum,
        PropertyCount
    };
    QLabel *m_properties[PropertyCount], *m_propertiesLabel[PropertyCount],
            *m_titleLabel, *m_lyricsLabel;
    KNHighLightLabel *m_albumArt;
    KNMusicStoreSongDetailInfo *m_songDetail;
};

#endif // KNMUSICSTORESINGLESONGWIDGET_H
