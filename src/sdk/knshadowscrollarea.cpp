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
#include <QScrollBar>

#include "knsideshadowwidget.h"
#include "sao/knsaostyle.h"

#include "knshadowscrollarea.h"

int KNShadowScrollArea::m_shadowSize=15;

KNShadowScrollArea::KNShadowScrollArea(QWidget *parent) :
    QScrollArea(parent),
    m_topShadow(new KNSideShadowWidget(KNSideShadowWidget::TopShadow,
                                       this)),
    m_bottomShadow(new KNSideShadowWidget(KNSideShadowWidget::BottomShadow,
                                          this))
{
    //Set properties.
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Style the vertical scroll bar.
    KNSaoStyle::styleVerticalScrollBar(verticalScrollBar());

    //Hide the shadow for default.
    m_topShadow->hide();
    m_bottomShadow->hide();
    //Link the vertical scroll bar.
    connect(verticalScrollBar(), &QScrollBar::rangeChanged,
            this, &KNShadowScrollArea::onActionVerticalRangeChanged);
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &KNShadowScrollArea::onActionVerticalValueChanged);
}

void KNShadowScrollArea::resizeEvent(QResizeEvent *event)
{
    //Resize the scroll area.
    QScrollArea::resizeEvent(event);
    //Set the new geometry of the top and bottom shadow.
    m_topShadow->setGeometry(0,
                             0,
                             width(),
                             m_shadowSize);
    m_bottomShadow->setGeometry(0,
                                height()-m_shadowSize,
                                width(),
                                m_shadowSize);
}

void KNShadowScrollArea::onActionVerticalRangeChanged(int min, int max)
{
    //When minimal and maximum is the same, hide all shadows.
    if(min==max)
    {
        m_topShadow->hide();
        m_bottomShadow->hide();
        return;
    }
    //Or else check the value of the scroll bar.
    onActionVerticalValueChanged(verticalScrollBar()->value());
}

void KNShadowScrollArea::onActionVerticalValueChanged(int value)
{
    //Show the bottom shadow only if the value is minimum.
    if(verticalScrollBar()->minimum()==value)
    {
        m_topShadow->hide();
        m_bottomShadow->show();
        return;
    }
    //Show the top shadow only if the value is maximum.
    if(verticalScrollBar()->maximum()==value)
    {
        m_topShadow->show();
        m_bottomShadow->hide();
        return;
    }
    //Show both shadow when the value is just a normal value.
    m_topShadow->show();
    m_bottomShadow->show();
}
