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
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QTimeLine>

#include "knthememanager.h"

#include "knmusiccategoryproxymodel.h"
#include "knmusicalbummodel.h"

#include "knmusicalbumview.h"

#include <QDebug>

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent),
    m_shadowSource(QPixmap("://public/shadow.png")),
    m_albumArtShadow(QPixmap()),
    m_nullIndex(QModelIndex()),
    m_selectedIndex(m_nullIndex),
    m_mouseDownIndex(m_nullIndex),
    m_scrollAnime(new QTimeLine(200, this)),
    m_proxyModel(nullptr),
    m_model(nullptr),
    m_spacing(30),
    m_itemMinimalWidth(124),
    minimalWidth(m_itemMinimalWidth+m_spacing),
    m_lineCount(0),
    m_imageTextSpacing(5),
    m_itemHeight(154),
    m_itemWidth(134),
    m_itemSpacingHeight(m_spacing+m_itemHeight),
    m_itemSpacingWidth(m_spacing+m_itemWidth),
    m_maxColumnCount(0),
    m_shadowIncrease(15)
{
    setObjectName("MusicAlbumView");
    //Set properties.
    setFocusPolicy(Qt::WheelFocus);
    //Set default scrollbar properties.
    verticalScrollBar()->setRange(0, 0);

    //Set the palette.
    knTheme->registerWidget(this);

    //Initial the timeline.
    m_scrollAnime->setUpdateInterval(10);
    m_scrollAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_scrollAnime, &QTimeLine::frameChanged,
            verticalScrollBar(), &QScrollBar::setValue);

    //Link the scrolling event.
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
                this, &KNMusicAlbumView::onActionScrolling);

    //Update the painting parameters.
    updateUIElements();
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{

    //Calculate the point content position and the line of the point.
    int pointContentY=verticalScrollBar()->value()+point.y(),
        itemLine=pointContentY/m_itemSpacingHeight;
    //Check whether mouse click on a row spacing part.
    if(pointContentY-itemLine*m_itemSpacingHeight<m_spacing)
    {
        return m_nullIndex;
    }
    //Calculate the column of the position.
    int itemColumn=point.x()/m_itemSpacingWidth,
        pointXInItem=point.x()-itemColumn*m_itemSpacingWidth;
    //Check whether mouse click on a column spacing part.
    if(pointXInItem<m_spacing || pointXInItem>m_spacing+m_itemWidth)
    {
        return m_nullIndex;
    }
    //Calculate the item category index.
    int categoryRow=itemLine*m_maxColumnCount+itemColumn;
    //Check if the category row vaild.
    //We should check the category proxy model, because the point is a display
    //variable.
    return (categoryRow>-1 && categoryRow<m_proxyModel->rowCount())?
                m_proxyModel->index(categoryRow, 0, rootIndex()):
                m_nullIndex;
}

void KNMusicAlbumView::scrollTo(const QModelIndex &index,
                                QAbstractItemView::ScrollHint hint)
{
    //Check the index and the validation of max column count.
    if(!index.isValid() || m_maxColumnCount==0 ||
            //Check whether we need to move the vertical scroll bar
            (hint==QAbstractItemView::EnsureVisible &&
                  rect().contains(visualRect(index), true)))
    {
        return;
    }
    //Use timeline to move to the position.
    m_scrollAnime->stop();
    m_scrollAnime->setFrameRange(verticalScrollBar()->value(),
                                 indexScrollBarValue(index, hint));
    m_scrollAnime->start();
    //Update the painting.
    viewport()->update();
}

void KNMusicAlbumView::locateTo(const QModelIndex &index,
                                QAbstractItemView::ScrollHint hint)
{
    //Check the index and the max column count.
    if(!index.isValid() || m_maxColumnCount==0 ||
            //Check whether we need to move the vertical scroll bar.
            (hint==QAbstractItemView::EnsureVisible &&
             rect().contains(visualRect(index), true)))
    {
        return;
    }
    //Use timeline to move to the position.
    verticalScrollBar()->setValue(indexScrollBarValue(index, hint));
    //Update.
    viewport()->update();
}

