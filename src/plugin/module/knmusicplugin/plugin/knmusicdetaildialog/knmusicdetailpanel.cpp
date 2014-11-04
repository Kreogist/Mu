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

#include "knhwidgetswitcher.h"
#include "knmusicdetailoverview.h"

#include "knmusicdetailpanel.h"

KNMusicDetailPanel::KNMusicDetailPanel(QWidget *parent) :
    QWidget(parent)
{
    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the content switcher header.
    //Initial the content switcher widget.
    m_contents=new KNHWidgetSwitcher(this);
    mainLayout->addWidget(m_contents);
    //Initial overview widget.
    initialOverview();
}

QSize KNMusicDetailPanel::sizeHint() const
{
    return QSize(460, 400);
}

void KNMusicDetailPanel::setFilePath(const QString &filePath)
{
    //Set all the content to the file path.
    m_overview->setFilePath(filePath);
}

void KNMusicDetailPanel::initialOverview()
{
    //Initial overview widget.
    m_overview=new KNMusicDetailOverview(this);
    //Add widget to content switcher.
    m_contents->addWidget(m_overview);
}
