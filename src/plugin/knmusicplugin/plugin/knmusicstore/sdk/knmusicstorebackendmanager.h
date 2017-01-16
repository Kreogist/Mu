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

#ifndef KNMUSICSTOREBACKENDMANAGER_H
#define KNMUSICSTOREBACKENDMANAGER_H

#include <QHash>

#include <QObject>

/*!
 * \def knMusicStoreBackendManager
 * A global pointer referring to the unique music store backend manager object.
 */
#define knMusicStoreBackendManager (KNMusicStoreBackendManager::instance())

class KNMusicStoreBackend;
class KNMusicStoreContainer;
/*!
 * \brief The KNMusicStoreBackendManager class manages all the backends and
 * connect the using backend to the front end widgets.
 */
class KNMusicStoreBackendManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the global instance object pointer.
     * \return The instance object pointer.
     */
    static KNMusicStoreBackendManager *instance();

    /*!
     * \brief Construct the global KNMusicStoreBackendManager object.
     * \param workingThread The working thread of the manager.
     */
    static void initial(QThread *workingThread);

    /*!
     * \brief Add one backend to the backend manager.
     * \param backend The backend instance.
     */
    void addBackend(KNMusicStoreBackend *backend);

    /*!
     * \brief Set the page container for the backend.
     * \param pageContainer The page container widget.
     */
    void setPageContainer(KNMusicStoreContainer *pageContainer);

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

public slots:
    /*!
     * \brief Load all the backend to the manager. Because all the timer must be
     * loaded here.
     */
    void loadPlugins();

    /*!
     * \brief Fetch the home page information and show the home page.
     */
    void showHomePage();

    /*!
     * \brief Fetch the artist information according to the information it
     * provides to a specific backend.
     * \param backendId The ID of the backend, i.e. its object name.
     * \param artistInfo The artist identical information.
     */
    void showArtist(const QString &backendId, const QString &artistInfo);

    /*!
     * \brief Fetch the album information according to the information it
     * provides to a specific backend.
     * \param backendId The ID of the backend, i.e. its object name.
     * \param albumInfo The album identical information.
     */
    void showAlbum(const QString &backendId, const QString &albumInfo);

    /*!
     * \brief Fetch the single song information according to the information it
     * provides to the backend.
     * \param backendId The ID of the backend, i.e. its object name.
     * \param songInfo The single song information.
     */
    void showSingleSong(const QString &backendId, const QString &songInfo);

private slots:
    void resetOperationFlag();

private:
    enum OperationTypes
    {
        OperationShowHome,
        OperationShowArtist,
        OperationShowAlbum,
        OperationShowSingleSong
    };
    static KNMusicStoreBackendManager *m_instance;
    explicit KNMusicStoreBackendManager(QThread *workingThread);
    KNMusicStoreBackendManager(const KNMusicStoreBackendManager &);
    KNMusicStoreBackendManager(KNMusicStoreBackendManager &&);

    QHash<QString, KNMusicStoreBackend *> m_backendMap;
    QString m_currentMetadata, m_backendName;
    KNMusicStoreContainer *m_pageContainer;
    int m_currentOperation;
};

#endif // KNMUSICSTOREBACKENDMANAGER_H
