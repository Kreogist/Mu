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
#include <QCursor>

#include "knmousedetectheader.h"

KNMouseDetectHeader::KNMouseDetectHeader(QWidget *parent) :
    QWidget(parent)
{
}

void KNMouseDetectHeader::checkCursor()
{
    //Check the global cursor is inside or outside the header.
    if(rect().contains(mapFromGlobal(QCursor::pos())))
    {
        //Emit activate widget signal.
        emit requireActivateWidget();
    }
    else
    {
        //Emit inactivate widget signal.
        emit requireInactivateWidget();
    }
}

void KNMouseDetectHeader::enterEvent(QEvent *event)
{
    //Emit activate signal.
    emit requireActivateWidget();
    //Do original enter event.
    QWidget::enterEvent(event);
}

void KNMouseDetectHeader::leaveEvent(QEvent *event)
{
    //Emit inactivate signal.
    emit requireInactivateWidget();
    //Do original leave event.
    QWidget::leaveEvent(event);
}