QRect KNMusicAlbumView::visualRect(const QModelIndex &index) const
{
    //Get the item content rect.
    QRect indexRect=itemContentRect(index);
    //If the rect is vaild, remove the value of the scroll bar to get the
    //position in the rect.
    return indexRect.isValid()?
                QRect(indexRect.left()-horizontalScrollBar()->value(),
                      indexRect.top()-verticalScrollBar()->value(),
                      indexRect.width(),
                      indexRect.height()):
                QRect();
}

void KNMusicAlbumView::setModel(QAbstractItemModel *model)
{
    //Save the proxy model and the category model.
    m_proxyModel=static_cast<KNMusicCategoryProxyModel *>(model);
    //Check out the proxy model.
    if(m_proxyModel==nullptr)
    {
        return;
    }
    //Get the album model.
    m_model=static_cast<KNMusicAlbumModel *>(m_proxyModel->sourceModel());
    //Set the model.
    QAbstractItemView::setModel(m_proxyModel);
    //Update the geometries.
    updateGeometries();
}

void KNMusicAlbumView::clearSelection()
{
    //When complete the fold, set the selected index to null.
    setCurrentIndex(m_nullIndex);
    m_selectedIndex=m_nullIndex;
    //Update the viewport.
    viewport()->update();
}

void KNMusicAlbumView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Get the option view item.
    QStyleOptionViewItem option=viewOptions();
    //Check if we need to paint the background.
    if(autoFillBackground())
    {
        painter.fillRect(rect(), option.palette.base());
    }

    //Update the parameters of the view first.
    updateUIElements();
    //Check the model first.
    if(m_proxyModel==nullptr)
    {
        return;
    }
    //Get the row count.
    int albumCount=m_proxyModel->rowCount();
    //Check the album count first.
    if(albumCount==0)
    {
        return;
    }
    //Calculate the line count.
    m_lineCount=(albumCount+m_maxColumnCount-1)/m_maxColumnCount;
    //Generate the parameters.
    int currentColumn=0,
        currentLeft=m_spacing,
        currentLine=verticalScrollBar()->value()/m_itemSpacingHeight,
        currentTop=m_spacing+currentLine*m_itemSpacingHeight,
        currentRow=currentLine*m_maxColumnCount,
        heightSurplus=height()+m_itemSpacingHeight;
    /*
     *                currentColumn
     *                |
     *  +-------------------------------------------------+
     *  | +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ |
     *  | |   | |   | |   | |   | |   | |   | |   | |   | |
     *  | +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ |
     *  |                                                 |
     *  | +---+ +---+ xxxxx-currentTop                    |
     *  | |   | |   | xxxxx                               | - currentLine
     *  | +---+ +---+ xxxxx                               |
     *  |             | |                                 | -+
     *  | currentLeft-+ |                                 |  |
     *  |               +--QModelIndex(25252, 0)          |  |
     *  |                                |                |  |
     *  |                                currentRow       |  +-heightSurplus
     *  |                                                 |  |
     *  |                                                 |  |
     *  |                                                 |  |
     *  +-------------------------------------------------+ -+
     */
    //Change the origin of coordinate.
    painter.translate(0, -verticalScrollBar()->value());
    //Draw all the albums, until there's no album left, no height surplus.
    while(currentRow < albumCount && heightSurplus > 0)
    {
        //Get the source index of the item.
        QModelIndex proxyIndex=m_proxyModel->index(currentRow, 0);
        //If the source index is not the current index, then draw the album.
        if(m_proxyModel->mapToSource(proxyIndex)!=m_selectedIndex)
        {
            //Draw the unselect album.
            paintAlbum(painter, currentLeft, currentTop, proxyIndex);
        }
        //Add current row to the next album model row.
        currentRow++;
        //Add current column to the next position.
        currentColumn++;
        //Check if we need to move to next row.
        if(currentColumn>=m_maxColumnCount)
        {
            //Add current line.
            currentLine++;
            //Reset the column.
            currentColumn=0;
            //Change the position.
            currentLeft=m_spacing;
            currentTop+=m_itemSpacingHeight;
            heightSurplus-=m_itemSpacingHeight;
        }
        else
        {
            //Move to next column position.
            currentLeft+=m_itemSpacingWidth;
        }
    }
    //Update the scroll bar value.
    updateGeometries();
}

