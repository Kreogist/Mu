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
#include <QPainter>
#include <QMouseEvent>
#include <QPersistentModelIndex>
#include <QTimeLine>

#include "sao/knsaostyle.h"
#include "knthememanager.h"
#include "knsideshadowwidget.h"

#include "knmusicstoreutil.h"
#include "knmusicstorealbumlistmodel.h"

#include "knmusicstorealbumlist.h"

#include <QDebug>

#define StoreAlbumArtTextSpacing 5
#define StoreAlbumSpacing 20
#define StoreAlbumTopSpacing 19
#define StoreAlbumScrollBarHeight 12

KNMusicStoreAlbumList::KNMusicStoreAlbumList(QWidget *parent) :
    QAbstractItemView(parent),
    m_scrollBar(new QScrollBar(Qt::Horizontal, this)),
    m_model(nullptr),
    m_leftShadow(new KNSideShadowWidget(KNSideShadowWidget::LeftShadow, this)),
    m_rightShadow(new KNSideShadowWidget(KNSideShadowWidget::RightShadow,
                                         this)),
    m_mouseAnime(new QTimeLine(200, this)),
    m_itemHeight(0),
    m_scrollBarBrightness(0)
{
    setObjectName("MusicStoreAlbumList");
    //Set properties.
    setFrameStyle(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Add to theme manager.
    knTheme->registerWidget(this);
    //Configure the horizontal scroll bar.
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->hide();
    //Link the scroll bar.
    connect(horizontalScrollBar(), &QScrollBar::valueChanged,
            this, &KNMusicStoreAlbumList::onActionUpdateOpacity);
    connect(m_scrollBar, &QScrollBar::valueChanged,
            horizontalScrollBar(), &QScrollBar::setValue);
    //Update item height.
    m_itemHeight=StoreAlbumSize + (StoreAlbumArtTextSpacing << 1) +
                 fontMetrics().height() * 3;
    //Set the fixed size of the album list.
    setFixedHeight((StoreAlbumTopSpacing << 1) + m_itemHeight);
    //Update the page and single step.
    int albumFullWidth=(StoreAlbumSize + StoreAlbumSpacing);
    horizontalScrollBar()->setPageStep(albumFullWidth);
    horizontalScrollBar()->setSingleStep(albumFullWidth);
    //Update the visible scroll bar page and single step.
    m_scrollBar->setPageStep(albumFullWidth);
    m_scrollBar->setSingleStep(albumFullWidth);
    //Configure the scroll bar.
    m_scrollBar->move(StoreAlbumShadow,
                      height() - StoreAlbumScrollBarHeight);
    m_scrollBar->setFixedHeight((StoreAlbumTopSpacing>>2)*3);
    //Link time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMusicStoreAlbumList::onActionUpdateBrightness);

    //Configure left and right shadow.
    m_leftShadow->setFixedSize(StoreAlbumShadow, height());
    m_rightShadow->setFixedSize(StoreAlbumShadow, height());
    //Change the opacity 0.
    onActionUpdateOpacity(0);
    onActionUpdateBrightness(0);
}

QModelIndex KNMusicStoreAlbumList::indexAt(const QPoint &point) const
{
    //Check model.
    if(!m_model)
    {
        //Give a null index.
        return QModelIndex();
    }
        //Calculate the X of the content.
    int pointContentX=horizontalScrollBar()->value() + point.x(),
        //Calculate which album this point is locate on.
        albumIndex=pointContentX/(StoreAlbumSize + StoreAlbumSpacing),
        //Calculate the clicked position.
        clickedPosition=
            pointContentX - albumIndex * (StoreAlbumSize + StoreAlbumSpacing);
    //Check whether the point is click on space or album itself.
    return (clickedPosition > StoreAlbumSpacing) &&
            (clickedPosition < StoreAlbumSpacing + StoreAlbumSize) &&
            (albumIndex<m_model->rowCount()) &&
            (point.y() > StoreAlbumTopSpacing) &&
            (point.y() < StoreAlbumTopSpacing + m_itemHeight) ?
                m_model->index(albumIndex, 0) : QModelIndex();
}

void KNMusicStoreAlbumList::scrollTo(const QModelIndex &index,
                                     QAbstractItemView::ScrollHint hint)
{
    //Check the index and the validation of max column count.
    if(!index.isValid() ||
            //Check whether we need to move the horizontal scroll bar
            (hint==QAbstractItemView::EnsureVisible &&
                  rect().contains(visualRect(index), true)))
    {
        //Don't need to move the horizontal scroll bar.
        return;
    }
    //Set the horizontal scroll bar value directly.
    horizontalScrollBar()->setValue(indexScrollBarValue(index, hint));
    //Update the painting.
    viewport()->update();
}

QRect KNMusicStoreAlbumList::visualRect(const QModelIndex &index) const
{
    //Get the item content rect.
    QRect indexRect=itemContentRect(index);
    //If the rect is vaild, remove the value of the scroll bar to get the
    //position in the rect.
    return indexRect.isValid()?
                QRect(indexRect.left()-horizontalScrollBar()->value(),
                      indexRect.top(),
                      indexRect.width(),
                      indexRect.height()):
                QRect();
}

void KNMusicStoreAlbumList::setModel(QAbstractItemModel *model)
{
    //Save the model.
    m_model=static_cast<KNMusicStoreAlbumListModel *>(model);
    //Check out the model.
    if(m_model==nullptr)
    {
        return;
    }
    //Set the model.
    QAbstractItemView::setModel(m_model);
    //Update the geometries.
    updateGeometries();
}

void KNMusicStoreAlbumList::resetPostion()
{
    //Reset the value.
    horizontalScrollBar()->setValue(0);
}

void KNMusicStoreAlbumList::enterEvent(QEvent *event)
{
    //Do original event.
    QAbstractItemView::enterEvent(event);
    //Update the horizontal scroll bar.
    startAnime(100);
}

void KNMusicStoreAlbumList::leaveEvent(QEvent *event)
{
    //Do original event
    QAbstractItemView::leaveEvent(event);
    //Update the horizontal scroll bar.
    startAnime(0);
}

void KNMusicStoreAlbumList::mouseMoveEvent(QMouseEvent *event)
{
    //Set the cursor to be a clickable pointer.
    setCursor(indexAt(event->pos()).isValid()?
                  Qt::PointingHandCursor :
                  Qt::ArrowCursor);
    //Check the move event.
    QAbstractItemView::mouseMoveEvent(event);
}

void KNMusicStoreAlbumList::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing,
                           true);
    //Check the model first.
    if(m_model==nullptr)
    {
        return;
    }
    //Calculate the first index.
    int currentModelRow=horizontalScrollBar()->value() /
                        (StoreAlbumSize + StoreAlbumSpacing),
        currentX=currentModelRow * (StoreAlbumSize + StoreAlbumSpacing) -
            horizontalScrollBar()->value() + StoreAlbumSpacing;
    //Get the text color.
    static QColor textColor=palette().color(QPalette::Text),
                  artistColor=textColor;
    //Reduce artist color alpha.
    artistColor.setAlpha(textColor.alpha()>>1);
    //Draw until the current x is greater than width, or reach the end of the
    //model.
    while(currentX < width() && currentModelRow < m_model->rowCount())
    {
        //Draw the current item.
        painter.drawPixmap(currentX, StoreAlbumTopSpacing,
                           m_model->albumArt(currentModelRow));
        //Get the elied album name.
        QString albumName=fontMetrics().elidedText(
                    m_model->albumTitle(currentModelRow),
                    Qt::ElideRight,
                    StoreAlbumSize<<1);
        //Get the bounding rect.
        QRect albumNameRect=fontMetrics().boundingRect(
                    QRect(currentX,
                          StoreAlbumSize + StoreAlbumArtTextSpacing +
                          StoreAlbumTopSpacing,
                          StoreAlbumSize,
                          fontMetrics().height()<<1),
                    Qt::TextWordWrap | Qt::TextWrapAnywhere,
                    albumName);
        //Draw the album name and artist name.
        painter.setPen(textColor);
        //Save the draw text.
        painter.drawText(QRect(currentX,
                               StoreAlbumSize + StoreAlbumArtTextSpacing +
                               StoreAlbumTopSpacing,
                               StoreAlbumSize,
                               albumNameRect.height()),
                         Qt::TextWordWrap | Qt::TextWrapAnywhere,
                         albumName);
        //Set the painter.
        painter.setPen(artistColor);
        //Draw the artist.
        painter.drawText(QRect(currentX,
                               albumNameRect.bottom() + 2,
                               StoreAlbumSize,
                               fontMetrics().height()),
                         fontMetrics().elidedText(
                             m_model->artist(currentModelRow),
                             Qt::ElideRight,
                             StoreAlbumSize));
        //Set the color.
        //Move to next album.
        currentX += (StoreAlbumSize + StoreAlbumSpacing);
        ++currentModelRow;
    }
    //Update the scroll bar value.
    updateGeometries();
}

