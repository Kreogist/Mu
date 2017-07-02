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

#ifndef KNMUSICLIBRARYSONGTAB_H
#define KNMUSICLIBRARYSONGTAB_H

#include "knmusiclibrarytab.h"

class KNCategoryTab;
class KNEmptyStateWidget;
class KNMusicLibraryDropProxy;
class KNMusicLibraryTreeView;
class KNMusicLibraryEmptyHint;
/*!
 * \brief The KNMusicLibrarySongTab class provides the library tab for the all
 * song view of the library.
 */
class KNMusicLibrarySongTab : public KNMusicLibraryTab
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibrarySongTab widget.
     * \param parent The parent widget.
     */
    explicit KNMusicLibrarySongTab(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from the KNMusicLibraryTab::tab().
     */
    QAbstractButton *tab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from the KNMusicLibraryTab::showInAction().
     */
    QAction *showInAction() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from the KNMusicLibraryTab::showInTab().
     */
    void showInTab(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from the KNMusicLibraryTab::showIndex().
     */
    void showIndex(KNMusicModel *musicModel,
                   const QModelIndex &index) Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from the KNMusicLibraryTab::setLibraryModel().
     */
    void setLibraryModel(KNMusicLibraryModel *model) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from the KNMusicLibraryTab::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionShowInSong();
    void onActionSearch();

private:
    inline void showAndSelectRow(int musicRow);
    KNCategoryTab *m_tab;
    KNEmptyStateWidget *m_emptyStateWidget;
    KNMusicLibraryDropProxy *m_dropProxy;
    KNMusicLibraryEmptyHint *m_emptyHint;
    KNMusicLibraryModel *m_libraryModel;
    KNMusicLibraryTreeView *m_treeView;
    QAction *m_showInSongTab;
};

#endif // KNMUSICLIBRARYSONGTAB_H
