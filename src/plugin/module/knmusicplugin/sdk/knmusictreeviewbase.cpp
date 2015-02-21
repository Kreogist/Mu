/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QScopedPointer>
#include <QMimeData>
#include <QPainter>
#include <QList>
#include <QKeyEvent>
#include <QDataStream>
#include <QDrag>
#include <QScrollBar>
#include <QTimeLine>
#include <QMouseEvent>
#include <QJsonDocument>

#include "knconnectionhandler.h"
#include "knmusicdetailtooltipbase.h"
#include "knmusicmodel.h"
#include "knmusicmodelassist.h"
#include "knmusicsearchbase.h"
#include "knmusicsolomenubase.h"
#include "knmusicmultimenubase.h"
#include "knmusicnowplayingbase.h"
#include "knmusictreeviewheader.h"
#include "knmusicproxymodel.h"
#include "knmusicratingdelegate.h"

#include "knmusictreeviewbase.h"

#include <QDebug>

KNMusicTreeViewBase::KNMusicTreeViewBase(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setAllColumnsShowFocus(true);
    setContentsMargins(0,0,0,0);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDropIndicatorShown(true);
    setFrameShape(QFrame::NoFrame);
    setIndentation(0);
    setMouseTracking(true);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformRowHeights(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //Set scroll bar properties.
    horizontalScrollBar()->setSingleStep(5);
    horizontalScrollBar()->setPageStep(5);
    verticalScrollBar()->setSingleStep(4);
    verticalScrollBar()->setPageStep(4);

    //Using frame updater to set palette.
    onActionMouseInOut(0);

    //Set animation header.
    KNMusicTreeViewHeader *header=new KNMusicTreeViewHeader(this);
    connect(header, &KNMusicTreeViewHeader::requireResizeColumnToContents,
            this, &KNMusicTreeViewBase::resizeColumnToContents);
    setHeader(header);

    //Set delegate.
    setItemDelegateForColumn(Rating,
                             new KNMusicRatingDelegate(this));
    setItemDelegateForColumn(AlbumRating,
                             new KNMusicRatingDelegate(this));

    //Initial menu connections.
    m_soloConnections=new KNConnectionHandler(this);
    m_multiConnections=new KNConnectionHandler(this);

    //Initial mouse sense animation.
    m_mouseIn=new QTimeLine(200, this);
    configureTimeLine(m_mouseIn);
    m_mouseIn->setEndFrame(m_maxOpacity);
    m_mouseOut=new QTimeLine(200, this);
    configureTimeLine(m_mouseOut);
    m_mouseOut->setEndFrame(0);

    //Initial reacts.
    connect(this, &KNMusicTreeViewBase::activated,
            this, &KNMusicTreeViewBase::playIndex);

    //Initial music global.
    m_musicGlobal=KNMusicGlobal::instance();

    //Initial actions.
    initialActions();
}

KNMusicModel *KNMusicTreeViewBase::musicModel()
{
    return m_proxyModel==nullptr?nullptr:m_proxyModel->musicModel();
}

void KNMusicTreeViewBase::setMusicModel(KNMusicModel *musicModel)
{
    //Hide the detail tool tip first.
    KNMusicGlobal::detailTooltip()->hide();
    //If we're going to set the model to null, backup header first.
    if(musicModel==nullptr)
    {
        backupHeader();
    }
    else
    {
        //Before we set the source model, we need to check the proxy model.
        if(m_musicGlobal->nowPlaying()->playingMusicModel()==proxyModel()->musicModel())
        {
            m_musicGlobal->nowPlaying()->shadowPlayingModel();
        }
    }
    //Set the source model.
    proxyModel()->setSourceModel(musicModel);
    //Check and do header reset.
    if(m_initialLoad)
    {
        //Clear the initial load flag.
        m_initialLoad=false;
        resetHeaderState();
        //Clear the header state backup data.
        m_headerState.clear();
        return;
    }
    //Check if we need to load header state from backup.
    if(musicModel!=nullptr && !m_headerState.isEmpty())
    {
        header()->restoreState(m_headerState);
        m_headerState.clear();
    }
}

void KNMusicTreeViewBase::backupHeader()
{
    m_headerState=header()->saveState();
}

void KNMusicTreeViewBase::enableSearchShortcut()
{
    addAction(m_findAction);
}

void KNMusicTreeViewBase::disableSearchShortcut()
{
    removeAction(m_findAction);
}

void KNMusicTreeViewBase::scrollToSourceSongRow(const int &row)
{
    //Do scroll and ensure that the music model exist.
    if(m_proxyModel->musicModel()!=nullptr)
    {
        scrollToSongIndex(m_proxyModel->mapFromSource(
                              m_proxyModel->musicModel()->index(row, Name)));
    }
}

void KNMusicTreeViewBase::scrollToSongRow(const int &row)
{
    //Get the target index.
    scrollToSongIndex(m_proxyModel->index(row, Name));
}

inline void KNMusicTreeViewBase::scrollToSongIndex(const QModelIndex &songIndex)
{
    //Set the current index to that row.
    setCurrentIndex(songIndex);
    //And move to that index.
    scrollTo(songIndex, QAbstractItemView::PositionAtCenter);
}

void KNMusicTreeViewBase::resetHeaderState()
{
    //Hide all column first.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        setColumnHidden(i, true);
    }
    //Show the default columns.
    setColumnHidden(Time, false);
    setColumnHidden(Artist, false);
    setColumnHidden(Album, false);
    setColumnHidden(Genre, false);
    setColumnHidden(Rating, false);
    //Reorder the columns.
    moveToFirst(Rating);
    moveToFirst(Genre);
    moveToFirst(Album);
    moveToFirst(Artist);
    moveToFirst(Time);
    moveToFirst(Name);
    //Set the default width.
    setColumnWidth(Genre, 81);
    setColumnWidth(Album, 126);
    setColumnWidth(Artist, 126);
    setColumnWidth(Time, 60);
    setColumnWidth(Name, 200);
}