void KNMusicAlbumView::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QAbstractItemView::resizeEvent(event);
    //Resize the album detail.
    //!FIXME: Add codes here.
    //If the current index is not null, must ensure that we can display the
    //selected album.
    if(currentIndex().isValid())
    {
        locateTo(currentIndex());
    }
}

QModelIndex KNMusicAlbumView::moveCursor(
        QAbstractItemView::CursorAction cursorAction,
        Qt::KeyboardModifiers modifiers)
{
    //!FIXME: Add operation here.
    Q_UNUSED(cursorAction)
    Q_UNUSED(modifiers)
    return currentIndex();
}

int KNMusicAlbumView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int KNMusicAlbumView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool KNMusicAlbumView::isIndexHidden(const QModelIndex &index) const
{
    //Treat all the index visible.
    Q_UNUSED(index)
    return false;
}

void KNMusicAlbumView::setSelection(const QRect &rect,
                                    QItemSelectionModel::SelectionFlags command)
{
    //Simply select the top left position.
    selectionModel()->select(indexAt(QPoint(rect.x(), rect.y())),
                             command);
}

QRegion KNMusicAlbumView::visualRegionForSelection(
        const QItemSelection &selection) const
{
    //If there's no selection, of course nothing.
    if(selection.size()==0)
    {
        return QRect();
    }
    //Add the only select item to the region, return the region.
    QItemSelectionRange range=selection.at(0);
    QRegion region;
    region+=visualRect(indexAt(QPoint(range.top(), range.left())));
    return region;
}

void KNMusicAlbumView::mousePressEvent(QMouseEvent *event)
{
    //Do the original press event.
    QAbstractItemView::mousePressEvent(event);
    //Get the mouse down index.
    m_mouseDownIndex=indexAt(event->pos());

}

void KNMusicAlbumView::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the original press event.
    QAbstractItemView::mouseReleaseEvent(event);
    //Check whether the released pos index is the pressed index.
    if(m_mouseDownIndex==indexAt(event->pos()))
    {
        //Check the mouse pressed button is left button or not.
        if(event->button()==Qt::LeftButton)
        {
            displayAlbum(event->pos());
            return;
        }
    }
    //If goes here, we need to fold the expanded album.
    displayAlbum(QPoint(-1,-1));
}

void KNMusicAlbumView::updateGeometries()
{
    //Update the range of the vertical scroll bar.
    verticalScrollBar()->setRange(0,
                                  qMax(0,
                                       m_lineCount*m_itemSpacingHeight +
                                       m_spacing-height()));
    //Update the page and single step.
    verticalScrollBar()->setPageStep(m_itemSpacingHeight>>2);
    verticalScrollBar()->setSingleStep(m_itemSpacingHeight>>2);
}

void KNMusicAlbumView::onActionScrolling()
{
    //!FIXME: add codes here
}

void KNMusicAlbumView::displayAlbum(const QPoint &point)
{
    //!FIXME: add codes here.
}

