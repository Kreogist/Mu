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

#ifndef KNMUSICLIBRARYTAB_H
#define KNMUSICLIBRARYTAB_H

#include "knmusicutil.h"

#include "knmusictab.h"

using namespace MusicUtil;

class KNMusicLibraryModel;
/*!
 * \brief The KNMusicLibraryTab class provides the basic interface of a music
 * library tab should be realized.\n
 * You have to provide the showInAction() which will be automatically loaded to
 * music library. And the show in function which will be used by the action.
 */
class KNMusicLibraryTab : public KNMusicTab
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryTab tab with parent widget.
     * \param parent The parent widget.
     */
    explicit KNMusicLibraryTab(QWidget *parent = 0);

    /*!
     * \brief Get the 'show-in' action of the tab.
     * \return The show-in action pointer.
     */
    virtual QAction *showInAction()=0;

    /*!
     * \brief Show up a detail info right in the tab.
     * \param detailInfo The detail info of the music.
     */
    virtual void showInTab(const KNMusicDetailInfo &detailInfo)=0;

signals:
    /*!
     * \brief When this tab is shown up, it will emit this signal which is going
     * to ask for loading the library.
     */
    void requireLoadLibrary();

public slots:
    /*!
     * \brief Set the music library model.
     * \param model The music model.
     */
    virtual void setLibraryModel(KNMusicLibraryModel *model)=0;

protected slots:
    /*!
     * \brief Reimplemented from KNMusicTab::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTab::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
};

#endif // KNMUSICLIBRARYTAB_H
