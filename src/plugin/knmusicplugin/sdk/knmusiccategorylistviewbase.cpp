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
#include <QAction>
#include <QTimeLine>
#include <QScrollBar>
#include <QKeyEvent>

#include "sao/knsaostyle.h"
#include "knthememanager.h"
#include "kndpimanager.h"

#include "knmusiccategorysearch.h"

#include "knmusiccategorylistviewbase.h"

#define InBrightness 0x50
#define OutBrightness 0x17
#define TextBrightnessGap 0xAF
#define ButtonBrightnessGap -0x10
#define ScrollBarWidth 10
#define ScrollBarSpacing 1

KNMusicCategoryListViewBase::KNMusicCategoryListViewBase(QWidget *parent) :
    QListView(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_searchAnime(new QTimeLine(200, this)),
    m_scrollBar(new QScrollBar(this)),
    m_searchBox(new KNMusicCategorySearch(this))
{
    setObjectName("CategoryListViewBase");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0, 0, 0, 0);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setIconSize(knDpi->size(40, 40));
    setLineWidth(0);
    setLayoutMode(QListView::Batched); //For speed up.
    setMinimumWidth(knDpi->width(200));
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSpacing(0);
    setUniformItemSizes(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Set viewport properties.
    viewport()->setContentsMargins(0, 0, 0, 0);

    //Configure the scroll bar.
    m_scrollBar->setObjectName("MusicScrollBar");
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->hide();
    knTheme->registerWidget(m_scrollBar);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged,
            [=](int min, int max)
            {
                //Update the range first.
                m_scrollBar->setRange(min, max);
                //Check whether the scroll bar is still valid.
                m_scrollBar->setVisible(min!=max);
                //Update scrollbar state parameters.
                m_scrollBar->setPageStep(verticalScrollBar()->pageStep());
                m_scrollBar->setSingleStep(verticalScrollBar()->singleStep());
            });
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            [=](int value)
            {
                //Block the signal.
                m_scrollBar->blockSignals(true);
                //Reset the value.
                m_scrollBar->setValue(value);
                //Release the block
                m_scrollBar->blockSignals(false);
            });
    connect(m_scrollBar, &QScrollBar::valueChanged,
            verticalScrollBar(), &QScrollBar::setValue);

    //Configure the mouse anime time line.
    m_mouseAnime->setUpdateInterval(16);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMusicCategoryListViewBase::onActionMouseInOut);

    //Configure the search anime time line.
    m_searchAnime->setUpdateInterval(16);
    m_searchAnime->setEasingCurve(QEasingCurve::OutCubic);
    //Link time line.
    connect(m_searchAnime, &QTimeLine::frameChanged,
            this, &KNMusicCategoryListViewBase::onActionSearchInOut);

    //Hide the category search widget.
    m_searchBox->move(0, -m_searchBox->height());
    //Link requirements.
    connect(m_searchBox, &KNMusicCategorySearch::requireSearch,
            this, &KNMusicCategoryListViewBase::requireSearchCategory);
    connect(m_searchBox, &KNMusicCategorySearch::requireCancel,
            this, &KNMusicCategoryListViewBase::hideSearchBar);

    //Monitor the theme change signal.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicCategoryListViewBase::onActionPaletteChange);
    //Set the palette.
    onActionPaletteChange();
}

void KNMusicCategoryListViewBase::setSearchPlaceHolderText(const QString &text)
{
    //Set the text to the search place holder.
    m_searchBox->setPlaceHolderText(text);
}

void KNMusicCategoryListViewBase::enabledSearch()
{
    //Generate the search action.
    QAction *searchAction=new QAction(this);
    searchAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F));
    searchAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(searchAction, &QAction::triggered,
            this, &KNMusicCategoryListViewBase::onActionSearch);
    addAction(searchAction);
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
    startMouseAnime(InBrightness);
    //Do the original enter event.
    QListView::enterEvent(event);
}

void KNMusicCategoryListViewBase::leaveEvent(QEvent *event)
{
    //Start the mouse out anime.
    startMouseAnime(OutBrightness);
    //Do the original leave event.
    QListView::leaveEvent(event);
}

void KNMusicCategoryListViewBase::resizeEvent(QResizeEvent *event)
{
    //Resize the list view.
    QListView::resizeEvent(event);
    //Resize the search box widget width.
    m_searchBox->setFixedWidth(verticalScrollBar()->isVisible()?
                                   width()-verticalScrollBar()->width():
                                   width());
    //Resize the scroll bar position.
    m_scrollBar->setGeometry(width()-
                             knDpi->width(ScrollBarWidth+ScrollBarSpacing),
                             m_searchBox->geometry().bottom(),
                             knDpi->width(ScrollBarWidth),
                             height()-m_searchBox->geometry().bottom());
}

void KNMusicCategoryListViewBase::keyPressEvent(QKeyEvent *event)
{
    //Check out the current key.
    if(event->key()==Qt::Key_Escape)
    {
        //Hide the search box.
        hideSearchBar();
    }
    //Do the original key press event.
    QListView::keyPressEvent(event);
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

void KNMusicCategoryListViewBase::onActionSearch()
{
    //Give the focus on the search box.
    m_searchBox->setFocus();
    //Show the search widget on the widget via animation.
    startSearchAnime(m_searchBox->height());
}

void KNMusicCategoryListViewBase::hideSearchBar()
{
    //Set the focus on the list view.
    setFocus();
    //Clear the search box.
    m_searchBox->clear();
    //Hide the search widget on the widget via animation.
    startSearchAnime(0);
}

void KNMusicCategoryListViewBase::onActionMouseInOut(int frame)
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
    //Update the scroll bar palette.
    QPalette pal=m_scrollBar->palette();
    QColor color=pal.color(QPalette::Base);
    color.setAlpha((frame-OutBrightness)>>1);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha((frame-OutBrightness)<<1);
    pal.setColor(QPalette::Button, color);
    //Set the palette to scroll bar.
    m_scrollBar->setPalette(pal);
}

void KNMusicCategoryListViewBase::onActionSearchInOut(int frame)
{
    //Update the margin of the category view.
    setViewportMargins(0, frame, 0, 0);
    //Move the search box.
    m_searchBox->move(0, frame-m_searchBox->height());
}

inline void KNMusicCategoryListViewBase::startMouseAnime(int endFrame)
{
    //Stop mouse time line.
    m_mouseAnime->stop();
    //Configure the time line.
    m_mouseAnime->setFrameRange(m_backgroundColor.value(), endFrame);
    //Start the time line.
    m_mouseAnime->start();
}

void KNMusicCategoryListViewBase::startSearchAnime(int endFrame)
{
    //Stop search anime.
    m_searchAnime->stop();
    //Configure the time line.
    m_searchAnime->setFrameRange(m_searchBox->height()+m_searchBox->y(),
                                 endFrame);
    //Start up the anime.
    m_searchAnime->start();
}
