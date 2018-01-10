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
#include <QCheckBox>
#include <QPainter>
#include <QStyle>

#include "kndpimanager.h"

#include "knpreferencepanelsubitem.h"

#define CheckImageSize 32

KNPreferencePanelSubItem::KNPreferencePanelSubItem(QWidget *parent) :
    QAbstractButton(parent),
    m_rawText(QString()),
    m_displayText(QString())
{
    //Set the properties.
//    setCheckable(true);
}

void KNPreferencePanelSubItem::setText(const QString &text)
{
    //Save the raw text.
    m_rawText=text;
    //Update the text data.
    updateButtonText();
}

void KNPreferencePanelSubItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Draw the content.
    QPainter painter(this);
    painter.drawText(textRect(), Qt::AlignLeft | Qt::AlignVCenter,
                     m_displayText);
    if(isCheckable())
    {
        //Check the button is checked.
        int iconPos=(height()-knDpi->height(16))>>1,
                iconX=contentsMargins().left()+iconPos;
        painter.drawRect(QRect(QPoint(iconX, iconPos)+knDpi->pos(2, 2),
                               knDpi->size(12, 12)));
        if(isChecked())
        {
            //Draw the icon button.
            painter.drawPixmap(iconX, iconPos,
                               QPixmap("://public/tick.png"));
        }
    }
}

void KNPreferencePanelSubItem::resizeEvent(QResizeEvent *event)
{
    //Resize the item.
    QAbstractButton::resizeEvent(event);
    //Update the button text.
    updateButtonText();
}

void KNPreferencePanelSubItem::updateButtonText()
{
    //Check the raw text length.
    if(m_rawText.isEmpty())
    {
        //Clear all.
        m_displayText=QString();
        setToolTip("");
        //Update the cache.
        update();
        return;
    }
    //Get the rect text.
    int contentWidth=textRect().width();
    //Update the text.
    QString eliedText=fontMetrics().elidedText(m_rawText, Qt::ElideMiddle,
                                               contentWidth);
    //Check text length.
    if(eliedText.length()!=m_rawText.length())
    {
        //Text has been elided.
        //Do original function.
        m_displayText=eliedText;
        //Update the tooltip.
        setToolTip(m_rawText);
        //Update the cache.
        update();
    }
    else
    {
        //Set the raw text.
        m_displayText=m_rawText;
        //Clear the tooltip.
        setToolTip("");
        //Update the cache.
        update();
    }
}

inline QRect KNPreferencePanelSubItem::textRect() const
{
    int leftPatch=isCheckable()?height():0;
    return QRect(contentsRect().x()+leftPatch,
                 contentsRect().y(),
                 contentsRect().width()-leftPatch,
                 contentsRect().height());
}
