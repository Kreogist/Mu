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

#ifndef KNMUSICSTORE_H
#define KNMUSICSTORE_H

#include <QModelIndex>

#include "sdk/knmusicstoreutil.h"

#include "knmusicstorebase.h"

class QLabel;
class KNCategoryTab;
class KNMusicStorePanel;
class KNMusicStoreContainer;
class KNMusicStoreTitleBar;
class KNMusicStoreBackend;
class KNMusicStoreGlobal;
class KNMusicStoreHomeWidget;
class KNMusicStoreSearchResultWidget;
class KNMusicStoreListWidget;
class KNMusicStoreSingleSongWidget;
class KNMusicStoreEmptyWidget;
/*!
 * \brief The KNMusicStore class provides a official standard music store plugin
 * realize. You can treat this plugin module as a standard module.
 */
class KNMusicStore : public KNMusicStoreBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStore plugin widget.
     * \param parent The parent widget pointer.
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
                   const QModelIndex &index) Q_DECL_OVERRIDE;

signals:

public slots:

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
    void onActionShowPanel(int category);

private:
    void loadBackend(KNMusicStoreBackend *backend);

    inline void showWidget(QWidget *widget);

    KNCategoryTab *m_tab;
    KNMusicStoreGlobal *m_storeGlobal;
    KNMusicStoreTitleBar *m_titleBar;
    KNMusicStoreContainer *m_storeSwitcher;
    KNMusicStorePanel *m_panels[KNMusicStoreUtil::StorePanelCount];
    KNMusicStoreEmptyWidget *m_emptyWidget;
    KNMusicStoreBackend *m_backend;
};

#endif // KNMUSICSTORE_H
