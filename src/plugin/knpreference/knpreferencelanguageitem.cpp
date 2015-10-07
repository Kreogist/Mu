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
#include <QPainter>

#include "knpreferencelanguageitem.h"

#define ItemHeight 40
#define IconSize 30
#define IconX 17
#define TextX 55 //IconSize+IconX+Spacing(8)

KNPreferenceLanguageItem::KNPreferenceLanguageItem(QWidget *parent) :
    KNPreferenceItem(parent)
{
}

void KNPreferenceLanguageItem::setLanguageName(const QString &languageName)
{
    //Save the language name.
    m_languageName=languageName;
    //Update the widget.
    update();
}

void KNPreferenceLanguageItem::paintContent(QPainter *painter)
{
    //Draw the language icon.
    //Draw the icon if icon is not null.
    if(!icon().isNull())
    {
        //Draw the icon.
        painter->drawPixmap(17,
                            (ItemHeight-IconSize)>>1,
                            IconSize,
                            IconSize,
                            icon().pixmap(IconSize, IconSize));
    }

    //Draw the text.
    painter->setPen(isChecked()?
                        palette().color(QPalette::ButtonText):
                        palette().color(QPalette::WindowText));
    painter->setFont(font());
    painter->drawText(TextX,
                      0,
                      width()-TextX,
                      ItemHeight,
                      Qt::AlignLeft | Qt::AlignVCenter,
                      m_languageName);
}

