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
#include "knheadercontainer.h"
#include "knheaderswitcher.h"
#include "knheaderbutton.h"

#include "knmainwindowheader.h"

KNMainWindowHeader::KNMainWindowHeader(QObject *parent) :
    KNMainWindowHeaderPlugin(parent)
{
    //Initial the widgets.
    m_headerContainer=new KNHeaderContainer;

    //Initial the header button.
    m_headerButton=new KNHeaderButton(m_headerContainer);
    m_headerContainer->setHeaderButton(m_headerButton);

    //Initial the switcher.
    m_headerSwitcher=new KNHeaderSwitcher(m_headerContainer);
    m_headerContainer->setHeaderSwitcher(m_headerSwitcher);

    //Connect signals.
    //To make the text of button visible, we need to change the left margin.
    connect(m_headerButton, &KNHeaderButton::requireSetLeftSpacing,
            m_headerContainer, &KNHeaderContainer::setLeftSpacing);
    connect(m_headerButton, &KNHeaderButton::requireShowCategorySwitcher,
                this, &KNMainWindowHeaderPlugin::requireShowCategorySwitcher);
}

QWidget *KNMainWindowHeader::headerWidget()
{
    return m_headerContainer;
}

void KNMainWindowHeader::addHeaderWidget(QWidget *widget)
{
    m_headerSwitcher->addWidget(widget);
}

void KNMainWindowHeader::setHeaderText(const QString &text)
{
    m_headerButton->setText(text);
}

void KNMainWindowHeader::setHeaderIcon(const QPixmap &icon)
{
    m_headerButton->setIcon(icon);
}

void KNMainWindowHeader::restoreHeaderButton()
{
    m_headerButton->restoreButtonState();
}
