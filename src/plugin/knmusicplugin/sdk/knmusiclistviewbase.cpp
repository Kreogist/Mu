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
#include <QDrag>
#include <QScrollBar>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimeLine>

#include "knconnectionhandler.h"
#include "sao/knsaostyle.h"

#include "knmusicglobal.h"
#include "knmusicmodel.h"
#include "knmusicproxymodel.h"
#include "knmusicsearchbase.h"
#include "knmusicnowplayingbase.h"
#include "knmusicdetailtooltipbase.h"
#include "knmusicsolomenubase.h"
#include "knmusicmultimenubase.h"

#include "knmusiclistviewbase.h"

#define MaxOpacity 0x40
#define ScrollBarWidth 10
#define ScrollBarSpacing 1

KNMusicListViewBase::KNMusicListViewBase(QWidget *parent, KNMusicTab *tab) :
    QListView(parent),
    m_musicTab(tab),
    m_proxyModel(nullptr),
    m_mouseAnime(new QTimeLine(200, this)),
    m_scrollBar(new QScrollBar(this)),
    m_pressed(false),
    m_autoBuildProxy(true)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setLayoutMode(QListView::Batched);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Set scroll bar policies.
    verticalScrollBar()->setSingleStep(4);
    verticalScrollBar()->setPageStep(4);

    //Configure the scroll bar.
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->hide();
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
            this, &KNMusicListViewBase::onActionMouseInOut);

    //Link the tree view signal and slot.
    connect(this, &KNMusicListViewBase::activated,
            this, &KNMusicListViewBase::onActionActivate);
}

KNMusicProxyModel *KNMusicListViewBase::proxyModel()
{
    //Check is the proxy model need to initial.
    if(m_proxyModel==nullptr && m_autoBuildProxy)
    {
        //Initial the proxy model.
        m_proxyModel=new KNMusicProxyModel(this);
        //Apply the proxy model.
        applyProxyModel();
    }
    //Give back the proxy model.
    return m_proxyModel;
}

KNMusicModel *KNMusicListViewBase::musicModel()
{
    //If the proxy model is nullptr, then it wil be nullptr.
    //Or else, get the music model of the proxy model.
    return m_proxyModel==nullptr?nullptr:m_proxyModel->musicModel();
}

void KNMusicListViewBase::setMusicModel(KNMusicModel *musicModel)
{
    //Hide the detail tooltip first.
    if(knMusicGlobal->detailTooltip())
    {
        knMusicGlobal->detailTooltip()->hide();
    }
    //Check before set the music model, if the current playing model is the
    //previous model, shadow the proxy model.
    if(knMusicGlobal->nowPlaying()!=nullptr)
    {
        //Get the now playing object.
        KNMusicNowPlayingBase *nowPlaying=knMusicGlobal->nowPlaying();
        //Check the current playing model is the current proxy and the proxy
        //model is copied from the treeview.
        if(nowPlaying->playingMusicModel()==proxyModel()->musicModel() &&
                nowPlaying->playingModel()==proxyModel())
        {
            //If so, shadow the proxy model.
            nowPlaying->shadowPlayingModel();
        }
    }
    //Set the source model.
    proxyModel()->setSourceModel(musicModel);
}

void KNMusicListViewBase::selectSourceSong(const int &sourceRow)
{
    //Ignore the request when there's no music model.
    if(musicModel()==nullptr)
    {
        return;
    }
    //Select the row.
    setCurrentIndex(
                proxyModel()->mapFromSource(
                    musicModel()->index(sourceRow, Name)));
}

void KNMusicListViewBase::scrollToSourceRow(const int &sourceRow)
{
    //Check the music model first.
    if(musicModel()!=nullptr)
    {
        //Scroll to the row.
        scrollToIndex(m_proxyModel->mapFromSource(musicModel()->index(sourceRow,
                                                                      Name)));
    }
}

void KNMusicListViewBase::scrollToRow(const int &row)
{
    //Get the name item of the song row.
    scrollToIndex(m_proxyModel->index(row, Name));
}