void KNMusicTreeViewBase::searchText(QString text)
{
    //Backup the search text.
    m_seachText=text;
    //Set to proxy model's filter.
    if(m_proxyModel!=nullptr)
    {
        //Do search.
        m_proxyModel->setFilterFixedString(m_seachText);
        if(currentIndex().isValid())
        {
            scrollTo(model()->index(currentIndex().row(),
                                    0),
                     QAbstractItemView::PositionAtCenter);
        }
        //Emit search signal.
        emit searchComplete();
    }
}

void KNMusicTreeViewBase::sortMusicColumn(int column,
                                          Qt::SortOrder order)
{
    m_proxyModel->sort(column, order);
}

void KNMusicTreeViewBase::enterEvent(QEvent *event)
{
    QTreeView::enterEvent(event);
    if(m_animate)
    {
        //Stop timelines.
        m_mouseIn->stop();
        m_mouseOut->stop();
        //Set parameters.
        m_mouseIn->setStartFrame(m_alternateColor.alpha());
        m_mouseIn->start();
    }
}

void KNMusicTreeViewBase::leaveEvent(QEvent *event)
{
    QTreeView::leaveEvent(event);
    if(m_animate)
    {
        //Stop timelines.
        m_mouseIn->stop();
        m_mouseOut->stop();
        //Set parameters.
        m_mouseOut->setStartFrame(m_alternateColor.alpha());
        m_mouseOut->start();
    }
}

void KNMusicTreeViewBase::keyReleaseEvent(QKeyEvent *event)
{
    QRect selectedRect;
    //Get the key event
    switch(event->key())
    {
    case Qt::Key_Menu:
        //Hide preview tooltips.
        KNMusicGlobal::detailTooltip()->hide();
        //Check the select rows count.
        switch(selectionModel()->selectedRows().size())
        {
        case 0:
            break;
        case 1:
            //Calculate the selected rows position.
            selectedRect=visualRect(currentIndex());
            showSoloMenu(QPoint(0,
                                selectedRect.y()));
            break;
        default:
            //Calculate the selected rows position.
            selectedRect=visualRect(currentIndex());
            showMultiMenu(QPoint(0,
                                 selectedRect.y()));
            break;
        }
    default:
        QTreeView::keyReleaseEvent(event);
        break;
    }
}

