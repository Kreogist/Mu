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
#include <QPropertyAnimation>

#include "knhwidgetswitcher.h"

KNHWidgetSwitcher::KNHWidgetSwitcher(QWidget *parent) :
    KNWidgetSwitcher(parent)
{
    ;
}

void KNHWidgetSwitcher::updateAnimationPosition()
{
    //Get the three position: left, center, right.
    QRect leftPosition=QRect(-width(),
                             0,
                             width(),
                             height()),
          centerPosition=QRect(0,
                             0,
                             width(),
                             height()),
          rightPosition=QRect(width(),
                              0,
                              width(),
                              height());
    //The end of in and the start of out animation should be the center rect.
    inAnimation()->setEndValue(centerPosition);
    outAnimation()->setStartValue(centerPosition);
    //Check the index of the widget, judge the moving position.
    if(currentIndex()>outWidgetIndex())
    {
        //Move left.
        outAnimation()->setEndValue(leftPosition);
        inAnimation()->setStartValue(rightPosition);
    }
    else
    {
        //Move right.
        outAnimation()->setEndValue(rightPosition);
        inAnimation()->setStartValue(leftPosition);
    }
}
