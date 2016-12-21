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
#include <QPainter>
#include <QWheelEvent>

#include "sao/knsaostyle.h"
#include "knthememanager.h"

#include "knmusicstorehomelistmodel.h"

#include "knmusicstorehomealbumview.h"

#include <QDebug>

#define AlbumArtSize            125
#define AlbumHorizontalSpacing  20
#define AlbumHorizontalSize     145
#define AlbumVerticalSize       191
#define AlbumViewHeight         390
#define AlbumAreaHeight         172
#define AlbumAreaWidth          125
#define AlbumTitleSpacing       9
#define AlbumTextAreaHeight     44
#define ScrollBarWidth          10
#define ShadowSize              151
#define ShadowIncrease          12
#define PaintTopMargin          7
#define MaxOpacity              0x80

KNMusicStoreHomeAlbumView::KNMusicStoreHomeAlbumView(QWidget *parent) :
    QAbstractItemView(parent),
    m_noAlbumArtCache(QPixmap(":/plugin/music/public/noalbum.png")),
    m_albumShadow(QPixmap(ShadowSize, ShadowSize)),
    m_mouseAnime(new QTimeLine(200, this)),
    m_listModel(nullptr),
    m_scrollBar(new QScrollBar(Qt::Horizontal, this))
{
    setObjectName("MusicStoreHomeNewAlbum");
    //Set properties.
    setFixedHeight(AlbumViewHeight);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Configure the scroll bar.
    horizontalScrollBar()->setSingleStep(AlbumHorizontalSize>>2);
    horizontalScrollBar()->setPageStep(AlbumHorizontalSize>>1);
    //Configure the scroll bar.
    m_scrollBar->setObjectName("MusicStoreScrollBar");
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->setSingleStep(100);
    m_scrollBar->setPageStep(AlbumHorizontalSize<<3);
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
    //Initial the cache.
    m_noAlbumArtCache=m_noAlbumArtCache.scaled(AlbumArtSize, AlbumArtSize,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
    //Initial the album shadow.
    QPixmap shadowSource("://public/shadow.png");
    //Initial the shadow pixmap.
    m_albumShadow.fill(QColor(255, 255, 255, 0));
    //Configure the timeline.
    //Configure the timeline.
    m_mouseAnime->setUpdateInterval(33);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMusicStoreHomeAlbumView::onMouseInOut);
    //Prepare the parameters.
    int sourceSize=shadowSource.width(),
        blockSize=sourceSize/3,
        blockSize2x=blockSize<<1,
        destinationWidth=ShadowSize-blockSize2x,
        destinationHeight=ShadowSize-blockSize2x;
    //Initial the antialiasing painter.
    QPainter painter(&m_albumShadow);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    painter.setOpacity(0.8);
    //Draw Top-left shadow.
    painter.drawPixmap(QRect(0,0,blockSize,blockSize),
                       shadowSource,
                       QRect(0,0,blockSize,blockSize));
    //Draw Top-Middle shadow.
    painter.drawPixmap(QRect(blockSize,0,destinationWidth-blockSize,blockSize),
                       shadowSource,
                       QRect(blockSize,0,blockSize,blockSize));
    //Draw Top-Right shadow.
    painter.drawPixmap(QRect(destinationWidth,0,blockSize2x, blockSize),
                       shadowSource,
                       QRect(blockSize,0,blockSize2x,blockSize));
    //Draw Middle-Left shadow.
    painter.drawPixmap(QRect(0,blockSize,blockSize,destinationHeight-blockSize),
                       shadowSource,
                       QRect(0,blockSize,blockSize,blockSize));
    //Draw Middle-Right shadow.
    painter.drawPixmap(QRect(destinationWidth+blockSize,
                             blockSize,
                             blockSize,
                             destinationHeight),
                       shadowSource,
                       QRect(blockSize2x,blockSize,blockSize,blockSize));
    //Draw Left-Bottom shadow.
    painter.drawPixmap(QRect(0,destinationHeight,blockSize, blockSize2x),
                       shadowSource,
                       QRect(0,blockSize,blockSize,blockSize2x));
    //Draw Middle-Bottom shadow.
    painter.drawPixmap(QRect(blockSize,
                             destinationHeight+blockSize,
                             destinationWidth,
                             blockSize),
                       shadowSource,
                       QRect(blockSize,blockSize2x,blockSize,blockSize));
    //Draw Right-Buttom shadow.
    painter.drawPixmap(QRect(destinationWidth+blockSize,
                             destinationHeight+blockSize,
                             blockSize,
                             blockSize),
                       shadowSource,
                       QRect(blockSize2x,blockSize2x,blockSize,blockSize));
    painter.end();

    //Link theme manager.
    knTheme->registerWidget(this);
}