void KNMusicListViewBase::sortByColumn(int column, Qt::SortOrder order)
{
    //Set the sort action to the proxy model.
    proxyModel()->sort(column, order);
}

void KNMusicListViewBase::setProxyModel(KNMusicProxyModel *proxyModel)
{
    //Check the current proxy model.
    if(m_proxyModel)
    {
        //Delete the proxy model later.
        m_proxyModel->deleteLater();
    }
    //Save the new proxy model.
    m_proxyModel=proxyModel;
    //Apply the proxy model to the view.
    applyProxyModel();
}

void KNMusicListViewBase::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions)
    //Emit the start dragging signal.
    emit startDraggingSong();
    //Get all the selections.
    QModelIndexList indexes=selectionModel()->selectedRows(Name);
    //Check the size of selected rows.
    if(indexes.isEmpty())
    {
        //If there's no rows selected, ignore the drag operation.
        return;
    }
    //Generate the mime data.
    QScopedPointer<QDrag> drag(new QDrag(this));
    //Set the mime data to the drag action.
    drag->setMimeData(m_proxyModel->mimeData(indexes));
    //Do the drag.
    drag->exec();
    //Emit the end dragging signal.
    emit endDraggingSong();
}

void KNMusicListViewBase::enterEvent(QEvent *event)
{
    //Do the original enter event.
    QListView::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KNMusicListViewBase::leaveEvent(QEvent *event)
{
    //Do the original enter event.
    QListView::leaveEvent(event);
    //Start mouse in anime.
    startAnime(0);
}

void KNMusicListViewBase::resizeEvent(QResizeEvent *event)
{
    //Resize the list view.
    QListView::resizeEvent(event);
    //Update the scroll bar position.
    m_scrollBar->setGeometry(width()-ScrollBarWidth-ScrollBarSpacing,
                             0,
                             ScrollBarWidth,
                             height());
}

void KNMusicListViewBase::mousePressEvent(QMouseEvent *event)
{
    //Do the original pressed event.
    QListView::mousePressEvent(event);
    //Hide the detail tooltip first.
    if(knMusicGlobal->detailTooltip())
    {
        knMusicGlobal->detailTooltip()->hide();
    }
    //Set pressed flag.
    m_pressed=true;
}

void KNMusicListViewBase::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the original release event.
    QListView::mouseReleaseEvent(event);
    //Check we has been pressed on this widget before.
    if(m_pressed)
    {
        //Reset the pressed flag.
        m_pressed=false;
        //Check is button right button and position is in the treeview.
        if(event->button()==Qt::RightButton && rect().contains(event->pos()))
        {
            //Show the menu at the position.
            showMenu(event->pos(), indexAt(event->pos()));
        }
    }
}

void KNMusicListViewBase::keyReleaseEvent(QKeyEvent *event)
{
    //Judge according to the released key.
    switch(event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
        //Check the current index first.
        if(currentIndex().isValid())
        {
            //Hack this as the activate action.
            onActionActivate(currentIndex());
        }
        break;
    }
    case Qt::Key_Menu:
    {
        //Check the current index first.
        if(currentIndex().isValid())
        {
            //Get the current rect of the item.
            QRect currentRect=visualRect(currentIndex());
            //Get the target Y position.
            int targetY=currentRect.y()+(currentRect.height()>>1);
            //Check the target Y.
            //Over the bottom.
            if(targetY>=height())
            {
                //Force to set the target Y to be the bottom of the widget.
                targetY=height();
            }
            //Over the top.
            if(targetY<0)
            {
                //Force to set the target Y to be 0.
                targetY=0;
            }
            //Show the menu.
            showMenu(QPoint(currentRect.height()>>1, targetY), currentIndex());
        }
        break;
    }
    default:
        //Do the orignal key release event.
        QListView::keyReleaseEvent(event);
    }
}

