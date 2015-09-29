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
#define ButtonBrightnessGap -0x10

KNMusicCategoryListViewBase::KNMusicCategoryListViewBase(QWidget *parent) :
    QListView(parent),
    m_mouseAnime(new QTimeLine(200, this))
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
    setSpacing(0);
    setUniformItemSizes(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //Set viewport properties.
    viewport()->setContentsMargins(0, 0, 0, 0);

    //Configure the mouse anime time line.
    m_mouseAnime->setUpdateInterval(10);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMusicCategoryListViewBase::onActionMouseInOut);

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
    startAnime(InBrightness);
    //Do the original enter event.
    QListView::enterEvent(event);
}

void KNMusicCategoryListViewBase::leaveEvent(QEvent *event)
{
    //Start the mouse out anime.
    startAnime(OutBrightness);
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
    //Update the button color.
    m_buttonColor=m_palette.color(QPalette::Button);
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
    //Update the button color's brightness.
    m_buttonColor.setHsv(m_buttonColor.hue(),
                         m_buttonColor.saturation(),
                         frame+ButtonBrightnessGap);
    //Update the palette.
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Button, m_buttonColor);
    m_palette.setColor(QPalette::Text, m_textColor);
    //Set the palette.
    setPalette(m_palette);
}

inline void KNMusicCategoryListViewBase::startAnime(const int &endFrame)
{
    //Stop all the time line.
    m_mouseAnime->stop();
    //Configure the time line.
    m_mouseAnime->setFrameRange(m_backgroundColor.value(), endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
