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
#include <QPainter>
#include <QScrollBar>

#include "knthememanager.h"
#include "knlocalemanager.h"
#include "sao/knsaostyle.h"

#include "knnotificationdelegate.h"

#include "knnotificationview.h"

KNNotificationView::KNNotificationView(QWidget *parent) :
    QListView(parent)
{
    setObjectName("NotificationView");
    //Set properties.
    setAutoFillBackground(true);
    setFrameShape(QFrame::NoFrame);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //Set delegate.
    setItemDelegate(new KNNotificationDelegate(this));
    //Configure the scroll bar.
    KNSaoStyle::styleVerticalScrollBar(verticalScrollBar());

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
    //Link international translated.
    knI18n->link(this, &KNNotificationView::retranslate);
    retranslate();
}

int KNNotificationView::heightHint() const
{
    return (model()==nullptr || model()->rowCount()==0)?
                45:model()->rowCount()*NotificationItemHeight;
}

void KNNotificationView::paintEvent(QPaintEvent *event)
{
    //Check model row count.
    if(model()==nullptr || model()->rowCount()==0)
    {
        //Initial the painter.
        QPainter painter(viewport());
        painter.setRenderHints(QPainter::TextAntialiasing |
                               QPainter::SmoothPixmapTransform, true);
        //Paint the empty hint.
        painter.drawText(rect(), Qt::AlignCenter, m_emptyHint);
        //Finished.
        return;
    }
    //Paint the original viewer.
    QListView::paintEvent(event);
}

void KNNotificationView::retranslate()
{
    m_emptyHint=tr("No Notifications");
}