bool KNMusicTreeViewBase::event(QEvent *event)
{
    if(event->type()==QEvent::ToolTip)
    {
        //Get the help event from the event.
        QHelpEvent *helpEvent=static_cast<QHelpEvent *>(event);
        //Get the right position of the index.
        QPoint indexPosition=QPoint(helpEvent->pos().x(),
                                    helpEvent->pos().y()-header()->height());
        //If the pointer is on the header, hide the detail tooltip.
        if(indexPosition.y()<0)
        {
            KNMusicGlobal::detailTooltip()->hide();
            return false;
        }
        //Locate the index via the position.
        QModelIndex mouseIndex=indexAt(indexPosition);
        if(mouseIndex.isValid())
        {
            if(verticalScrollBar()->isVisible() &&
                    indexPosition.x()>(viewport()->rect().right()-verticalScrollBar()->width()))
            {
                KNMusicGlobal::detailTooltip()->hide();
            }
            else
            {
                KNMusicGlobal::detailTooltip()->setPreviewIndex(m_proxyModel->musicModel(),
                                                                m_proxyModel->mapToSource(mouseIndex),
                                                                helpEvent->globalPos());
                KNMusicGlobal::detailTooltip()->showTooltip();
            }
        }
        return true;
    }
    return QTreeView::event(event);
}

void KNMusicTreeViewBase::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions)
    //Get all the selections.
    QModelIndexList indexes=selectionModel()->selectedRows(Name);
    if(indexes.isEmpty())
    {
        return;
    }
    //Clear the mimedata.
    QScopedPointer<QDrag> drag(new QDrag(this));
    QMimeData *mimeData=m_proxyModel->mimeData(selectionModel()->selectedIndexes());
    //Get the file path and music row of all the selected rows.
    QList<QUrl> fileUrlList;
    QJsonArray musicRowList;
    KNMusicModel *musicModel=m_proxyModel->musicModel();
    QByteArray sourceRowList;
    QDataStream sourceRowStream(&sourceRowList, QIODevice::WriteOnly);
    for(auto i=indexes.begin();
             i!=indexes.end();
             ++i)
    {
        //Get the original row of the specific row.
        int currentRow=m_proxyModel->mapToSource(*i).row();
        //Add the file path, row index and music row to list.
        fileUrlList.append(QUrl::fromLocalFile(musicModel->filePathFromRow(currentRow)));
        musicRowList.append(KNMusicModelAssist::rowToJsonArray(musicModel, currentRow));
        sourceRowStream << currentRow;
    }
    QJsonDocument musicRowDocument;
    musicRowDocument.setArray(musicRowList);
    //Set the data to mimedata.
    mimeData->setUrls(fileUrlList);
    mimeData->setData("org.kreogist.mu.musicrowlist", musicRowDocument.toBinaryData());
    mimeData->setData("org.kreogist.mu.musicmodel", QByteArray::number((qint64)musicModel));
    mimeData->setData("org.kreogist.mu.musicmodelrows", sourceRowList);
    mimeData->setData("org.kreogist.mu.musicmodelrowsize", QByteArray::number(indexes.size()));
    //Set the mime data to the drag action.
    drag->setMimeData(mimeData);
    //Do the drag.
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void KNMusicTreeViewBase::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);
    //Set pressed flag.
    m_pressed=true;
}

void KNMusicTreeViewBase::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    //Check we has been pressed on this widget before.
    if(m_pressed)
    {
        m_pressed=false;
        //Check is button right button and position is in the treeview.
        if(event->button()==Qt::RightButton && rect().contains(event->pos()))
        {
            //Hide preview tooltips.
            KNMusicGlobal::detailTooltip()->hide();
            //According to the selected rows to display menu.
            switch(selectionModel()->selectedRows().size())
            {
            case 0:
                break;
            case 1:
                showSoloMenu(event->pos());
                break;
            default:
                showMultiMenu(event->pos());
                break;
            }
        }
    }
}

