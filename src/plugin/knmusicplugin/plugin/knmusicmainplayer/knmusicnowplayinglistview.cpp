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
#include <QScrollBar>
#include <QTimeLine>

#include "sao/knsaostyle.h"
#include "knthememanager.h"

#include "knmusicmodel.h"
#include "knmusicglobal.h"
#include "knmusicproxymodel.h"
#include "knmusicnowplayingbase.h"
#include "knmusicnowplayingdelegate.h"

#include "knmusicnowplayinglistview.h"

using namespace MusicUtil;

#define MaxOpacity 0x20
#define ScrollBarWidth 10
#define ScrollBarSpacing 1

KNMusicNowPlayingListView::KNMusicNowPlayingListView(QWidget *parent) :
    QListView(parent),
    m_vScrollBar(new QScrollBar(Qt::Vertical, this)),
    m_mouseAnime(new QTimeLine(200, this)),
    m_currentFrame(0)
{
    setObjectName("NowPlayingListView");
    //Set item delegate.
    setItemDelegate(new KNMusicNowPlayingDelegate(this));
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(16);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMusicNowPlayingListView::onActionMouseInOut);
    //Add the widget the theme manager.
    knTheme->registerWidget(this);
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

    //Link the tree view signal and slot.
    connect(this, &KNMusicNowPlayingListView::activated,
            this, &KNMusicNowPlayingListView::onActionActivate);

    //Update the scroll bar.
    onActionMouseInOut(0);
}

KNMusicProxyModel *KNMusicNowPlayingListView::proxyModel()
{
    //Give back the model of this list view.
    return static_cast<KNMusicProxyModel *>(model());
}

KNMusicModel *KNMusicNowPlayingListView::musicModel()
{
    //Check out the proxy model.
    return proxyModel()==nullptr?nullptr:proxyModel()->musicModel();
}

void KNMusicNowPlayingListView::showEvent(QShowEvent *event)
{
    //Show the widget.
    QListView::showEvent(event);
    //Check out whether the proxy model is null or not.
    //Check whether the music model is null or not.
    //Check the playing index.
    if(proxyModel()==nullptr || musicModel()==nullptr ||
            (!musicModel()->playingIndex().isValid()))
    {
        //Give up.
        return;
    }
    //Get the playing proxy index..
    QModelIndex playingProxyIndex=
            proxyModel()->mapFromSource(musicModel()->playingIndex());
    //Scroll to the index.
    scrollTo(playingProxyIndex, PositionAtCenter);
    //Set current index to the playing proxy index.
    setCurrentIndex(playingProxyIndex);
    //Update the scroll bar position.
    updateVerticalScrollBarGeometry();
}

void KNMusicNowPlayingListView::enterEvent(QEvent *event)
{
    //Do original enter event.
    QListView::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KNMusicNowPlayingListView::leaveEvent(QEvent *event)
{
    //Do original enter event.
    QListView::leaveEvent(event);
    //Start mouse in anime.
    startAnime(0);
}

void KNMusicNowPlayingListView::resizeEvent(QResizeEvent *event)
{
    //Resize the tree view widget.
    QListView::resizeEvent(event);
    //Update the scroll bar position.
    updateVerticalScrollBarGeometry();
}

void KNMusicNowPlayingListView::onActionActivate(const QModelIndex &index)
{
    //Play the activate index.
    playIndex(index);
}

void KNMusicNowPlayingListView::onActionMouseInOut(int frame)
{
    //Save the frame.
    m_currentFrame=frame;
    //Calculate scroll bar alpha.
    int buttonAlpha=m_currentFrame<<2,
        baseAlpha=m_currentFrame*3;
    //Update the vertical scroll bar palette.
    QPalette pal=m_vScrollBar->palette();
    QColor color=pal.color(QPalette::Base);
    color.setAlpha(baseAlpha);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(buttonAlpha);
    pal.setColor(QPalette::Button, color);
    //Set the palette to vertical scroll bar.
    m_vScrollBar->setPalette(pal);
}

inline void KNMusicNowPlayingListView::playIndex(const QModelIndex &index)
{
    //Check the music row and the index is valid.
    if(musicModel()==nullptr || !index.isValid())
    {
        //Ignore those invalid request.
        return;
    }
    //Get the now playing from the music global.
    KNMusicNowPlayingBase *nowPlaying=knMusicGlobal->nowPlaying();
    //Check null.
    if(nowPlaying==nullptr)
    {
        return;
    }
    //Ask the now playing to play the index row.
    nowPlaying->playMusicRow(proxyModel(),
                             index.row(),
                             nowPlaying->playingTab());
}

inline void KNMusicNowPlayingListView::updateVerticalScrollBarGeometry()
{
    //Check scroll bar visiblility.
    if(m_vScrollBar->isVisible())
    {
        //Only vertical scroll bar is visible.
        m_vScrollBar->setGeometry(width()-ScrollBarWidth-ScrollBarSpacing,
                                  -10,
                                  ScrollBarWidth,
                                  height()+20);
    }
}

inline void KNMusicNowPlayingListView::startAnime(int endFrame)
{
    //Stop the mouse animations.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(m_currentFrame, endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
