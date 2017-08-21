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
#include <QAction>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QTimeLine>

#include "knthememanager.h"
#include "kndpimanager.h"
#include "knlocalemanager.h"
#include "sao/knsaostyle.h"

#include "knmusiccategoryproxymodel.h"
#include "knmusicsearchbase.h"
#include "knmusicalbummodel.h"
#include "knmusicalbumdetail.h"
#include "knmusicglobal.h"

#include "knmusicalbumview.h"

#include <QDebug>

#define MaxOpacity          0x80
#define ShadowIncrease      15
#define ScrollBarWidth      10
#define ScrollBarSpacing    1

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent),
    m_shadowSource(QPixmap("://public/shadow.png")),
    m_albumArtShadow(QPixmap()),
    m_albumBase(QPixmap(":/plugin/music/public/base.png")),
    m_scaledAlbumBase(QPixmap()),
    m_noAlbumArt(knMusicGlobal->noAlbumArt()),
    m_scaledNoAlbumArt(QPixmap()),
    m_nullIndex(QModelIndex()),
    m_selectedIndex(m_nullIndex),
    m_mouseDownIndex(m_nullIndex),
    m_scrollAnime(new QTimeLine(200, this)),
    m_mouseAnime(new QTimeLine(200, this)),
    m_proxyModel(nullptr),
    m_model(nullptr),
    m_albumDetail(nullptr),
    m_scrollBar(new QScrollBar(this)),
    m_itemWidth(knDpi->width(135)),
    m_itemMinimalSpacing(knDpi->width(30)),
    m_minimalWidth(m_itemMinimalSpacing+m_itemWidth),
    m_lineCount(0),
    m_textSpacing(knDpi->height(5)),
    m_itemHeight(knDpi->height(154)),
    m_spacing(knDpi->height(30)),
    m_itemSpacingHeight(m_spacing+m_itemHeight),
    m_itemSpacingWidth(m_spacing+m_itemWidth),
    m_maxColumnCount(0)
{
    setObjectName("MusicAlbumView");
    //Set properties.
    setFocusPolicy(Qt::WheelFocus);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Set default scrollbar properties.
    verticalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setSingleStep(m_itemSpacingHeight);
    verticalScrollBar()->setPageStep((m_itemSpacingHeight>>3)*3);

    //Set the palette.
    knTheme->registerWidget(this);

    //Configure the timeline.
    m_scrollAnime->setUpdateInterval(16);
    m_scrollAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_scrollAnime, &QTimeLine::frameChanged,
            verticalScrollBar(), &QScrollBar::setValue);
    //Configure the timeline.
    m_mouseAnime->setUpdateInterval(16);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMusicAlbumView::onActionMouseInOut);
    //Configure the scroll bar.
    m_scrollBar->setObjectName("MusicScrollBar");
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->setSingleStep(verticalScrollBar()->singleStep());
    m_scrollBar->setPageStep(verticalScrollBar()->pageStep());
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
                //Locate to the visible current index.
                locateTo(currentIndex());
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

    //Link the scrolling event.
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &KNMusicAlbumView::onActionScrolling);

    //The icon size should be the item width.
    //Update the painting parameters.
    updateUIElements();
    //Generate the album art shadow pixmap.
    int shadowSize=m_itemWidth+(ShadowIncrease<<1);
    //Update album art shadow.
    m_albumArtShadow=generateShadow(shadowSize, shadowSize);
    //Update the album base.
    m_scaledAlbumBase=m_albumBase.scaled(m_itemWidth,
                                         m_itemWidth,
                                         Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation);
    //Update the no album art.
    m_scaledNoAlbumArt=m_noAlbumArt.scaled(m_itemWidth,
                                           m_itemWidth,
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation);
    //The height of the item should be a item icon size and two text lines.
    m_itemHeight=m_itemWidth+(fontMetrics().height()<<1);

    //Link the search.
    connect(knMusicGlobal->search(), &KNMusicSearchBase::requireSearch,
            this, &KNMusicAlbumView::onActionSearch);
    //Set the search shortcut.
    QAction *searchAction=new QAction(this);
    searchAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F));
    searchAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(searchAction, &QAction::triggered,
            [=]
            {
                //Check whether the search plugin is loaded.
                if(knMusicGlobal->search())
                {
                    knMusicGlobal->search()->onActionSearchShortcut(this);
                }
            });
    addAction(searchAction);

    //Link the locale manager.
    knI18n->link(this, &KNMusicAlbumView::retranslate);
    retranslate();
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    //Check model first.
    if(!m_model)
    {
        //For a null model, return null data.
        return m_nullIndex;
    }
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
    //Set the vertical scroll bar value directly.
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