bool KNMusicListViewBase::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::ToolTip:
    case QEvent::ToolTipChange:
    {
        //Cast the event as a help event.
        QHelpEvent *helpEvent=static_cast<QHelpEvent *>(event);
        //Show the detail tooltip at the index position
        return showDetailTooltip(helpEvent->pos());
    }
    default:
        //Process other kinds of events.
        return QListView::event(event);
    }
}

void KNMusicListViewBase::wheelEvent(QWheelEvent *event)
{
    //Do original wheel event.
    QListView::wheelEvent(event);
    //When user using wheel, means the user don't want to preview the current
    //thing.
    //Hide the preview tooltip.
    if(knMusicGlobal->detailTooltip() &&
            knMusicGlobal->detailTooltip()->isVisible())
    {
        knMusicGlobal->detailTooltip()->hide();
    }
}

void KNMusicListViewBase::onActionActivate(const QModelIndex &index)
{
    //Hide the detail tooltip.
    if(knMusicGlobal->detailTooltip())
    {
        knMusicGlobal->detailTooltip()->hide();
    }
    //Play the activate index.
    playIndex(index);
}

void KNMusicListViewBase::onActionMouseInOut(int frame)
{
    //Update the scroll bar palette.
    QPalette pal=m_scrollBar->palette();
    QColor color=pal.color(QPalette::Button);
    color.setAlpha(frame);
    pal.setColor(QPalette::Button, color);
    color=pal.color(QPalette::Base);
    color.setAlpha(frame>>1);
    pal.setColor(QPalette::Base, color);
    //Set the palette to scroll bar.
    m_scrollBar->setPalette(pal);
}

void KNMusicListViewBase::playCurrent()
{
    //Play the current index.
    playIndex(currentIndex());
}

void KNMusicListViewBase::removeCurrent()
{
    //Check whether the proxy model has a content music model.
    //Check the current index is valid.
    if(musicModel()!=nullptr &&
            currentIndex().isValid())
    {
        //Remove the current index.
        proxyModel()->removeRow(currentIndex().row());
    }
}

inline void KNMusicListViewBase::applyProxyModel()
{
    //Set the search text.
    m_proxyModel->setSearchBlocks(knMusicGlobal->search()->rules());
    //Set the proxy model.
    setModel(m_proxyModel);
}

inline void KNMusicListViewBase::scrollToIndex(const QModelIndex &proxyIndex)
{
    //Set the current index to the specific row.
    setCurrentIndex(proxyIndex);
    //Use the default scrollTo function to locate that index.
    scrollTo(proxyIndex, QAbstractItemView::PositionAtCenter);
}

void KNMusicListViewBase::playIndex(const QModelIndex &index)
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
    nowPlaying->playMusicRow(proxyModel(), index.row(), m_musicTab);
}

inline void KNMusicListViewBase::startAnime(int endFrame)
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

void KNMusicListViewBase::showMenu(QPoint position,
                                   const QModelIndex &pressedIndex)
{
    //Hide the detail tooltip first.
    if(knMusicGlobal->detailTooltip())
    {
        knMusicGlobal->detailTooltip()->hide();
    }
    //According to the selected rows, display different menu.
    switch(selectionModel()->selectedRows().size())
    {
    case 0:
        break;
    case 1:
        showSoloMenu(position, pressedIndex);
        break;
    default:
        showMultiMenu(position);
        break;
    }
}

void KNMusicListViewBase::showSoloMenu(const QPoint &position,
                                       const QModelIndex &pressedIndex)
{
    //Check the valid of the index.
    if(pressedIndex.isValid() && knMusicGlobal->soloMenu()!=nullptr)
    {
        //Get the solo menu.
        KNMusicSoloMenuBase *soloMenu=knMusicGlobal->soloMenu();
        //Generate the connection handler.
        KNConnectionHandler connections;
        //Link the menu require signal to this slot.
        connections.append(
                   connect(soloMenu, &KNMusicSoloMenuBase::requirePlayCurrent,
                           this, &KNMusicListViewBase::playCurrent));
        connections.append(
                   connect(soloMenu, &KNMusicSoloMenuBase::requireRemoveCurrent,
                           this, &KNMusicListViewBase::removeCurrent));
        //Set the information to the solo menu.
        soloMenu->setMusicRow(m_proxyModel, pressedIndex);
        //Get the menu position, fixed the bug which ignore the header's height.
        QPoint menuPosition=mapToGlobal(position);
        menuPosition.setY(menuPosition.y());
        //Set the position to menu.
        soloMenu->setMouseDownPos(menuPosition);
        //Launch the menu.
        soloMenu->exec(menuPosition);
        //Disconnect the links.
        connections.disconnectAll();
    }
}

