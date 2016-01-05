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

#ifndef KNMUSICLIBRARYALBUMTAB_H
#define KNMUSICLIBRARYALBUMTAB_H

#include "knmusiclibrarycategorytab.h"

class KNCategoryTab;
class KNDropProxyContainer;
class KNMusicAlbumView;
class KNMusicAlbumModel;
class KNMusicAlbumDetail;
class KNMusicLibraryImageManager;
/*!
 * \brief The KNMusicLibraryAlbumTab class is one of the music category tab. It
 * is specific designed for album tab.\n
 * It contains a category model, a category view and album detail display
 * widget. You should set the.
 */
class KNMusicLibraryAlbumTab : public KNMusicLibraryCategoryTab
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryAlbumTab widget.
     * \param parent The parent widget.
     */
    explicit KNMusicLibraryAlbumTab(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showInAction().
     */
    QAction *showInAction() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::tab().
     */
    QAbstractButton *tab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showInTab().
     */
    void showInTab(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showIndex().
     */
    void showIndex(KNMusicModel *musicModel,
                   const QModelIndex &index) Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::setCategoryModel().
     */
    void setCategoryModel(KNMusicCategoryModelBase *model) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::setLibraryModel().
     */
    void setLibraryModel(KNMusicLibraryModel *model) Q_DECL_OVERRIDE;

    /*!
     * \brief Set the album image manager to album details.
     * \param imageManager The image manager pointer.
     */
    void setImageManager(KNMusicLibraryImageManager *imageManager);

protected:

private slots:
    void retranslate();
    void onActionShowInAlbum();

private:
    inline void showAndSelectRow(const int &musicRow);
    KNCategoryTab *m_tab;
    QAction *m_showInAlbumTab;
    KNDropProxyContainer *m_dropProxy;
    KNMusicAlbumDetail *m_albumDetail;
    KNMusicAlbumView *m_albumView;
    KNMusicAlbumModel *m_albumModel;
    KNMusicLibraryModel *m_libraryModel;
};

#endif // KNMUSICLIBRARYALBUMTAB_H
