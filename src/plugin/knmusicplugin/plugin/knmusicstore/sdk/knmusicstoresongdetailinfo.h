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
#ifndef KNMUSICSTORESONGDETAILINFO_H
#define KNMUSICSTORESONGDETAILINFO_H

#include <QMap>
#include <QPixmap>
#include <QStringList>

#include <QObject>

/*!
 * \brief The KNMusicSongDetailInfo class provides a object which could describe
 * a song and show up the song details.
 */
class KNMusicStoreSongDetailInfo : public QObject
{
    Q_OBJECT
public:
    enum SongProperties
    {
        Name,
        AlbumName,
        AlbumId,
        SongId,
        Lyrics,
        OnlineUrl,
        HighUrl,
        LossLessUrl,
        SongPropertiesCount
    };

    /*!
     * \brief Construct a KNMusicSongDetailInfo object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreSongDetailInfo(QObject *parent = 0);

    /*!
     * \brief Get the song data. If the song data is not being updated before,
     * it will return a null QString.
     * \param propertyId The porperty Id.
     * \return The property text.
     */
    QString songData(int propertyId);

    /*!
     * \brief Get the album art.
     * \return
     */
    QPixmap albumArt() const;

    /*!
     * \brief Get the artist name list.
     * \return The artist name string list.
     */
    QStringList artists() const;

    /*!
     * \brief Artists locate Ids. Each id will be point-to-point to the artist
     * list.
     * \return The artist id string list.
     */
    QStringList artistsId() const;

signals:

public slots:
    /*!
     * \brief Set the artist list of current song.
     * \param artists The artists name list.
     */
    void setArtists(const QStringList &artists);

    /*!
     * \brief Set the artist Id list of current song.
     * \param artistsId The artists id list.
     */
    void setArtistsId(const QStringList &artistsId);

    /*!
     * \brief Set the song data information.
     * \param propertyId The property Id, it should be in one of the
     * SongProperties.
     * \param data The song property data.
     */
    void setSongData(int propertyId,
                     const QString &data);

    /*!
     * \brief Set the album art pixmap.
     * \param albumArt The album art pixmap.
     */
    void setAlbumArt(const QPixmap &albumArt);

    /*!
     * \brief Reset the song detail information.
     */
    void reset();

private:
    QString m_songProperites[SongPropertiesCount];
    QStringList m_artists, m_artistsId;
    QPixmap m_albumArt;
};

#endif // KNMUSICSTORESONGDETAILINFO_H
