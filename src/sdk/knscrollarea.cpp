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
#include <QTimeLine>

#include "sao/knsaostyle.h"
#include "knthememanager.h"

#include "knscrollarea.h"

#include <QDebug>

#define MaxOpacity 0x20
#define ScrollBarWidth 10
#define ScrollBarSpacing 1

KNScrollArea::KNScrollArea(QWidget *parent) :
    QScrollArea(parent),
    m_hScrollBar(new QScrollBar(Qt::Horizontal, this)),
    m_vScrollBar(new QScrollBar(Qt::Vertical, this)),
    m_mouseAnime(new QTimeLine(200, this)),
    m_currentFrame(0),
    m_vScrollBarTopMargin(0)
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(16);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNScrollArea::onActionMouseInOut);

    //Configure the horizontal scroll bar.
    m_hScrollBar->setStyle(KNSaoStyle::instance());
    m_hScrollBar->hide();
    m_hScrollBar->setFixedHeight(ScrollBarWidth);
    setHorizontalScrollBar(m_hScrollBar);
    //Configure the vertical scroll bar.
    m_vScrollBar->setStyle(KNSaoStyle::instance());
    m_vScrollBar->hide();
    m_vScrollBar->setFixedWidth(ScrollBarWidth);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged,
            [=](int min, int max)
            {
                //Update the range first.
                m_vScrollBar->setRange(min, max);
                //Check whether the scroll bar is still valid.
                m_vScrollBar->setVisible(min!=max);
                //Update scrollbar state parameters.
                m_vScrollBar->setPageStep(verticalScrollBar()->pageStep());
                m_vScrollBar->setSingleStep(verticalScrollBar()->singleStep());
                //Update the geometry.
                updateVerticalScrollBarGeometry();
            });
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            [=](int value)
            {
                //Block the signal.
                m_vScrollBar->blockSignals(true);
                //Reset the value.
                m_vScrollBar->setValue(value);
                //Release the block
                m_vScrollBar->blockSignals(false);
            });
    connect(m_vScrollBar, &QScrollBar::valueChanged,
            verticalScrollBar(), &QScrollBar::setValue);
}

void KNScrollArea::updateObjectName(const QString &name)
{
    //Set the object name.
    setObjectName(name);
    //Update the frame.
    onActionThemeUpdate();
}

void KNScrollArea::enterEvent(QEvent *event)
{
    //Do original enter event.
    QScrollArea::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KNScrollArea::leaveEvent(QEvent *event)
{
    //Do original enter event.
    QScrollArea::leaveEvent(event);
    //Start mouse in anime.
    startAnime(0);
}

void KNScrollArea::showEvent(QShowEvent *event)
{
    //Show the treeview widget.
    QScrollArea::showEvent(event);
    //Update the scroll bar position.
    updateVerticalScrollBarGeometry();
}

void KNScrollArea::resizeEvent(QResizeEvent *event)
{
    //Resize the tree view widget.
    QScrollArea::resizeEvent(event);
    //Update the scroll bar position.
    updateVerticalScrollBarGeometry();
}

void KNScrollArea::onActionThemeUpdate()
{
    //Get the new palette from theme manager, and set it.
    setPalette(knTheme->getPalette(objectName()));
    //Update the palette.
    onActionMouseInOut(0);
}

void KNScrollArea::onActionMouseInOut(int frame)
{
    //Save the frame.
    m_currentFrame=frame;
    //Calculate scroll bar alpha.
    int buttonAlpha=m_currentFrame<<2,
        baseAlpha=m_currentFrame*3;
    //Update the horizontal scroll bar palette.
    QPalette pal=m_hScrollBar->palette();
    QColor color=pal.color(QPalette::Base);
    color.setAlpha(baseAlpha);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(buttonAlpha);
    pal.setColor(QPalette::Button, color);
    //Set the palette to horizontal scroll bar.
    m_hScrollBar->setPalette(pal);
    //Update the vertical scroll bar palette.
    pal=m_vScrollBar->palette();
    color=pal.color(QPalette::Base);
    color.setAlpha(baseAlpha);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(buttonAlpha);
    pal.setColor(QPalette::Button, color);
    //Set the palette to vertical scroll bar.
    m_vScrollBar->setPalette(pal);
}

inline void KNScrollArea::updateVerticalScrollBarGeometry()
{
    //Check scroll bar visiblility.
    if(m_vScrollBar->isVisible())
    {
        //Check whether horizontal scroll is need to be visible.
        if(m_hScrollBar->isVisible())
        {
            //Both of the scroll bar is visible.
            m_vScrollBar->setGeometry(width()-ScrollBarWidth-ScrollBarSpacing,
                                      m_vScrollBarTopMargin,
                                      ScrollBarWidth,
                                      height()-ScrollBarWidth-ScrollBarSpacing
                                      -m_vScrollBarTopMargin);
        }
        else
        {
            //Only vertical scroll bar is visible.
            m_vScrollBar->setGeometry(width()-ScrollBarWidth-ScrollBarSpacing,
                                      m_vScrollBarTopMargin,
                                      ScrollBarWidth,
                                      height()-m_vScrollBarTopMargin);
        }
    }
}

inline void KNScrollArea::startAnime(int endFrame)
{
    //Stop the mouse animations.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(m_currentFrame, endFrame);
    //Start the time line.
    m_mouseAnime->start();
}

void KNScrollArea::setVScrollBarTopMargin(int vScrollBarTopMargin)
{
    m_vScrollBarTopMargin = vScrollBarTopMargin;
}

QScrollBar *KNScrollArea::vScrollBar() const
{
    return m_vScrollBar;
}

QScrollBar *KNScrollArea::hScrollBar() const
{
    return m_hScrollBar;
}
