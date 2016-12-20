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

#ifndef KNMUSICSTORE_H
#define KNMUSICSTORE_H

#include "knmusicstorebase.h"

class KNCategoryTab;
class KNMusicStoreLoadingDimmer;
class KNMusicStoreErrorDimmer;
class KNMusicStoreContainer;
/*!
 * \brief The KNMusicStore class provides the official music store plugin. The
 * official plugin provides four pages,and it will be increased in the future:\n
 *   1. The home page, for provides the several list and some other information.
 *   2. The search result page, for provides the category search result
 * information.
 *   3. The list page, for provides 'list' format data, for example, artist,
 * albums and collection list.
 *   4. The single page, for provides single song data.
 * Besides, the following page should be provided as well:
 *   1. The loading page, for the initial loading scene.
 *   2. The error page, for the Internet error display.
 * All these widget should be provide as a part of the official sdks. This
 * plugin is only a framework of the music store. The content data will be
 * provided by plugins, like the lyrics system.
 */
class KNMusicStore : public KNMusicStoreBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStore tab widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStore(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicStoreBase::tab().
     */
    QAbstractButton *tab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBase::showIndex().
     */
    void showIndex(KNMusicModel *musicModel,
                   const QModelIndex &index);

signals:
    /*!
     * \brief This signal is used to ask the backend manager to start fetching
     * the home page information.
     */
    void requireShowHome();

    /*!
     * \brief Ask the backend manager to load all the backend to the manager,
     * because the thread cannot control the timer created at another thread, so
     * we need to create the backend at the manager thread.
     */
    void requireLoadPlugin();

public slots:
    /*!
     * \brief Reimplemented from KNMusicStoreBase::loadPlugins().
     */
    void loadPlugins() Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMusicStoreBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBase::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();

private:
    KNCategoryTab *m_tab;
    KNMusicStoreContainer *m_container;
    KNMusicStoreErrorDimmer *m_errorDimmer;
    KNMusicStoreLoadingDimmer *m_loadingDimmer;
};

#endif // KNMUSICSTORE_H