void KNMusicTreeViewBase::moveToFirst(const int &logicalIndex)
{
    //The reorder function: move section!
    header()->moveSection(header()->visualIndex(logicalIndex), 0);
}

void KNMusicTreeViewBase::drawRow(QPainter *painter,
                                  const QStyleOptionViewItem &options,
                                  const QModelIndex &index) const
{
    if((index.row()&1))
    {
        //Draw the alternative background.
        painter->fillRect(QRect(options.rect.x(),
                                options.rect.y(),
                                width(),
                                options.rect.height()),
                          m_alternateColor);
    }
    QTreeView::drawRow(painter, options, index);
}

void KNMusicTreeViewBase::setAnimateState(bool on)
{
    m_animate=on;
}

KNMusicProxyModel *KNMusicTreeViewBase::proxyModel()
{
    //Check is the proxy model need to initial.
    if(m_proxyModel==nullptr)
    {
        //Initial the proxy model.
        m_proxyModel=new KNMusicProxyModel(this);
        //Set the search text.
        m_proxyModel->setFilterFixedString(m_seachText);
        //Set the proxy model.
        setModel(m_proxyModel);
    }
    return m_proxyModel;
}

void KNMusicTreeViewBase::onActionSearch()
{
    //Set focus.
    KNMusicGlobal::musicSearch()->setSearchFocus(this);
}

void KNMusicTreeViewBase::onActionMouseInOut(const int &frame)
{
    QPalette pal=palette();
    m_alternateColor.setAlpha(frame);
    m_fontColor.setAlpha(m_fontBase+(frame<<1));
    m_buttonColor.setHsv(m_buttonColor.hue(),
                         m_buttonColor.saturation(),
                         frame<<1);
    pal.setColor(QPalette::Text, m_fontColor);
    pal.setColor(QPalette::Button, m_buttonColor);
    setPalette(pal);
}

void KNMusicTreeViewBase::playCurrent()
{
    playIndex(currentIndex());
}

void KNMusicTreeViewBase::removeCurrent()
{
    removeIndex(currentIndex());
}

void KNMusicTreeViewBase::renameCurrent(const QString &preferName)
{
    //Rename the file.
    KNMusicGlobal::instance()->renameMusicFile(
                m_proxyModel->filePathFromRow(currentIndex().row()),
                preferName);
    //Change the record in the proxy model.
}

void KNMusicTreeViewBase::initialActions()
{
    //Initial the search action
    m_findAction=new QAction(this);
    m_findAction->setShortcut(QKeySequence(QKeySequence::Find));
    m_findAction->setShortcutContext(Qt::WidgetShortcut);
    connect(m_findAction, SIGNAL(triggered()),
            this, SLOT(onActionSearch()));

    enableSearchShortcut();
}

void KNMusicTreeViewBase::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMusicTreeViewBase::onActionMouseInOut);
}

void KNMusicTreeViewBase::showSoloMenu(const QPoint &position)
{
    //Get the index of the position where mouse pressed.
    QModelIndex pressedIndex=indexAt(position);
    if(pressedIndex.isValid())
    {
        KNMusicSoloMenuBase *soloMenu=KNMusicGlobal::instance()->soloMenu();
        m_soloConnections->addConnectionHandle(
                    connect(soloMenu, &KNMusicSoloMenuBase::requirePlayCurrent,
                            this, &KNMusicTreeViewBase::playCurrent));
        m_soloConnections->addConnectionHandle(
                    connect(soloMenu, &KNMusicSoloMenuBase::requireRemoveCurrent,
                            this, &KNMusicTreeViewBase::removeCurrent));
        m_soloConnections->addConnectionHandle(
                    connect(soloMenu, &KNMusicSoloMenuBase::requireRenameCurrent,
                            this, &KNMusicTreeViewBase::renameCurrent));
        //Set proxy model and current index.
        soloMenu->setProxyModel(m_proxyModel);
        soloMenu->setCurrentIndex(pressedIndex);
        //Set position.
        QPoint menuPosition=mapToGlobal(position);
        menuPosition.setY(menuPosition.y()+header()->height());
        soloMenu->setMouseDownPos(menuPosition);
        //Launch it.
        soloMenu->exec(menuPosition);
        //Disconnect all the signals.
        m_soloConnections->disconnectAll();
    }
}

