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
#include <QLabel>

#include "knanimeroundswitcher.h"
#include "knsideshadowwidget.h"
#include "knlinearsensewidget.h"
#include "knpreferencetitle.h"
#include "knpreferencecategorylist.h"

#include "knpreferencecategory.h"

KNPreferenceCategory::KNPreferenceCategory(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedWidth(m_listWidth);

    //Set layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                            this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Set the highlight gradient.
    m_highlightGradient.setStart(0,0);
    m_highlightGradient.setFinalStop(0, m_highlightHeight);
    m_highlightGradient.setColorAt(0, QColor(255,255,255,40));
    m_highlightGradient.setColorAt(1, QColor(255,255,255,0));

    //Initial the header button.
    m_title=new KNPreferenceTitle(this);
    connect(m_title, &KNPreferenceTitle::requireHidePreference,
            this, &KNPreferenceCategory::requireHidePreference);
    mainLayout->addWidget(m_title);

    //Initial the category list.
    m_categoryList=new KNPreferenceCategoryList(this);
    connect(m_categoryList, &KNPreferenceCategoryList::currentIndexChanged,
            this, &KNPreferenceCategory::onActionIndexChanged);
    mainLayout->addWidget(m_categoryList, 1);

    //Initial the status bar.
    initialStatusBar();
    mainLayout->addWidget(m_statusBar);

    //Initial the shadow.
    m_rightShadow=new KNSideShadowWidget(RightShadow, this);

    //Do translation.
    retranslate();
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

void KNPreferenceCategory::retranslate()
{
    m_normal->setText(tr("Normal"));
    m_advanced->setText(tr("Advanced"));
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
}

void KNPreferenceCategory::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Move the shadow.
    m_rightShadow->setGeometry(width()-m_shadowWidth,
                               0,
                               m_shadowWidth,
                               height());
}

void KNPreferenceCategory::onActionIndexChanged(const int &index)
{
    //Set the header icon and text.
    m_title->setTitleIcon(m_headerIcons.at(index));
    m_title->setTitleText(m_categoryList->categoryText(index));
    //Emit the category changed signal.
    emit currentIndexChanged(index);
}

void KNPreferenceCategory::initialStatusBar()
{
    //Initial the status bar widget.
    m_statusBar=new KNLinearSenseWidget(this);
    m_statusBar->setContentsMargins(0,0,0,0);
    m_statusBar->setFixedHeight(34);

    //Initial layout.
    QBoxLayout *statusLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_statusBar);
    statusLayout->setContentsMargins(0,0,0,0);
    statusLayout->setSpacing(0);
    m_statusBar->setLayout(statusLayout);

    //Initial normal/advanced switcher.
    m_normal=new QLabel(this);
    m_normal->setContentsMargins(5,0,5,0);
    QPalette captionPalette=m_normal->palette();
    captionPalette.setColor(QPalette::WindowText, QColor(255,255,255));
    m_normal->setPalette(captionPalette);
    statusLayout->addWidget(m_normal);
    KNAnimeRoundSwitcher *advancedSwitcher=new KNAnimeRoundSwitcher(this);
    connect(advancedSwitcher, &KNAnimeRoundSwitcher::buttonPositionChange,
            this, &KNPreferenceCategory::requireShowNormal);
    statusLayout->addWidget(advancedSwitcher);
    m_advanced=new QLabel(this);
    m_advanced->setContentsMargins(5,0,5,0);
    m_advanced->setPalette(captionPalette);
    statusLayout->addWidget(m_advanced);

    statusLayout->addStretch();
}
