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
#include <QBoxLayout>

#include "knpreferencewidgetspanel.h"
#include "knpreferencecategory.h"
#include "knpreferencecontents.h"

#include "knpreferencepanel.h"

#include <QDebug>

KNPreferencePanel::KNPreferencePanel(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Initial layout.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight,
                            this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Initial widgets.
    m_categoryList=new KNPreferenceCategory(this);
    connect(m_categoryList, &KNPreferenceCategory::currentIndexChanged,
            this, &KNPreferencePanel::onActionCategoryIndexChange);
    connect(m_categoryList, &KNPreferenceCategory::requireHidePreference,
            this, &KNPreferencePanel::requireHidePreference);
    m_layout->addWidget(m_categoryList, 1);
    m_contents=new KNPreferenceContents(this);
    m_layout->addWidget(m_contents, 1);

    //Initial the general panel.
    m_generalPanel=new KNPreferenceWidgetsPanel(this);

    //Add category.
    addCategory("",
                QPixmap(":/plugin/configure/general/icon.png"),
                QPixmap(":/plugin/configure/general/headicon.png"),
                m_generalPanel);

    //Retranslate.
    retranslate();
}

void KNPreferencePanel::addCategory(const QString &title,
                                    const QPixmap &icon,
                                    const QPixmap &headerIcon,
                                    QWidget *contentWidget)
{
    //Add the info of this category to the category list and content.
    m_categoryList->addCategory(title, icon, headerIcon);
    m_contents->addPanelWidget(contentWidget);
}

void KNPreferencePanel::setCategoryText(const int &index,
                                        const QString &title)
{
    m_categoryList->setCategoryText(index, title);
}

void KNPreferencePanel::setCurrentIndex(const int &index)
{
    //Because when category list change the index, the content will
    //automatically changed, so we don't need to change the contents index.
    m_categoryList->setCurrentIndex(index);
}

void KNPreferencePanel::retranslate()
{
    setCategoryText(0, tr("General"));
}

void KNPreferencePanel::onActionCategoryIndexChange(const int &index)
{
    m_contents->setCurrentIndex(index);
}