inline int KNMusicAlbumView::indexScrollBarValue(
        const QModelIndex &index,
        QAbstractItemView::ScrollHint hint)
{
    //Get the top of index position, set it to scroll value.
    int topPositionValue=index.row()/m_maxColumnCount*m_itemSpacingHeight;
    //Change the item content Y according to the hint.
    switch(hint)
    {
    case QAbstractItemView::PositionAtTop:
        //No need to change.
        return topPositionValue;
    case QAbstractItemView::PositionAtCenter:
        //Reduce a half of the viewer height to move up.
        return topPositionValue-((height()-m_itemSpacingHeight)>>1);
    case QAbstractItemView::PositionAtBottom:
        //Reduce the whole viewer height to move up.
        return topPositionValue-height()+m_itemSpacingHeight;
        break;
    default:
        //Now, the index item must be a unvisible one in the viewer.
        //We have already has the top position, calculate the bottom position,
        //and calculate the distence of the current vertical scroll bar's value
        //to these two position.
        int bottomPositionValue=topPositionValue-height()+m_itemSpacingHeight;
        //If to the bottom is lesser than to top, change the value.
        return (qAbs(verticalScrollBar()->value()-bottomPositionValue)<
                qAbs(verticalScrollBar()->value()-topPositionValue))?
                    bottomPositionValue:
                    topPositionValue;
    }
}

inline QRect KNMusicAlbumView::itemContentRect(const QModelIndex &index) const
{
    //Check the index first.
    if(!index.isValid() || m_maxColumnCount==0)
    {
        return QRect();
    }
    //Calculate the item line.
    int itemLine=index.row()/m_maxColumnCount;
    //Calculate the rect.
    return QRect((index.row()-itemLine*m_maxColumnCount)*m_itemSpacingWidth+
                 m_spacing,
                 itemLine*m_itemSpacingHeight+m_spacing,
                 m_itemWidth,
                 m_itemHeight);
}