QModelIndex KNMusicStoreHomeAlbumView::indexAt(const QPoint &point) const
{
    //Check model is nullptr or not.
    if(!m_listModel)
    {
        //For null model, return null value.
        return QModelIndex();
    }
    //Calculate the column and row.
    int viewX=point.x()+horizontalScrollBar()->value(),
        targetColumn=viewX/AlbumHorizontalSize,
        targetRow=point.y()>AlbumVerticalSize;
    //For a single item, check the target width and column position, make sure
    //that it is in the position.
    if((!model()) || viewX > AlbumAreaWidth || point.y() > AlbumAreaHeight)
    {
        //Outside the part, an invalid index is set.
        return QModelIndex();
    }
    //The item is listed from left to right, from top to bottom.
    return m_listModel->index(targetColumn * 2 + targetRow, 0);
}

void KNMusicStoreHomeAlbumView::scrollTo(const QModelIndex &index,
                                         QAbstractItemView::ScrollHint hint)
{
    Q_UNUSED(index)
    Q_UNUSED(hint)
    //Ignore the scroll to event.
}

QRect KNMusicStoreHomeAlbumView::visualRect(const QModelIndex &index) const
{
    //Calculate the real position.
    int xPos=(index.row()>>1)*AlbumHorizontalSize-
            horizontalScrollBar()->value();
    //Check the validation of the position.
    if(xPos<-AlbumAreaWidth || xPos>width())
    {
        //Nothing is visible.
        return QRect();
    }
    //Calculate the vertical position.
    int yPos=(index.column()&1)*AlbumVerticalSize;
    //Part or all the part of the album is visible.
    if(xPos<0)
    {
        //Only a part of album is visible, it is at the left side of the widget.
        return QRect(0, yPos, AlbumAreaWidth+xPos, AlbumAreaHeight);
    }
    if(xPos>width()-AlbumAreaWidth)
    {
        //Only a part of album is visible, it is at the right side of the
        //widget.
        return QRect(xPos, yPos, width()-xPos, AlbumAreaHeight);
    }
    //All the part of the album is visible.
    return QRect(xPos, yPos, AlbumAreaWidth, AlbumAreaHeight);
}

void KNMusicStoreHomeAlbumView::setModel(QAbstractItemModel *model)
{
    //Set the model to view.
    QAbstractItemView::setModel(model);
    //This model must be a list model.
    m_listModel=static_cast<KNMusicStoreHomeListModel *>(model);
}

QModelIndex KNMusicStoreHomeAlbumView::moveCursor(
        QAbstractItemView::CursorAction cursorAction,
        Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(cursorAction)
    Q_UNUSED(modifiers)
    //Ignore the move cursor.
    return QModelIndex();
}

void KNMusicStoreHomeAlbumView::setSelection(
        const QRect &rect,
        QItemSelectionModel::SelectionFlags command)
{
    Q_UNUSED(rect)
    Q_UNUSED(command)
}

