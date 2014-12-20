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
#include "knsearchbox.h"
#include "knlocalemanager.h"

#include "knmusicsearch.h"

KNMusicSearch::KNMusicSearch(QObject *parent) :
    KNMusicSearchBase(parent)
{
    //Initial search box.
    m_searchBox=new KNSearchBox;
    m_searchBox->setMinimumWidth(220);
    //Connect request.
    connect(m_searchBox, &KNSearchBox::textChanged,
            this, &KNMusicSearch::requireSearch);

    //Connect retranslate request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicSearch::retranslate);
    //Retranslate.
    retranslate();
}

QWidget *KNMusicSearch::searchBox()
{
    return m_searchBox;
}

QString KNMusicSearch::searchText()
{
    return m_searchBox->text();
}

void KNMusicSearch::retranslate()
{
    m_searchBox->setPlaceHolderText(tr("Search in Music"));
}

void KNMusicSearch::setSearchFocus(QWidget *focusSource)
{
    //Set the focus source and focus.
    m_searchBox->setEscFocusTo(focusSource==nullptr?m_defaultSender:focusSource);
    m_searchBox->setFocus();
}

void KNMusicSearch::search(const QString &text)
{
    m_searchBox->setText(text);
}

void KNMusicSearch::setDefaultFocusSource(QWidget *widget)
{
    m_defaultSender=widget;
}
