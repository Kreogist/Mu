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
#include <QTimeLine>

#include "knthememanager.h"

#include "knmusiccategorylistviewbase.h"

#define InBrightness 0x50
#define OutBrightness 0x17
#define TextBrightnessGap 0xAF

KNMusicCategoryListViewBase::KNMusicCategoryListViewBase(QWidget *parent) :
    QListView(parent),
    m_mouseIn(generateTimeLine(InBrightness)),
    m_mouseOut(generateTimeLine(OutBrightness))
{
    setObjectName("CategoryListViewBase");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0, 0, 0, 0);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setIconSize(QSize(40, 40));
    setLineWidth(0);
    setLayoutMode(QListView::Batched); //For speed up.
    setMinimumWidth(200);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSpacing(2);
    setUniformItemSizes(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //Set viewport properties.
    viewport()->setContentsMargins(0, 0, 0, 0);
    //Monitor the theme change signal.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicCategoryListViewBase::onActionPaletteChange);
    //Set the palette.
    onActionPaletteChange();
}

void KNMusicCategoryListViewBase::enabledDragDrop()
{
    //Enabled viewport drag and drop.
    viewport()->setAcceptDrops(true);
    //Enabled the drag and drop properties.
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDropIndicatorShown(true);
}

void KNMusicCategoryListViewBase::enterEvent(QEvent *event)
{
    //Start the mouse in anime.
    startAnime(m_mouseIn);
    //Do the original enter event.
    QListView::enterEvent(event);
}

void KNMusicCategoryListViewBase::leaveEvent(QEvent *event)
{
    //Start the mouse out anime.
    startAnime(m_mouseOut);
    //Do the original leave event.
    QListView::leaveEvent(event);
}

void KNMusicCategoryListViewBase::onActionPaletteChange()
{
    //Get the new palette.
    m_palette=knTheme->getPalette(objectName());
    //Update the background color.
    m_backgroundColor=m_palette.color(QPalette::Base);
    //Update the text color.
    m_textColor=m_palette.color(QPalette::Text);
    //Use the mouse in out to update the palette.
    onActionMouseInOut(OutBrightness);
}

void KNMusicCategoryListViewBase::onActionMouseInOut(const int &frame)
{
    //Update the background color's brightness.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             frame);
    //Update the text color's brightness.
    m_textColor.setHsv(m_textColor.hue(),
                       m_textColor.saturation(),
                       frame+TextBrightnessGap);
    //Update the palette.
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Text, m_textColor);
    //Set the palette.
    setPalette(m_palette);
}

inline QTimeLine *KNMusicCategoryListViewBase::generateTimeLine(
        const int &endFrame)
{
    //Generate the time line,
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Set the end frame.
    timeLine->setEndFrame(endFrame);
    //Configure the time line.
    timeLine->setUpdateInterval(10);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    //Link the time line.
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMusicCategoryListViewBase::onActionMouseInOut);
    //Give back the time line.
    return timeLine;
}

inline void KNMusicCategoryListViewBase::startAnime(QTimeLine *timeLine)
{
    //Stop all the animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Configure the time line.
    timeLine->setStartFrame(m_backgroundColor.value());
    //Start the time line.
    timeLine->start();
}