void KNMusicTreeViewBase::showMultiMenu(const QPoint &position)
{
    if(indexAt(position).isValid())
    {
        KNMusicMultiMenuBase *multiMenu=KNMusicGlobal::instance()->multiMenu();
        m_multiConnections->addConnectionHandle(
                            connect(multiMenu, &KNMusicMultiMenuBase::requireDeleteSelection,
                                    this, &KNMusicTreeViewBase::removeSelections));
        //Set position.
        QPoint menuPosition=mapToGlobal(position);
        menuPosition.setY(menuPosition.y()+header()->height());
        multiMenu->setMouseDownPos(menuPosition);
        //Launch it.
        multiMenu->exec(menuPosition);
        //Disconnect all the signals.
        m_multiConnections->disconnectAll();
    }
}

KNMusicTab *KNMusicTreeViewBase::musicTab() const
{
    return m_musicTab;
}

void KNMusicTreeViewBase::setMusicTab(KNMusicTab *musicTab)
{
    m_musicTab = musicTab;
}

void KNMusicTreeViewBase::playIndex(const QModelIndex &index)
{
    if(index.isValid())
    {
        //Set the playing model.
        m_musicGlobal->requirePlayMusicRow(m_proxyModel,
                                           index.row(),
                                           m_musicTab);
    }
}

void KNMusicTreeViewBase::removeIndex(const QModelIndex &index)
{
    QModelIndex sourceIndex=m_proxyModel->mapToSource(index);
    //Check is the current model playing, and is the index playing.
    if(m_musicGlobal->nowPlaying()->playingModel()!=nullptr &&
            m_musicGlobal->nowPlaying()->playingMusicModel()==m_proxyModel->sourceModel() &&
            m_musicGlobal->nowPlaying()->currentPlayingIndex().row()==sourceIndex.row())
    {
        //If so, ask now playing to reset current playing.
        m_musicGlobal->nowPlaying()->resetCurrentPlaying();
    }
    //Remove the row right in the proxy model.
    m_proxyModel->removeSourceMusicRow(sourceIndex.row());
}

void KNMusicTreeViewBase::removeSelections()
{
    //Check is the current playing item is in the selection.
    if(m_musicGlobal->nowPlaying()->playingModel()!=nullptr &&
            m_musicGlobal->nowPlaying()->playingMusicModel()==m_proxyModel->sourceModel())
    {
        //Get the current playing index first.
        QModelIndex playingSourceIndex=m_musicGlobal->nowPlaying()->currentPlayingIndex();
        if(playingSourceIndex.isValid())
        {
            QModelIndex playingProxyIndex=m_proxyModel->mapFromSource(playingSourceIndex);
            //Check is the playing index is in the selection.
            if(selectionModel()->selectedIndexes().contains(playingProxyIndex))
            {
                //If so, ask now playing to reset current playing.
                m_musicGlobal->nowPlaying()->resetCurrentPlaying();
            }
        }
    }
    //Get the current indexes.
    QModelIndexList selectionList=selectionModel()->selectedRows(m_proxyModel->playingItemColumn());
    //Change the model index list to persistent index.
    QList<QPersistentModelIndex> persistentList;
    while(!selectionList.isEmpty())
    {
        persistentList.append(m_proxyModel->mapToSource(selectionList.takeLast()));
    }
    //Remove all the indexes.
    while(!persistentList.isEmpty())
    {
        QPersistentModelIndex currentRemovedIndex=persistentList.takeLast();
        if(currentRemovedIndex.isValid())
        {
            m_proxyModel->removeSourceMusicRow(currentRemovedIndex.row());
        }
    }
}