void KNMusicStoreAlbumList::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QAbstractItemView::resizeEvent(event);
    //Move the right.
    m_rightShadow->move(width()-StoreAlbumShadow,
                        0);
    //Resize the scroll bar.
    m_scrollBar->resize(width()-(StoreAlbumShadow<<1),
                        m_scrollBar->height());
    //Update the page and single step.
    horizontalScrollBar()->setPageStep(width());
    //Update the visible scroll bar page and single step.
    m_scrollBar->setPageStep(width());
    m_scrollBar->setSingleStep(width());
}

QModelIndex KNMusicStoreAlbumList::moveCursor(
        QAbstractItemView::CursorAction cursorAction,
        Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(cursorAction)
    Q_UNUSED(modifiers)
    return currentIndex();
}

int KNMusicStoreAlbumList::horizontalOffset() const
{
    //Horizontal offset scroll bar provides the offset.
    return horizontalScrollBar()->value();
}

int KNMusicStoreAlbumList::verticalOffset() const
{
    return 0;
}

bool KNMusicStoreAlbumList::isIndexHidden(const QModelIndex &index) const
{
    Q_UNUSED(index)
    //No index will be hidden.
    return false;
}

void KNMusicStoreAlbumList::setSelection(
        const QRect &rect,
        QItemSelectionModel::SelectionFlags command)
{
    //Simply select the top left position.
    selectionModel()->select(indexAt(QPoint(rect.x(), rect.y())),
                             command);
}

