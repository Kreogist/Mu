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
     * \brief Require to show error dimmer with the specific information
     * provided by the error index.
     * \param errorType The type of the error. 0 for connection error, which is
     * provided by Qt's QNetworkReply itself. 1 for customized error, which is
     * related to the content of the information.
     * \param errorId The error index. For connection error, it is defined in
     * the QNetworkReply. For cutomized error, it is defined in
     * KNMusicStoreGlobal.
     */
    void requireShowError(int errorType, int errorId);

    /*!
     * \brief Require the backend manager to reset operation flag to null. When
     * the operation is done, this signal will be emitted.
     */
    void requireResetOperation();

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
     * \brief Reset Internet connection counter to the GUI connection status.
     */
    void requireResetConnectionCount();

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
     * \brief Fetch the artist information according to the information it
     * provides to the backend.
     * \param artistInfo The artist identical information.
     */
    virtual void showArtist(const QString &artistInfo)=0;

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