void KNMusicStoreHomeAlbumView::paintEvent(QPaintEvent *event)
{
    //Check the model pointer, if the model pointer is null, ignore the paint
    //event.
    if(!m_listModel)
    {
        //Ignore to paint null model.
        return;
    }
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(viewport());
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    painter.setPen(palette().color(QPalette::WindowText));
    //Calculate the start index.
    int currentRow=(horizontalScrollBar()->value()/AlbumHorizontalSize)<<1,
        //Calculate the start x.
        currentX=(currentRow>>1)*AlbumHorizontalSize-
            horizontalScrollBar()->value(),
        currentY=PaintTopMargin;
    //Start to paint the album item.
    while(currentRow<m_listModel->rowCount())
    {
        //Draw the pixmap first.
        const KNMusicStoreHomeItem &albumItem=
                m_listModel->albumItem(currentRow);
        //Check the album art is null or not.
        //Draw the pixmap shadow first.
        painter.drawPixmap(currentX-ShadowIncrease, currentY-ShadowIncrease,
                           m_albumShadow);
        //Draw the album art pixmap.
        painter.drawPixmap(currentX, currentY,
                           albumItem.artwork.isNull()?
                               m_noAlbumArtCache:
                               albumItem.artwork);
        //Get the title text.
        QString titleText=fontMetrics().elidedText(albumItem.title,
                                                   Qt::ElideRight,
                                                   AlbumAreaWidth<<1);
        //Prepare the title area.
        QRect titleArea=QRect(currentX, currentY+AlbumArtSize+AlbumTitleSpacing,
                              AlbumAreaWidth, AlbumTextAreaHeight);
        //Calculate the real title area.
        titleArea=fontMetrics().boundingRect(titleArea,
                                             Qt::AlignLeft | Qt::AlignTop |
                                             Qt::TextWordWrap,
                                             titleText);
        //Draw the album data.
        painter.drawText(titleArea,
                         Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                         titleText);
        //Change to subtitle opacity.
        painter.setOpacity(0.5);
        //Draw the album text.
        int subtitleHeight=AlbumTextAreaHeight-titleArea.height();
        painter.drawText(QRect(currentX,
                               titleArea.y()+titleArea.height(),
                               AlbumAreaWidth,
                               subtitleHeight),
                         Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                         fontMetrics().elidedText(
                             albumItem.subheading,
                             Qt::ElideRight,
                             subtitleHeight/fontMetrics().height()*
                             AlbumAreaWidth));
        //Reset the opacity.
        painter.setOpacity(1.0);
        //Change the position.
        if(currentY==PaintTopMargin)
        {
            //Increase the Y position only.
            currentY+=AlbumVerticalSize;
        }
        else
        {
            //Reset Y position.
            currentY=PaintTopMargin;
            //Increase X position.
            currentX+=AlbumHorizontalSize;
        }
        //Increase the row.
        ++currentRow;
    }
}

void KNMusicStoreHomeAlbumView::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QAbstractItemView::resizeEvent(event);
    //Update the scroll bar position.
    m_scrollBar->setGeometry(0, height()-ScrollBarWidth,
                             width(), ScrollBarWidth);
}

int KNMusicStoreHomeAlbumView::horizontalOffset() const
{
    //The horizontal offset is the scroll bar value.
    return horizontalScrollBar()->value();
}

int KNMusicStoreHomeAlbumView::verticalOffset() const
{
    //The vertical offset is the scroll bar value. However, it is const 0.
    return 0;
}

bool KNMusicStoreHomeAlbumView::isIndexHidden(const QModelIndex &index) const
{
    //Treat all the index visible.
    Q_UNUSED(index)
    return false;
}

QRegion KNMusicStoreHomeAlbumView::visualRegionForSelection(
        const QItemSelection &selection) const
{
    Q_UNUSED(selection)
    //No selection for the view.
    return QRegion();
}

void KNMusicStoreHomeAlbumView::enterEvent(QEvent *event)
{
    //Enter the list view.
    QAbstractItemView::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KNMusicStoreHomeAlbumView::leaveEvent(QEvent *event)
{
    //Leave the list view.
    QAbstractItemView::leaveEvent(event);
    //Start mouse leave anime.
    startAnime(0);
}

void KNMusicStoreHomeAlbumView::wheelEvent(QWheelEvent *event)
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

void KNMusicStoreHomeAlbumView::updateGeometries()
{
    //Update the range of the horizontal scroll bar.
    horizontalScrollBar()->setRange(0,
                                    (AlbumHorizontalSize<<4)-
                                    AlbumHorizontalSpacing-width());
}

void KNMusicStoreHomeAlbumView::onMouseInOut(int frame)
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

inline void KNMusicStoreHomeAlbumView::startAnime(int endFrame)
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
