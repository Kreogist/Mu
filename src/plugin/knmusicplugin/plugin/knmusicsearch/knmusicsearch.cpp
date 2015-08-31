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
#include <QCoreApplication>
#include <QInputMethodEvent>
#include <QTextLayout>

#include "knsearchbox.h"
#include "knlocalemanager.h"
#include "knmusicglobal.h"

#include "knmusicsearchsyntaxengine.h"

#include "knmusicsearch.h"

#include <QDebug>

KNMusicSearch::KNMusicSearch(QObject *parent) :
    KNMusicSearchBase(parent),
    m_searchBox(new KNSearchBox()),
    m_engine(new KNMusicSearchSyntaxEngine(this))
{
    //Configure the search box.
    m_searchBox->setMinimumWidth(220);

    //Link the search box to the search actions.
    connect(m_searchBox, &KNSearchBox::textChanged,
            this, &KNMusicSearch::onActionSearch);

    //Link the music search with the locale manager.
    knI18n->link(this, &KNMusicSearch::retranslate);
    retranslate();
}

KNMusicSearch::~KNMusicSearch()
{
    //Check the parent widget of the search box.
    if(m_searchBox->parent()==nullptr)
    {
        m_searchBox->deleteLater();
    }
}

QWidget *KNMusicSearch::widget()
{
    return m_searchBox;
}

QList<KNMusicSearchBlock> KNMusicSearch::rules()
{
    return m_searchBlockList;
}

void KNMusicSearch::search(const QList<KNMusicSearchBlock> &blocks)
{
    //Save the new blocks.
    m_searchBlockList=blocks;
    //Block the signal of the search box.
    m_searchBox->blockSignals(true);
    //Set the search text to search box.
    m_searchBox->setText(m_engine->generateSearchText(m_searchBlockList));
    //Release the signal block.
    m_searchBox->blockSignals(false);
    //Ask to search the data.
    emit requireSearch();
}

void KNMusicSearch::onActionSearchShortcut(QWidget *sourceWidget)
{
    //Set the source widget.
    m_searchBox->setFocusSource(sourceWidget);
    //Set the focus to search box.
    m_searchBox->setFocus();
}

void KNMusicSearch::retranslate()
{
    //Update the text of the search box.
    m_searchBox->setPlaceholderText(tr("Search"));
}

void KNMusicSearch::onActionSearch(const QString &text)
{
    //Use the engin to parse the search box text, save the result to the list.
    m_searchBlockList=m_engine->parseSearch(text);
    //Ask to search the data.
    emit requireSearch();
}