void KNMusicListViewBase::showMultiMenu(const QPoint &position)
{
    //Get the selected indexes.
    QModelIndexList dumpIndexes=selectedIndexes();
    //Generate a temporary list.
    QList<int> rowList;
    //Check the indexes size.
    if(dumpIndexes.isEmpty())
    {
        return;
    }
    //Check all the indexes list, we will only save one row.
    for(auto i=dumpIndexes.constBegin(); i!=dumpIndexes.constEnd(); ++i)
    {
        //Check whether current row is in the row list.
        if(rowList.contains((*i).row()))
        {
            continue;
        }
        //Add the new row to row list.
        rowList.append((*i).row());
    }
    //Get the index of the position where mouse pressed.
    QModelIndex pressedIndex=indexAt(position);
    //Check the validation of the multi menu.
    if(knMusicGlobal->multiMenu())
    {
        //Get the multi menu.
        KNMusicMultiMenuBase *multiMenu=knMusicGlobal->multiMenu();
        //Generate the connection handler.
        KNConnectionHandler connections;
        //Link the menu require signal to this slot.
        ;
        //Set information to menu.
        multiMenu->setMusicRows(m_proxyModel, rowList, pressedIndex);
        //Get the menu position.
        QPoint menuPosition=mapToGlobal(position);
        //Set the position to menu.
        multiMenu->setMouseDownPos(menuPosition);
        //Launch the menu.
        multiMenu->exec(menuPosition);
        //Disconnect the links.
        connections.disconnectAll();
    }
}

bool KNMusicListViewBase::showDetailTooltip(const QPoint &indexPosition)
{
    //If the detail tooltip is not set, ignore the tooltip request.
    if(!knMusicGlobal->detailTooltip())
    {
        return false;
    }
    //If the position is in the header, hide the detail tooltip.
    if(indexPosition.y() < 0)
    {
        //Hide the detail tooltip widget.
        knMusicGlobal->detailTooltip()->hide();
        //Finished.
        return false;
    }
    //Locate index at the position.
    QModelIndex mouseIndex=indexAt(indexPosition);
    //Check the validation of the index.
    if(mouseIndex.isValid())
    {
        //If the position is on the vertical scrollbar, then hide the
        //tooltip widget.
        if(verticalScrollBar()->isVisible() &&
                indexPosition.x() > (viewport()->rect().right() -
                                     verticalScrollBar()->width()))
        {
            //Hide the tooltip.
            knMusicGlobal->detailTooltip()->hide();
            //Finished.
            return true;
        }
        //We don't need to check the proxy model. Because the indexAt() will
        //get a invalid index if the proxy model is null.
        //Set the index of the current music model to the detail tooltip.
        knMusicGlobal->detailTooltip()->setPreviewIndex(
                    musicModel(),
                    m_proxyModel->mapToSource(mouseIndex));
        //Show the tooltip at the specific position.
        QRect indexRect=visualRect(mouseIndex);
        knMusicGlobal->detailTooltip()->showTooltip(
                    mapToGlobal(
                        QPoint(indexPosition.x(),
                               indexRect.y()+(indexRect.height()>>1))));
        //Give the focus back to this tree view.
        setFocus();
    }
    return true;
}

void KNMusicListViewBase::setAutoBuildProxy(bool autoBuildProxy)
{
    m_autoBuildProxy = autoBuildProxy;
}

