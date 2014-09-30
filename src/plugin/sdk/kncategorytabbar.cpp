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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QTimeLine>
#include <QSignalMapper>
#include <QBoxLayout>

#include "kncategorybutton.h"

#include "kncategorytabbar.h"

KNCategoryTabBar::KNCategoryTabBar(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    //Hight should be the same as the button.
    setFixedHeight(KNCategoryButton::buttonHeight());
    //Initial the color.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             m_outBridhtness);
    m_palette=palette();
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);

    //Initial the layout.
    m_tabLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_tabLayout->setContentsMargins(0,0,0,0);
    m_tabLayout->setSpacing(0);
    setLayout(m_tabLayout);

    //Initial the signal mapper.
    m_buttonMapper=new QSignalMapper(this);
    connect(m_buttonMapper, SIGNAL(mapped(int)),
            this, SLOT(setCurrentIndex(int)));

    //Initial the mouse in/out timeline.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(m_inBridhtness);
    configureTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(m_outBridhtness);
    configureTimeLine(m_mouseOut);
}

void KNCategoryTabBar::addCategory(const QPixmap &pixmap,
                                   const QString &string)
{
    //Initial the tab button
    KNCategoryButton *categoryButton=new KNCategoryButton(this);
    //Set tab datas
    categoryButton->setIcon(pixmap);
    categoryButton->setText(string);
    //Connect the clicked signal.
    connect(categoryButton, SIGNAL(clicked()),
            m_buttonMapper, SLOT(map()));
    m_buttonMapper->setMapping(categoryButton, m_buttonList.size());
    //Add button to the list.
    m_buttonList.append(categoryButton);
    //Add button to the layout.
    m_tabLayout->addWidget(categoryButton);
    //If current index is -1, means this is the first button, set it to checked.
    if(m_currentIndex==-1)
    {
        setCurrentIndex(0);
    }
}

int KNCategoryTabBar::currentIndex() const
{
    return m_currentIndex;
}

void KNCategoryTabBar::setCurrentIndex(int buttonIndex)
{
    Q_ASSERT(buttonIndex>-1 && buttonIndex<m_buttonList.size());
    //If the current index is button clicked, do nothing.
    if(m_locked || buttonIndex==m_currentIndex)
    {
        return;
    }
    KNCategoryButton *button;
    //Reset the old current index button, check the index first.
    if(m_currentIndex!=-1 && m_currentIndex<m_buttonList.size())
    {
        button=m_buttonList.at(m_currentIndex);
        button->setChecked(false);
    }
    //Set to the current index.
    m_currentIndex=buttonIndex;
    button=m_buttonList.at(m_currentIndex);
    button->setChecked(true);
    //Lock the tabbed
    m_locked=true;
    //Emit index changed signal.
    emit currentIndexChanged(m_currentIndex);
}

void KNCategoryTabBar::enterEvent(QEvent *event)
{
    //Stop animations.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameters.
    m_mouseIn->setStartFrame(m_backgroundColor.value());
    m_mouseIn->start();
    QWidget::enterEvent(event);
}

void KNCategoryTabBar::leaveEvent(QEvent *event)
{
    //Stop animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseOut->setStartFrame(m_backgroundColor.value());
    m_mouseOut->start();
    QWidget::leaveEvent(event);
}

void KNCategoryTabBar::onActionInOutMouse(const int &frame)
{
    //Change the brightness, but can only set hsv in this mode.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             frame);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}

void KNCategoryTabBar::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(5);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNCategoryTabBar::onActionInOutMouse);
}

void KNCategoryTabBar::unlockBar()
{
    m_locked=false;
}

