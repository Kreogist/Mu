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
     * \param parent The parent object.
     */
    static void initial(QObject *parent = 0);

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
     * \brief Set the navigator item text in the header text.
     * \param navigatorIndex The navigator item index.
     * \param caption The text of the navigator item.
     */
    void requireSetNavigatorItem(int navigatorIndex, const QString &caption);

public slots:
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

private:
    static KNMusicStoreBackendManager *m_instance;
    explicit KNMusicStoreBackendManager(QObject *parent = 0);
    KNMusicStoreBackendManager(const KNMusicStoreBackendManager &);
    KNMusicStoreBackendManager(KNMusicStoreBackendManager &&);

    QHash<QString, KNMusicStoreBackend *> m_backendMap;
    KNMusicStoreContainer *m_pageContainer;
};

#endif // KNMUSICSTOREBACKENDMANAGER_H