void KNMusicAlbumView::selectAlbum(const QModelIndex &albumIndex)
{
    //If the index is vaild, set the initial animation parameters.
    if(albumIndex.isValid() && m_albumDetail)
    {
        //Check whether the album index is the same as the current one.
        if(albumIndex==m_selectedIndex)
        {
            //We don't need to expand the same album.
            return;
        }
        //Set current index to the proxy index of the album index.
        setCurrentIndex(m_proxyModel->mapFromSource(albumIndex));
        //Set the selected index.
        m_selectedIndex=albumIndex;
        //Show the detail.
        m_albumDetail->setAnimeParameter(
                    visualRect(m_proxyModel->mapFromSource(m_selectedIndex)),
                    m_itemWidth);
        m_albumDetail->displayAlbumDetail(m_selectedIndex);
        //Update the album view.
        viewport()->update();
        //Finished.
        return;
    }
    //Show the detail.
    m_albumDetail->setAnimeParameter(
                visualRect(m_proxyModel->mapFromSource(m_selectedIndex)),
                m_itemWidth);
    //Do fold detail animation.
    m_albumDetail->foldAlbumDetail();
    //Update the viewport.
    update();
    viewport()->update();
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
                           QPainter::TextAntialiasing,
                           true);
    //Check if we need to paint the background.
    if(autoFillBackground())
    {
        painter.fillRect(rect(), viewOptions().palette.base());
    }

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
        QModelIndex &&proxyIndex=m_proxyModel->index(currentRow, 0);
        //If the source index is not the current index, then draw the album.
        if(m_proxyModel->mapToSource(proxyIndex)!=m_selectedIndex)
        {
            //Draw the unselect album.
            paintAlbum(painter, currentLeft, currentTop, proxyIndex);
        }
        //Add current row to the next album model row.
        ++currentRow;
        //Add current column to the next position.
        ++currentColumn;
        //Check if we need to move to next row.
        if(currentColumn>=m_maxColumnCount)
        {
            //Add current line.
            ++currentLine;
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
    //Update the scroll bar position.
    m_scrollBar->setGeometry(
                width()-knDpi->width(ScrollBarWidth+ScrollBarSpacing),
                0,
                knDpi->width(ScrollBarWidth),
                height());
    //Update the parameters of the view first.
    updateUIElements();
    //Resize the album detail.
    m_albumDetail->resize(size());
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
    //! TODO: Add operation here.
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
    //Stop the animation as soon as possible.
    m_scrollAnime->stop();
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

void KNMusicAlbumView::enterEvent(QEvent *event)
{
    //Enter the list view.
    QAbstractItemView::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KNMusicAlbumView::leaveEvent(QEvent *event)
{
    //Leave the list view.
    QAbstractItemView::leaveEvent(event);
    //Start mouse leave anime.
    startAnime(0);
}

void KNMusicAlbumView::keyReleaseEvent(QKeyEvent *event)
{
    //Check the key press code.
    switch(event->key())
    {
    case Qt::Key_PageDown:
        //Use timeline to move to the position.
        m_scrollAnime->stop();
        m_scrollAnime->setFrameRange(verticalScrollBar()->value(),
                                     verticalScrollBar()->value() +
                                     (height()>>1));
        m_scrollAnime->start();
        break;
    case Qt::Key_PageUp:
        //Use timeline to move to the position.
        m_scrollAnime->stop();
        m_scrollAnime->setFrameRange(verticalScrollBar()->value(),
                                     verticalScrollBar()->value() -
                                     (height()>>1));
        m_scrollAnime->start();
        break;
    case Qt::Key_Down:
        //Stop the scroll anime.
        m_scrollAnime->stop();
        //Manually update the position.
        verticalScrollBar()->setValue(verticalScrollBar()->value() +
                                      (m_itemSpacingHeight>>2));
        break;
    case Qt::Key_Up:
        //Use timeline to move to the position.
        m_scrollAnime->stop();
        //Manually update the position.
        verticalScrollBar()->setValue(verticalScrollBar()->value() -
                                      (m_itemSpacingHeight>>2));
        break;
    case Qt::Key_Home:
        //Use timeline to move to the position.
        m_scrollAnime->stop();
        m_scrollAnime->setFrameRange(verticalScrollBar()->value(),
                                     0);
        m_scrollAnime->start();
        break;
    case Qt::Key_End:
        //Use timeline to move to the position.
        m_scrollAnime->stop();
        m_scrollAnime->setFrameRange(verticalScrollBar()->value(),
                                     verticalScrollBar()->maximum());
        m_scrollAnime->start();
        break;
    default:
        //Do original key press event.
        QAbstractItemView::keyReleaseEvent(event);
    }
}

void KNMusicAlbumView::updateGeometries()
{
    //Update the range of the vertical scroll bar.
    verticalScrollBar()->setRange(0,
                                  qMax(0,
                                       m_lineCount*m_itemSpacingHeight +
                                       m_spacing-height()));
}

void KNMusicAlbumView::retranslate()
{
    //Check out the album detail widget.
    if(m_albumDetail && m_selectedIndex.isValid())
    {
        //Update the album detail.
        m_albumDetail->updateAlbumCaptions();
    }
}

void KNMusicAlbumView::onActionScrolling()
{
    //Check out the album detail widget.
    if(m_albumDetail && m_selectedIndex.isValid())
    {
        //Update the fold end value of the album detail.
        m_albumDetail->updateFoldEndValue(
                    visualRect(m_proxyModel->mapFromSource(m_selectedIndex)),
                    m_itemWidth);
    }
}

void KNMusicAlbumView::displayAlbum(const QPoint &point)
{
    //Check out the display album.
    if(m_albumDetail==nullptr)
    {
        //Ignore the display album operation.
        return;
    }
    //Get the index from the point.
    QModelIndex proxyIndex=indexAt(point),
                sourceIndex=m_proxyModel->mapToSource(proxyIndex);
    //Check out the proxy index of the proxy index.
    if(proxyIndex.isValid() && m_selectedIndex!=sourceIndex)
    {
        //Scroll to the album.
        scrollTo(proxyIndex);
        //Select the album.
        selectAlbum(sourceIndex);
        //Finished.
        return;
    }
    //Show the detail.
    m_albumDetail->setAnimeParameter(
                visualRect(m_proxyModel->mapFromSource(m_selectedIndex)),
                m_itemWidth);
    //Fold the album.
    m_albumDetail->foldAlbumDetail();
    //Update the viewport.
    viewport()->update();
}

void KNMusicAlbumView::onActionSearch()
{
    //Check whether the proxy model is nullptr.
    if(m_proxyModel==nullptr || m_albumDetail==nullptr)
    {
        //Ignore the search request.
        return;
    }

    //Get the search.
    KNMusicSearchBase *search=knMusicGlobal->search();
    //Check the rules are the same or not.
    if(search->rules()==m_proxyModel->searchBlocks())
    {
        //The proxy model is the same as the search block.
        return;
    }
    //Set the search text to proxy model simply.
    m_proxyModel->setSearchBlocks(knMusicGlobal->search()->rules());
    //Hide the detail and clear the selection first.
    m_albumDetail->flyAwayAlbumDetail();
    //Update the viewport.
    viewport()->update();
}

void KNMusicAlbumView::onActionMouseInOut(int frame)
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
                                         int x,
                                         int y,
                                         const QModelIndex &index)
{
    //Ensure the index is vaild.
    if(!index.isValid())
    {
        return;
    }
    //Draw the shadow first.
    painter.drawPixmap(x-ShadowIncrease, y-ShadowIncrease,
                       m_albumArtShadow);

    //Render and draw the album art image.
    QPixmap &&albumArtImage=
            m_proxyModel->data(index,
                               Qt::DecorationRole).value<QPixmap>();
    //Check out the album art is valid.
    if(albumArtImage.isNull())
    {
        //Draw the no album art instead.
        painter.drawPixmap(x, y, m_scaledNoAlbumArt);
    }
    else
    {
        //Draw the album art to the specific position.
        painter.drawPixmap(QPoint(x+((m_itemWidth-albumArtImage.width())>>1),
                                  y+((m_itemWidth-albumArtImage.height())>>1)),
                           albumArtImage);
    }
    //Set the pen as the text color.
    QColor textColor=viewOptions().palette.color(QPalette::Text);
    painter.setPen(textColor);
    //Draw the album caption, which contains album name and artist.
    //Draw the album name first.
    int textY=y+m_itemWidth+m_textSpacing;
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
    QString &&artistText=
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
                           textY+fontMetrics().height()+(m_textSpacing>>1),
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
    int usableWidth=viewport()->width();
    //When the usable width is lesser than the minimal width,
    if(usableWidth<m_minimalWidth)
    {
        //Force set the max column count to 1.
        m_maxColumnCount=1;
        //Reset the spacing.
        m_spacing=knDpi->width(30);
    }
    else
    {
        //Calculate how many column we can put in one row(m_maxColumnCount).
        m_maxColumnCount=(usableWidth-m_itemMinimalSpacing)/m_minimalWidth;
        //Calculate the new spacing.
        m_spacing=(usableWidth-m_itemWidth*m_maxColumnCount)/
                    (m_maxColumnCount+1);
    }

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
    //Draw the black central content.
    painter.fillRect(QRect(ShadowIncrease,
                           ShadowIncrease,
                           m_itemWidth,
                           m_itemWidth),
                     QColor(0, 0, 0));
    painter.end();
    return shadowPixmap;
}

inline void KNMusicAlbumView::startAnime(int endFrame)
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

KNMusicAlbumDetail *KNMusicAlbumView::albumDetail() const
{
    return m_albumDetail;
}

void KNMusicAlbumView::setAlbumDetail(KNMusicAlbumDetail *albumDetail)
{
    //Check we have set it before or not.
    if(m_albumDetail!=nullptr)
    {
        //Ignore the secondary set.
        return;
    }
    //Save the album detail widget.
    m_albumDetail = albumDetail;
    //Check the validation of the album detail.
    if(m_albumDetail==nullptr)
    {
        //Ignore the null album detail.
        return;
    }
    //Change the focus proxy to the album detail.
    m_albumDetail->setFocusProxy(this);
    //Link the album detail to the album view.
    connect(m_albumDetail, &KNMusicAlbumDetail::requireShowAlbum,
            this, &KNMusicAlbumView::displayAlbum);
    connect(m_albumDetail, &KNMusicAlbumDetail::requireClearSelection,
            this, &KNMusicAlbumView::clearSelection);
    //Hide the detail widget.
    m_albumDetail->hide();
    //Raise it up.
    m_albumDetail->raise();
}
