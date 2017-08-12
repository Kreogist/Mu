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
#include <QBoxLayout>

#include "knlabelbutton.h"

#include "knpreferencepanelpathitem.h"

KNPreferencePanelPathItem::KNPreferencePanelPathItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_gotoFolder(new KNLabelButton(this))
{
    //Set the icon to the label.
    m_gotoFolder->setPixmap(QPixmap("://preference/goto_folder.png"));
    //Construct the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    //Construct the text edit layout.
    ;
    //Construct the button layout.
    ;
    buildWidgetLayout(mainLayout);
}

QVariant KNPreferencePanelPathItem::value() const
{
    ;
}

int KNPreferencePanelPathItem::itemHeightMultiple() const
{
    return 2;
}

void KNPreferencePanelPathItem::setWidgetValue(const QVariant &value)
{
    ;
}

bool KNPreferencePanelPathItem::isEqual(const QVariant &currentValue,
                                        const QVariant &originalValue)
{
    //To compare the path, simply compare the path string.
    return currentValue.toString()==originalValue.toString();
}
