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
#include <QBoxLayout>
#include <QBrush>

#include "knpreferencetitle.h"
#include "knpreferencecategorylist.h"

#include "knpreferencecategory.h"

KNPreferenceCategory::KNPreferenceCategory(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedWidth(m_listWidth);

    //Set layout.
    m_layout=new QBoxLayout(QBoxLayout::TopToBottom,
                            this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Set the highlight gradient.
    m_highlightGradient.setStart(0,0);
    m_highlightGradient.setFinalStop(0, m_highlightHeight);
    m_highlightGradient.setColorAt(0, QColor(255,255,255,40));
    m_highlightGradient.setColorAt(1, QColor(255,255,255,0));

    //Initial the header button.
    m_title=new KNPreferenceTitle(this);
    connect(m_title, &KNPreferenceTitle::requireHidePreference,
            this, &KNPreferenceCategory::requireHidePreference);
    m_layout->addWidget(m_title);

    //Initial the category list.
    m_categoryList=new KNPreferenceCategoryList(this);
    connect(m_categoryList, &KNPreferenceCategoryList::currentIndexChanged,
            this, &KNPreferenceCategory::onActionIndexChanged);
    m_layout->addWidget(m_categoryList, 1);
}

int KNPreferenceCategory::currentIndex() const
{
    return m_categoryList->currentIndex();
}

void KNPreferenceCategory::addCategory(const QString &title,
                                       const QPixmap &icon,
                                       const QPixmap &headerIcon)
{
    //Add to the list.
    m_categoryList->addCategory(title, icon);
    //Save the header icon.
    m_headerIcons.append(headerIcon);
}

void KNPreferenceCategory::setCurrentIndex(const int &index)
{
    //Check is the index available.
    if(index>-1 && index<m_categoryList->itemCount())
    {
        //Set to the index.
        m_categoryList->setCurrentIndex(index);
    }
}

void KNPreferenceCategory::setCategoryText(const int &index,
                                           const QString &caption)
{
    m_categoryList->setCategoryText(index, caption);
}

void KNPreferenceCategory::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    //Check is the index avaliable.
    //If the index is still -1, but the list is not empty, set it to the first
    //category.
    if(currentIndex()==-1)
    {
        setCurrentIndex(0);
    }
}

void KNPreferenceCategory::paintEvent(QPaintEvent *event)
{
    //Initial the antialiasing painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Paint the background.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundColor);
    painter.drawRect(rect());
    //Paint the highlight.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_highlightGradient);
    painter.drawRect(0,0,width(),64);
    //Draw other things.
    QWidget::paintEvent(event);
    //Paint the shadow.
    QLinearGradient shadowGradient(m_shadowWidth,0,0,0);
    shadowGradient.setColorAt(0, QColor(0,0,0,90));
    shadowGradient.setColorAt(1, QColor(0,0,0,0));
    painter.setBrush(shadowGradient);
    painter.translate(width()-m_shadowWidth, 0);
    painter.drawRect(0,0,m_shadowWidth,height());
}

void KNPreferenceCategory::onActionIndexChanged(const int &index)
{
    //Set the header icon and text.
    m_title->setTitleIcon(m_headerIcons.at(index));
    m_title->setTitleText(m_categoryList->categoryText(index));
    //Emit the category changed signal.
    emit currentIndexChanged(index);
}