QRegion KNMusicStoreAlbumList::visualRegionForSelection(
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

void KNMusicStoreAlbumList::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    //Disabled all the event of previous.
}

void KNMusicStoreAlbumList::updateGeometries()
{
    //Check model.
    if(!m_model)
    {
        //Reset the range.
        m_scrollBar->setRange(0, 0);
        horizontalScrollBar()->setRange(0, 0);
        return;
    }
    //Update the range of the vertical scroll bar.
    m_scrollBar->setRange(0,
                          qMax(0,
                               (StoreAlbumSize + StoreAlbumSpacing) *
                               m_model->rowCount() - width() +
                               StoreAlbumSpacing));
    horizontalScrollBar()->setRange(m_scrollBar->minimum(),
                                    m_scrollBar->maximum());
}

void KNMusicStoreAlbumList::onActionUpdateOpacity(int value)
{
    //Update the opacity according to value.
    // Left Darkness.
    m_leftShadow->setDarkness((value>70)?70:value);
    // Right Darkness.
    int rightDarkness=m_scrollBar->maximum()-value;
    m_rightShadow->setDarkness((rightDarkness>70)?70:rightDarkness);
    //Update scroll bar value.
    m_scrollBar->blockSignals(true);
    m_scrollBar->setValue(value);
    m_scrollBar->blockSignals(false);
}

void KNMusicStoreAlbumList::onActionUpdateBrightness(int opacity)
{
    //Configure the horizontal scroll bar.
    QPalette pal=m_scrollBar->palette();
    QColor buttonColor=pal.color(QPalette::Button);
    //Change the alpha.
    buttonColor.setAlpha(opacity);
    //Set back the button color
    pal.setColor(QPalette::Button, buttonColor);
    //Change the alpha for base color.
    buttonColor.setAlpha(opacity>>1);
    //Update the palette.
    pal.setColor(QPalette::Base, buttonColor);
    //Reset the color.
    m_scrollBar->setPalette(pal);
    //Save the opacity.
    m_scrollBarBrightness=opacity;
}

inline int KNMusicStoreAlbumList::indexScrollBarValue(
        const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    //Get the top of index position, set it to scroll value.
    int leftPositionValue=index.row()*(StoreAlbumSize + StoreAlbumSpacing);
    //Change the item content Y according to the hint.
    switch(hint)
    {
    case QAbstractItemView::PositionAtTop:
        //No need to change.
        return leftPositionValue;
    case QAbstractItemView::PositionAtCenter:
        //Reduce a half of the viewer width to move right.
        return leftPositionValue-((width()-StoreAlbumSpacing)>>1);
    case QAbstractItemView::PositionAtBottom:
        //Reduce the whole viewer width to move right.
        return leftPositionValue-width()+StoreAlbumSpacing;
        break;
    default:
        //Now, the index item must be a unvisible one in the viewer.
        //We have already has the top position, calculate the bottom position,
        //and calculate the distence of the current vertical scroll bar's value
        //to these two position.
        int rightPositionValue=leftPositionValue-width()+StoreAlbumSpacing;
        //If to the bottom is lesser than to top, change the value.
        return (qAbs(horizontalScrollBar()->value()-rightPositionValue)<
                qAbs(horizontalScrollBar()->value()-leftPositionValue))?
                    rightPositionValue:
                    leftPositionValue;
    }
}

inline QRect KNMusicStoreAlbumList::itemContentRect(
        const QModelIndex &index) const
{
    //Check the index first.
    if(!index.isValid())
    {
        return QRect();
    }
    //Calculate the rect.
    return QRect((StoreAlbumSize + StoreAlbumSpacing) * index.row(),
                 0,
                 StoreAlbumSize,
                 m_itemHeight);
}

inline void KNMusicStoreAlbumList::startAnime(int endFrame)
{
    //Stop the mouse anime.
    m_mouseAnime->stop();
    //Set the frame of the time line.
    m_mouseAnime->setFrameRange(m_scrollBarBrightness, endFrame);
    //Start the animation.
    m_mouseAnime->start();
}

void KNMusicStoreAlbumList::mouseReleaseEvent(QMouseEvent *event)
{
    //Get the current pressed position.
    QPoint position=event->pos();
    //Check the selection model and state.
    if(!selectionModel() || m_model==nullptr)
    {
        //If there's no selection model, ignore the press event.
        return;
    }
    //Get the clicked index.
    QPersistentModelIndex clickedIndex=indexAt(position);
    //Check the validation of the index.
    if(clickedIndex.isValid())
    {
        //Emit the clicked index.
        emit albumClick(m_model->albumData(clickedIndex));
    }
}