inline void KNMusicAlbumView::paintAlbum(QPainter &painter,
                                         const int &x,
                                         const int &y,
                                         const QModelIndex &index)
{
    //Ensure the index is vaild.
    if(!index.isValid())
    {
        return;
    }
    //Draw the shadow first.
    painter.drawPixmap(x-m_shadowIncrease,
                       y-m_shadowIncrease,
                       m_albumArtShadow);
    //Fill up the background.
    painter.fillRect(QRect(x,y,m_itemWidth,m_itemWidth),
                     QColor(0,0,0));
    //Render and draw the album art image.
    QPixmap albumArtImage=
            (m_proxyModel->data(
                 index,
                 Qt::DecorationRole).value<QPixmap>()).scaled(
                QSize(m_itemWidth,
                      m_itemWidth),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
    //Draw the album art.
    painter.drawPixmap(QRect(x+((m_itemWidth-albumArtImage.width())>>1),
                             y+((m_itemWidth-albumArtImage.height())>>1),
                             albumArtImage.width(),
                             albumArtImage.height()),
                       albumArtImage);
    //Get the option view item.
    QStyleOptionViewItem option=viewOptions();
    //Set the pen as the text color.
    QColor textColor=option.palette.color(QPalette::Text);
    painter.setPen(textColor);
    //Draw the album caption, which contains album name and artist.
    //Draw the album name first.
    int textY=y+m_itemWidth+m_imageTextSpacing;
    painter.drawText(QRect(x,
                           textY,
                           m_itemWidth,
                           fontMetrics().height()),
                     Qt::AlignLeft,
                     fontMetrics().elidedText(
                         index.data(Qt::DisplayRole).toString(),
                         Qt::ElideRight,
                         m_itemWidth));
    //Draw the album artist name.
    QString artistText=
            index.data(KNMusicAlbumModel::AlbumArtistRole).toString();
    //Check artist text is empty or not.
    if(artistText.isEmpty())
    {
        return;
    }
    //Make the color half opacity.
    textColor.setAlpha(textColor.alpha()>>1);
    //Set the color.
    painter.setPen(textColor);
    //Draw the text.
    painter.drawText(QRect(x,
                           textY+fontMetrics().height(),
                           m_itemWidth,
                           fontMetrics().height()),
                     Qt::AlignLeft,
                     fontMetrics().elidedText(artistText,
                                              Qt::ElideRight,
                                              m_itemWidth));
}

inline void KNMusicAlbumView::updateUIElements()
{
    //Get the usable width.
    int usableWidth=width()-m_spacing;
    //When the usable width is lesser than the minimal width,
    if(usableWidth<minimalWidth)
    {
        //Force set the max column count to 1.
        m_maxColumnCount=1;
        //Item width will be the whole width.
        m_itemWidth=width();
    }
    else
    {
        //Calculate how many column we can put in one row(m_maxColumnCount).
        m_maxColumnCount=usableWidth/minimalWidth;
        //Calculate the real item width, it's must be larger than minimal width.
        m_itemWidth=usableWidth/m_maxColumnCount-m_spacing;
    }
    //The icon size should be the item width.
    //Generate the album art shadow pixmap.
    int shadowSize=m_itemWidth+(m_shadowIncrease<<1);
    //Update album art shadow.
    m_albumArtShadow=generateShadow(shadowSize, shadowSize);
    //The height of the item should be a item icon size and two text lines.
    m_itemHeight=m_itemWidth+(fontMetrics().height()<<1);
    //Calcualte the spacing item width and height.
    m_itemSpacingHeight=m_spacing+m_itemHeight;
    m_itemSpacingWidth=m_spacing+m_itemWidth;
}

inline QPixmap KNMusicAlbumView::generateShadow(int shadowWidth,
                                                int shadowHeight)
{
    //Initial the shadow pixmap.
    QPixmap shadowPixmap(shadowWidth, shadowHeight);
    shadowPixmap.fill(QColor(255, 255, 255, 0));
    //Prepare the parameters.
    int sourceSize=m_shadowSource.width(),
        blockSize=sourceSize/3,
        blockSize2x=blockSize<<1,
        destinationWidth=shadowWidth-blockSize2x,
        destinationHeight=shadowHeight-blockSize2x;
    //Initial the antialiasing painter.
    QPainter painter(&shadowPixmap);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    painter.setOpacity(0.7);
    //Draw Top-left shadow.
    painter.drawPixmap(QRect(0,0,blockSize,blockSize),
                       m_shadowSource,
                       QRect(0,0,blockSize,blockSize));
    //Draw Top-Middle shadow.
    painter.drawPixmap(QRect(blockSize,0,destinationWidth-blockSize,blockSize),
                       m_shadowSource,
                       QRect(blockSize,0,blockSize,blockSize));
    //Draw Top-Right shadow.
    painter.drawPixmap(QRect(destinationWidth,0,blockSize2x, blockSize),
                       m_shadowSource,
                       QRect(blockSize,0,blockSize2x, blockSize));
    //Draw Middle-Left shadow.
    painter.drawPixmap(QRect(0,blockSize,blockSize,destinationHeight-blockSize),
                       m_shadowSource,
                       QRect(0,blockSize,blockSize,blockSize));
    //Draw Middle-Right shadow.
    painter.drawPixmap(QRect(destinationWidth+blockSize,
                             blockSize,
                             blockSize,
                             destinationHeight),
                       m_shadowSource,
                       QRect(blockSize2x,blockSize,blockSize,blockSize));
    //Draw Left-Bottom shadow.
    painter.drawPixmap(QRect(0,destinationHeight,blockSize, blockSize2x),
                       m_shadowSource,
                       QRect(0,blockSize,blockSize,blockSize2x));
    //Draw Middle-Bottom shadow.
    painter.drawPixmap(QRect(blockSize,
                             destinationHeight+blockSize,
                             destinationWidth,
                             blockSize),
                       m_shadowSource,
                       QRect(blockSize,blockSize2x,blockSize,blockSize));
    //Draw Right-Buttom shadow.
    painter.drawPixmap(QRect(destinationWidth+blockSize,
                             destinationHeight+blockSize,
                             blockSize,
                             blockSize),
                       m_shadowSource,
                       QRect(blockSize2x,blockSize2x,blockSize,blockSize));
    painter.end();
    return shadowPixmap;
}

