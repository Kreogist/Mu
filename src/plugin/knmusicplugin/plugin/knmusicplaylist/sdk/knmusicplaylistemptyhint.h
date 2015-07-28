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

#ifndef KNMUSICPLAYLISTEMPTYHINT_H
#define KNMUSICPLAYLISTEMPTYHINT_H

#include <QUrl>
#include <QList>

#include "kndropproxycontainer.h"

class QLabel;
class KNGlassButton;
/*!
 * \brief The KNMusicPlaylistEmptyHint class is used when there's no playlists
 * exist. This widget is used to give user a hint to create a new playlist or
 * import playlists from the other applications.
 */
class KNMusicPlaylistEmptyHint : public KNDropProxyContainer
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistEmptyHint widget.
     * \param parent The parent widget.
     */
    explicit KNMusicPlaylistEmptyHint(QWidget *parent = 0);

signals:
    /*!
     * \brief When the create playlist button pressed, this signal will be
     * emitted for asking generate a empty playlist.
     */
    void requireAddPlaylist();

    /*!
     * \brief When the import playlists button pressed, this signal will be
     * emitted for asking show import playlist dialog.
     */
    void requireImportPlaylists();

public slots:

private slots:
    void retranslate();

private:
    inline KNGlassButton *generateButton(const QString &iconPath);
    QLabel *m_hintText;
    KNGlassButton *m_addPlaylist, *m_importPlaylist;
};

#endif // KNMUSICPLAYLISTEMPTYHINT_H
