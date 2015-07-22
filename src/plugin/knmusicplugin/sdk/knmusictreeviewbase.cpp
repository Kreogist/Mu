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
#include <QScrollBar>
#include <QTimeLine>
#include <QPainter>

#include "knthememanager.h"

#include "knmusictreeviewheader.h"

#include "knmusictreeviewbase.h"

#include <QDebug>

#define MaxOpacity 0x20
#define FontBase 0xBF

KNMusicTreeViewBase::KNMusicTreeViewBase(QWidget *parent) :
    QTreeView(parent),
    m_mouseIn(generateTimeLine(0x20)),
    m_mouseOut(generateTimeLine(0)),
    m_animate(true)
{
    //Set properties.
    setAllColumnsShowFocus(true);
    setAlternatingRowColors(false); //We will use our own alternating drawing.
    setContentsMargins(0, 0, 0, 0);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDropIndicatorShown(true);
    setFrameShape(QFrame::NoFrame);
    setIndentation(0);
    setMouseTracking(true);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformRowHeights(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //Set scroll bar policies.
    horizontalScrollBar()->setSingleStep(5);
    horizontalScrollBar()->setPageStep(5);
    verticalScrollBar()->setSingleStep(4);
    verticalScrollBar()->setPageStep(4);

    //Set animation header.
    KNMusicTreeViewHeader *header=new KNMusicTreeViewHeader(this);
    setHeader(header);

    //Link with theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicTreeViewBase::onActionThemeUpdate);
}

void KNMusicTreeViewBase::enterEvent(QEvent *event)
{
    //Do original enter event.
    QTreeView::enterEvent(event);
    //Check if animate switch is on.
    if(m_animate)
    {
        //Start mouse in anime.
        startAnime(m_mouseIn);
    }
}

void KNMusicTreeViewBase::leaveEvent(QEvent *event)
{
    //Do original leave event.
    QTreeView::leaveEvent(event);
    //Check if animate switch is on.
    if(m_animate)
    {
        //Start mouse leave anime.
        startAnime(m_mouseOut);
    }
}

void KNMusicTreeViewBase::drawRow(QPainter *painter,
                                  const QStyleOptionViewItem &options,
                                  const QModelIndex &index) const
{
    //Check if this row is odd or oven, and then take one row to have
    //alternative background.
    if(index.row() & 1)
    {
        //Draw the alternative background.
        painter->fillRect(QRect(options.rect.x(),
                                options.rect.y(),
                                width(),
                                options.rect.height()),
                          palette().color(QPalette::AlternateBase));
    }
    //Paint the other parts of the row.
    QTreeView::drawRow(painter, options, index);
}

void KNMusicTreeViewBase::onActionThemeUpdate()
{
    //Get the new palette from theme manager, and set it.
    setPalette(knTheme->getPalette(objectName()));
    //Update the palette.
    onActionMouseInOut(0);
}

void KNMusicTreeViewBase::onActionMouseInOut(const int &frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Update the text palette color.
    QColor color=pal.color(QPalette::Text);
    color.setAlpha(FontBase+(frame<<1));
    pal.setColor(QPalette::Text, color);
    //Update the alternating base color.
    color=pal.color(QPalette::AlternateBase);
    color.setAlpha(frame);
    pal.setColor(QPalette::AlternateBase, color);
    //Update the button color.
    color=pal.color(QPalette::Button);
    color.setHsv(color.hue(), color.saturation(), frame<<1);
    pal.setColor(QPalette::Button, color);
    //Set the palette.
    setPalette(pal);
}

inline QTimeLine *KNMusicTreeViewBase::generateTimeLine(const int &endFrame)
{
    //Generate the time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configure the time line.
    timeLine->setEndFrame(endFrame);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(10);
    //Link the time line.
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMusicTreeViewBase::onActionMouseInOut);
    //Return the time line.
    return timeLine;
}

inline void KNMusicTreeViewBase::startAnime(QTimeLine *timeLine)
{
    //Stop all the animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set the parameter of the time line.
    timeLine->setStartFrame(palette().color(QPalette::AlternateBase).alpha());
    //Start the time line.
    timeLine->start();
}

bool KNMusicTreeViewBase::animate() const
{
    return m_animate;
}

void KNMusicTreeViewBase::setAnimate(bool animate)
{
    m_animate = animate;
}
