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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef Q_OS_MACX
#include <QPropertyAnimation>
#endif
#include <QApplication>
#include <QBitmap>
#include <QDesktopWidget>
#include <QLabel>

#include "knsaostyle.h"

#include "knsaosubmenu.h"

#include <QDebug>

KNSaoSubMenu::KNSaoSubMenu(QWidget *parent) :
    QMenu(parent),
    #ifndef Q_OS_MACX
    m_start(new QPropertyAnimation(this, "geometry", this)),
    #endif
    m_indicator(new QWidget)
{
    //Initial the object name.
    setObjectName("SAOSubMenu");
#ifdef Q_OS_MACX
    setWindowOpacity(0.9);
#endif
    //Initial the style.
    setStyle(KNSaoStyle::instance());
    //Set properties.
    m_indicator->setWindowFlags(Qt::ToolTip |
                                Qt::FramelessWindowHint);
    m_indicator->setAutoFillBackground(true);
    m_indicator->setAttribute(Qt::WA_NoBackground, true);
    m_indicator->setAttribute(Qt::WA_TranslucentBackground, true);
    m_indicator->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    //Set palette.
    QPalette pal=m_indicator->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(0,0,0,0));
    pal.setColor(QPalette::AlternateBase, QColor(0,0,0,0));
    pal.setColor(QPalette::ToolTipBase, QColor(255,255,255,0));
    pal.setColor(QPalette::ToolTipText, QColor(255,255,255,0));
    pal.setColor(QPalette::Button, QColor(255,255,255,0));
    pal.setColor(QPalette::ButtonText, QColor(255,255,255,0));
    m_indicator->setPalette(pal);
    //Generate the image label.
    QLabel *indicatorLabel=new QLabel(m_indicator);
    indicatorLabel->setPalette(pal);
    QPixmap indicatorPixmap=QPixmap("://public/indicator.png");
    indicatorLabel->setPixmap(indicatorPixmap);
#ifdef Q_OS_WIN32
    m_indicator->setMask(indicatorPixmap.mask());
#endif
    m_indicator->setFixedSize(indicatorPixmap.size());
    m_indicator->hide();

#ifndef Q_OS_MACX
    //Configure the animation.
    m_start->setEasingCurve(QEasingCurve::OutCubic);
#endif
}

KNSaoSubMenu::~KNSaoSubMenu()
{
    //Recover the indicator's memory.
    m_indicator->deleteLater();
}

void KNSaoSubMenu::showEvent(QShowEvent *event)
{
#ifndef Q_OS_MACX
    //Stop the animation.
    m_start->stop();
#endif
    //Move and show the indicator.
    int menuX=QCursor::pos().x()+m_indicator->width(),
        centerPosition=QCursor::pos().y();
    //Chechk the parent widget.
    if(parentWidget()!=nullptr)
    {
        //Check the parent object.
        if(parentWidget()->objectName()=="SAOSubMenu")
        {
            //When the parent menu is a SAO sub menu as well, update the
            //geometry of the menu.
            QMenu *parentMenu=(QMenu *)parentWidget();
            //Get the action position on the screen.
            QRect menuActionGeometry=
                    parentMenu->actionGeometry(this->menuAction());
            menuX=parentMenu->x()+
                    menuActionGeometry.x()+
                    menuActionGeometry.width()+
                    m_indicator->width()/2;
            centerPosition=parentMenu->y()+
                    menuActionGeometry.y()+
                    menuActionGeometry.height()/2;
        }
        else
        {
            //This is the top level sub menu. Update the position.
            QPoint globalParentPos=
                    parentWidget()->mapToGlobal(QPoint(0,0));
            menuX=globalParentPos.x() + parentWidget()->width() +
                    m_indicator->width()/2;
            centerPosition=globalParentPos.y()+parentWidget()->height()/2;
        }
    }
    //Move the indicator.
    m_indicator->move(menuX-m_indicator->width(),
                      centerPosition-m_indicator->height()/2);
    m_indicator->show();
    //Generate the prefer geometry.
    int preferTopPosition=centerPosition-height()/2;
    if(preferTopPosition<0)
    {
        preferTopPosition=0;
    }
    if(preferTopPosition+height()>QApplication::desktop()->height())
    {
        preferTopPosition=QApplication::desktop()->height()-height();
    }
#ifdef Q_OS_MACX
    //Set the geometry of the sub menu.
    setGeometry(QRect(menuX,
                      preferTopPosition,
                      width(),
                      height()));
#else
    //Set the position of the animation.
    m_start->setStartValue(QRect(menuX,
                                 preferTopPosition+height()/2,
                                 width(),
                                 1));
    m_start->setEndValue(QRect(menuX,
                               preferTopPosition,
                               width(),
                               height()));
    //Start the animation.
    m_start->start();
#endif
    //Do show event.
    QMenu::showEvent(event);
}

void KNSaoSubMenu::hideEvent(QHideEvent *event)
{
    //Hide the indicator.
    m_indicator->hide();
    //Do hide event.
    QMenu::hideEvent(event);
}

