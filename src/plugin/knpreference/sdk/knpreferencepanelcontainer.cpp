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
#include <QScrollBar>

#include "knsideshadowwidget.h"
#include "knthememanager.h"
#include "kndpimanager.h"
#include "knscrollarea.h"
#include "sao/knsaostyle.h"

#include "knpreferencepanel.h"

#include "knpreferencepanelcontainer.h"

#include <QDebug>

#define ShadowWidth 15
#define ScrollBarWidth 10

KNPreferencePanelContainer::KNPreferencePanelContainer(QWidget *parent) :
    QWidget(parent),
    m_panelContainer(new QWidget(this))
{
    setObjectName("PreferencePanelContainer");
    //Set properties.
    setAutoFillBackground(true);
    //Link the theme manager.
    knTheme->registerWidget(this);
}

void KNPreferencePanelContainer::setPanelBlocks(
        const QList<PreferencePanelBlock> &blocks)
{
    //Destroy all the current setting panels and shadows.
    while(!m_panelList.isEmpty())
    {
        //Remove the panel from the stack.
        m_panelList.takeLast()->deleteLater();
        //Remove its shadow.
        m_shadowList.takeLast()->deleteLater();
        //Remove its scroll bar.
        m_scrollList.takeLast()->deleteLater();
    }
    //Construct a new panel for the block.
    KNPreferencePanel *panel=new KNPreferencePanel(m_panelContainer);
    //Set the blocks for the panel.
    panel->setPanelBlocks(blocks);
    //Add the panel to the list.
    m_panelList.append(panel);
    //Generatel the scroll bar for the panel.
    QScrollBar *panelScrollBar=new QScrollBar(Qt::Vertical, this);
    //Configure the scroll bar.
    panelScrollBar->setFixedWidth(knDpi->width(ScrollBarWidth));
    panelScrollBar->setStyle(KNSaoStyle::instance());
    //Set the scroll bar to panel.
    panel->setScrollBar(panelScrollBar);
    //Add the scroll bar to list.
    m_scrollList.append(panelScrollBar);
    //Construct a shadow widget for the panel.
    KNSideShadowWidget *shadow=
            new KNSideShadowWidget(KNSideShadowWidget::LeftShadow,
                                   m_panelContainer);
    //Add the shadow to the list.
    m_shadowList.append(shadow);
    //Update container sizes.
    updateContainerSize();
    //Show the panel and shadow.
    panel->show();
    shadow->show();
}

void KNPreferencePanelContainer::setAdvancedShown(bool showAdvanced)
{
    //Update the item in the panel list.
    for(auto panel : m_panelList)
    {
        //Update the advanced state.
        panel->setAdvancedItemShown(showAdvanced);
    }
}

void KNPreferencePanelContainer::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Update container size.
    updateContainerSize();
}

void KNPreferencePanelContainer::updateContainerSize()
{
    //Check panel sizes.
    if(m_panelList.size()==0)
    {
        //Resize the container.
        m_panelContainer->resize(0, 0);
        return;
    }
    //Update the size of the panel and shadow.
    for(int i=0; i<m_panelList.size(); ++i)
    {
        //Update the panel and shadow widget.
        updatePanelSize(m_panelList.at(i), m_shadowList.at(i),
                        m_scrollList.at(i));
    }
    //Resize the container.
    m_panelContainer->resize(m_panelList.size()*m_panelList.at(0)->width()+
                             knDpi->width(ShadowWidth),
                             height());
}

inline void KNPreferencePanelContainer::updatePanelSize(
        KNPreferencePanel *panel, KNSideShadowWidget *shadow,
        QScrollBar *scrollBar)
{
    //Get the panel height hint.
    int panelHeight=panel->sizeHint().height();
    //Check the panel size.
    if(panelHeight<height())
    {
        //Hide the scroll bar.
        scrollBar->hide();
        //Update the panel size.
        panel->resize(panel->width(), height());
    }
    else
    {
        //Show the scroll bar.
        scrollBar->show();
        //Update the geometry of the scroll bar.
        scrollBar->setGeometry(panel->width()-scrollBar->width(),
                               0,
                               scrollBar->width(),
                               height());
        //Update the scroll bar value.
        scrollBar->setRange(0, panelHeight-height());
    }
    //Update the shadow geometry.
    shadow->setGeometry(panel->width() + panel->x(),
                        panel->y(),
                        knDpi->width(ShadowWidth),
                        height());
}
