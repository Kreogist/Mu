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
#include <QBoxLayout>

#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knanimelabelbutton.h"

#include "knmusicstoresearchmodel.h"
#include "knmusicstoreglobal.h"
#include "knmusicstoreutil.h"
#include "knmusicstorebackend.h"
#include "knmusicstorealbumtreeview.h"

#include "knmusicstoresearchresultwidget.h"

using namespace MusicStoreUtil;

KNMusicStoreSearchResultWidget::KNMusicStoreSearchResultWidget(QWidget *parent):
    KNMusicStorePanel(parent),
    m_songTreeView(new KNMusicStoreAlbumTreeView(this))
{
    setObjectName("MusicStoreWidget");
    //Set properties.
    setAutoFillBackground(true);
    setFixedWidth(knMusicStoreGlobal->storeContentWidth());
    //Register this widget to theme manager.
    knTheme->registerWidget(this);

    //Configure the song view.
    m_songTreeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_songTreeView->updateObjectName("MusicStoreAlbumView");

    QBoxLayout *test=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(test);
    test->addWidget(m_songTreeView);
}

void KNMusicStoreSearchResultWidget::setBackend(KNMusicStoreBackend *backend)
{
    //Check backend pointer first.
    if(backend==nullptr)
    {
        //Ignore the pointer.
        return;
    }
    //Save the backend.
    m_backend=backend;
    //Get the model, set the search model to the specific view.
    m_songTreeView->setModel(m_backend->searchResultModel(CategorySong));
    //Link the backend with the signal.
}

void KNMusicStoreSearchResultWidget::retranslate()
{
    //Update the label text.
    headerLabel()->setText(tr("Search %1").arg(""));
}

void KNMusicStoreSearchResultWidget::onActionSearchComplete()
{
    ;
}
