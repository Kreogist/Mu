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
#include <QWheelEvent>

#include "sao/knsaostyle.h"
#include "knthememanager.h"

#include "knmusicstorehomelistmodel.h"

#include "knmusicstorehomeitemview.h"

#define ScrollBarWidth          10
#define MaxOpacity              0x80

KNMusicStoreHomeItemView::KNMusicStoreHomeItemView(QWidget *parent) :
    QAbstractItemView(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_scrollBar(new QScrollBar(Qt::Horizontal, this)),
    m_listModel(nullptr)
{
    //Set properties.
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Configure the scroll bar.
    m_scrollBar->setObjectName("MusicStoreScrollBar");
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->setFixedHeight(ScrollBarWidth);
    m_scrollBar->hide();
    knTheme->registerWidget(m_scrollBar);
    connect(horizontalScrollBar(), &QScrollBar::rangeChanged,
            [=](int min, int max)
            {
                //Update the range first.
                m_scrollBar->setRange(min, max);
                //Check whether the scroll bar is still valid.
                m_scrollBar->setVisible(min!=max);
            });
    connect(horizontalScrollBar(), &QScrollBar::valueChanged,
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
            horizontalScrollBar(), &QScrollBar::setValue);

    //Configure the timeline.
    m_mouseAnime->setUpdateInterval(33);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMusicStoreHomeItemView::onMouseInOut);
}

void KNMusicStoreHomeItemView::scrollTo(const QModelIndex &index,
                                        QAbstractItemView::ScrollHint hint)
{
    //Ignore the scroll to event.
    Q_UNUSED(index)
    Q_UNUSED(hint)
}

QModelIndex KNMusicStoreHomeItemView::moveCursor(
        QAbstractItemView::CursorAction cursorAction,
        Qt::KeyboardModifiers modifiers)
{
    //Ignore the move cursor.
    Q_UNUSED(cursorAction)
    Q_UNUSED(modifiers)
    return QModelIndex();
}

void KNMusicStoreHomeItemView::setSelection(
        const QRect &rect,
        QItemSelectionModel::SelectionFlags command)
{
    //Ignore the selection operation.
    Q_UNUSED(rect)
    Q_UNUSED(command)
}

void KNMusicStoreHomeItemView::setModel(QAbstractItemModel *model)
{
    //Set the model to view.
    QAbstractItemView::setModel(model);
    //This model must be a list model.
    m_listModel=static_cast<KNMusicStoreHomeListModel *>(model);
}

void KNMusicStoreHomeItemView::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QAbstractItemView::resizeEvent(event);
    //Update the scroll bar position.
    m_scrollBar->setGeometry(0, height()-ScrollBarWidth,
                             width(), ScrollBarWidth);
}

int KNMusicStoreHomeItemView::horizontalOffset() const
{
    //The horizontal offset is the scroll bar value.
    return horizontalScrollBar()->value();
}

int KNMusicStoreHomeItemView::verticalOffset() const
{
    //The vertical offset is the scroll bar value. However, it is const 0.
    return 0;
}

bool KNMusicStoreHomeItemView::isIndexHidden(const QModelIndex &index) const
{
    //Treat all the index visible.
    Q_UNUSED(index)
    return false;
}

QRegion KNMusicStoreHomeItemView::visualRegionForSelection(
        const QItemSelection &selection) const
{
    Q_UNUSED(selection)
    //No selection for the view.
    return QRegion();
}

void KNMusicStoreHomeItemView::enterEvent(QEvent *event)
{
    //Enter the list view.
    QAbstractItemView::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KNMusicStoreHomeItemView::leaveEvent(QEvent *event)
{
    //Leave the list view.
    QAbstractItemView::leaveEvent(event);
    //Start mouse leave anime.
    startAnime(0);
}

void KNMusicStoreHomeItemView::wheelEvent(QWheelEvent *event)
{
    //Check the scroll direction.
    if(event->orientation()==Qt::Vertical)
    {
        //Ignore the event.
        event->ignore();
        //Wheel complete.
        return;
    }
    //Continue for the horizontal movement.
    QAbstractItemView::wheelEvent(event);
}

void KNMusicStoreHomeItemView::onMouseInOut(int frame)
{
    //Update the scroll bar color.
    QPalette pal=m_scrollBar->palette();
    QColor color=pal.color(QPalette::Base);
    color.setAlpha(frame>>2);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(frame);
    pal.setColor(QPalette::Button, color);
    //Set the palette to scroll bar.
    m_scrollBar->setPalette(pal);
}

inline void KNMusicStoreHomeItemView::startAnime(int endFrame)
{
    //Stop the mouse animations.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(
                m_scrollBar->palette().color(QPalette::Button).alpha(),
                endFrame);
    //Start the time line.
    m_mouseAnime->start();
}

QScrollBar *KNMusicStoreHomeItemView::scrollBar()
{
    return m_scrollBar;
}

KNMusicStoreHomeListModel *KNMusicStoreHomeItemView::listModel() const
{
    return m_listModel;
}
