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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREBACKEND_H
#define KNMUSICSTOREBACKEND_H

#include <QVariant>

#include <QObject>

/*!
 * \brief The KNMusicStoreBackend class provides the abstract ports for a one
 * music store backend. All the backend will be provided by plugin, Mu itself
 * won't provide any resources.
 */
class KNMusicStoreBackend : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreBackend object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreBackend(QObject *parent = 0);

    /*!
     * \brief Set the backend working thread. Do not use moveToThread() function
     * to change the working thread. This function will operate the thread.
     * \param thread The target working thread pointer.
     */
    virtual void setWorkingThread(QThread *thread);

signals:
    /*!
     * \brief Add Internet connection count to the GUI connection status.
     * \param count The number of the connections.
     */
    void requireAddConnectionCount(int count);

    /*!
     * \brief Reduce Internet connection count to the GUI connection status.
     * \param count The number of the connections.
     */
    void requireReduceConnectionCount(int count);

    /*!
     * \brief Set the navigator item text in the header text.
     * \param navigatorIndex The navigator item index.
     * \param caption The text of the navigator item.
     */
    void requireSetNavigatorItem(int navigatorIndex, const QString &caption);

    /*!
     * \brief Ask to set the home page data.
     * \param dataIndex The home page information data.
     * \param value Specific home page data.
     */
    void requireSetHome(int dataIndex, QVariant value);

    /*!
     * \brief Require to change the album song page.
     * \param labelIndex The index of label.
     * \param value The value of the data.
     */
    void requireSetAlbum(int labelIndex, QVariant value);

    /*!
     * \brief Require to change the single song page.
     * \param labelIndex The index of label.
     * \param value The value of the data.
     */
    void requireSetSingleSong(int labelIndex, QVariant value);

public slots:
    /*!
     * \brief Fetch the home page data, set the data to the home page.\n
     * For the home page, we need to fetch 6 different areas:
     *  1. New albums.
     *  2. New songs.
     *  3. Billboard ranking list.
     *  4. Oricon ranking list.
     *  5. iTunes ranking list.
     *  6. Top songs on the backend.
     * These are the information we need, so for each backend, they need to
     * provides these information.
     */
    virtual void showHome()=0;

    /*!
     * \brief Fetch the album information according to the information it
     * provides to the backend.
     * \param albumInfo The album identical information.
     */
    virtual void showAlbum(const QString &albumInfo)=0;

    /*!
     * \brief Fetch the single song information according to the information it
     * provides to the backend.
     * \param songInfo The single song information.
     */
    virtual void showSingleSong(const QString &songInfo)=0;

    /*!
     * \brief Set the timeout maximum seconds.
     * \param seconds The timeout counting seconds.
     */
    virtual void setTimeout(int seconds)=0;

private:

};

#endif // KNMUSICSTOREBACKEND_H
