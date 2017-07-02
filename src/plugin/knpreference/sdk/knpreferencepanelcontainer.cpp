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
#include "knsideshadowwidget.h"
#include "knthememanager.h"

#include "knpreferencepanel.h"

#include "knpreferencepanelcontainer.h"

#include <QDebug>

#define ShadowWidth 15

KNPreferencePanelContainer::KNPreferencePanelContainer(QWidget *parent) :
    QWidget(parent)
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
    //Destory all the current setting panels and shadows.
    while(!m_panelList.isEmpty())
    {
        //Remove the panel from the stack.
        m_panelList.takeLast()->deleteLater();
        //Remove its shadow.
        m_shadowList.takeLast()->deleteLater();
    }
    //Construct a new panel for the block.
    KNPreferencePanel *panel=new KNPreferencePanel(this);
    //Set the blocks for the panel.
    panel->setPanelBlocks(blocks);
    //Add the panel to the list.
    m_panelList.append(panel);
    //Construct a shadow widget for the panel.
    KNSideShadowWidget *shadow=
            new KNSideShadowWidget(KNSideShadowWidget::LeftShadow,
                                   this);
    //Add the shadow to the list.
    m_shadowList.append(shadow);
    //Update shadow and panel size.
    updatePanelSize(panel, shadow);
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
    //Resize the height of all the panel.
    for(int i=0; i<m_panelList.size(); ++i)
    {
        //Update the size of the panel and shadow.
        updatePanelSize(m_panelList.at(i), m_shadowList.at(i));
    }
}

inline void KNPreferencePanelContainer::updatePanelSize(
        KNPreferencePanel *panel,
        KNSideShadowWidget *shadow)
{
    //Update the panel size.
    panel->resize(panel->width(), height());
    //Update the shadow geometry.
    shadow->setGeometry(panel->width() + panel->x(),
                        panel->y(),
                        ShadowWidth,
                        height());
}
