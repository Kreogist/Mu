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
#include <QBoxLayout>
#include <QPushButton>

#include "knsearchbox.h"
#include "knlocalemanager.h"
#include "knthememanager.h"
#include "kndpimanager.h"

#include "knmusiccategorysearch.h"

KNMusicCategorySearch::KNMusicCategorySearch(QWidget *parent) :
    QWidget(parent),
    m_searchBox(new KNSearchBox(this)),
    m_cancelButton(new QPushButton(this))
{
    setObjectName("MusicCategorySearch");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(knDpi->height(30));
    setFocusProxy(m_searchBox);

    //Link the search box.
    connect(m_searchBox, &KNSearchBox::requireSearch,
            this, &KNMusicCategorySearch::requireSearch);
    connect(m_searchBox, &KNSearchBox::escapePressed,
            this, &KNMusicCategorySearch::onActionCancel);
    //Link the cancel box.
    connect(m_cancelButton, &QPushButton::clicked,
            this, &KNMusicCategorySearch::onActionCancel);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(knDpi->margins(5,0,5,0));
    mainLayout->setSpacing(knDpi->width(5));
    setLayout(mainLayout);
    //Add widget the layout.
    mainLayout->addWidget(m_searchBox, 1);
    mainLayout->addWidget(m_cancelButton);

    //Link theme manager.
    knTheme->registerWidget(this);

    //Link retranslate signal.
    knI18n->link(this, &KNMusicCategorySearch::retranslate);
    retranslate();
}

void KNMusicCategorySearch::setPlaceHolderText(const QString &text)
{
    //Set the text to the search box.
    m_searchBox->setPlaceholderText(text);
}

void KNMusicCategorySearch::clear()
{
    //Clear the search box text.
    m_searchBox->clear();
}

void KNMusicCategorySearch::retranslate()
{
    //Update the cancel button text.
    m_cancelButton->setText(tr("Cancel"));
    //Update the size of cancel button.
    m_cancelButton->setMaximumWidth(
                fontMetrics().width(m_cancelButton->text())+knDpi->width(10));
}

void KNMusicCategorySearch::onActionCancel()
{
    //Clear the search box.
    clear();
    //Emit cancel searching.
    emit requireCancel();
}

