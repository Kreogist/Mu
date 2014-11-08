/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QMimeData>
#include <QList>
#include <QKeyEvent>
#include <QDataStream>
#include <QDrag>
#include <QScrollBar>
#include <QTimeLine>
#include <QMouseEvent>

#include "knconnectionhandler.h"
#include "knmusicmodel.h"
#include "knmusicsearchbase.h"
#include "knmusicsolomenubase.h"
#include "knmusicmultimenubase.h"
#include "knmusicnowplayingbase.h"
#include "knmusictreeviewheader.h"
#include "knmusicproxymodelpool.h"
#include "knmusicratingdelegate.h"

#include "knmusictreeviewbase.h"

#include <QDebug>

KNMusicTreeViewBase::KNMusicTreeViewBase(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFrameShape(QFrame::NoFrame);
    setIndentation(0);
    setMouseTracking(true);
    setUniformRowHeights(true);
    setAllColumnsShowFocus(true);
    setDragDropMode(QAbstractItemView::DragOnly);
    setAlternatingRowColors(true);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::SelectedClicked |
                    QAbstractItemView::EditKeyPressed);

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

    //Initial proxy model pool.
    m_proxyModelPool=KNMusicProxyModelPool::instance();

    //Initial the mime data and drag action.
    m_mimeData=new QMimeData;
    m_drag=new QDrag(this);

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

    //Initial actions.
    initialActions();
}

KNMusicModel *KNMusicTreeViewBase::musicModel()
{
    return m_proxyModel==nullptr?nullptr:m_proxyModel->musicModel();
}

void KNMusicTreeViewBase::setMusicModel(KNMusicModel *musicModel)
{
    //Check is the current proxy model is the playing model.
    if(m_proxyModel==nullptr ||
            m_proxyModelPool->isModelPlaying(m_proxyModel))
    {
        //Backup myself.
        backupHeader();
        //Release the current model, and get a new avaliable proxy model.
        m_proxyModelPool->release(m_proxyModel);
        m_proxyModel=m_proxyModelPool->alloct();
        //Initial the proxy model.
        m_proxyModel->setFilterFixedString(m_seachText);
        //Set the proxy model.
        setModel(m_proxyModel);
    }
    //Set the source model.
    m_proxyModel->setSourceModel(musicModel);
    //Check and do header reset.
    if(m_initialLoad)
    {
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
        m_proxyModel->setFilterFixedString(m_seachText);
    }
}

void KNMusicTreeViewBase::enterEvent(QEvent *event)
{
    QTreeView::enterEvent(event);
    //Stop timelines.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseIn->setStartFrame(m_alternateColor.alpha());
    m_mouseIn->start();
}

void KNMusicTreeViewBase::leaveEvent(QEvent *event)
{
    QTreeView::leaveEvent(event);
    //Stop timelines.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseOut->setStartFrame(m_alternateColor.alpha());
    m_mouseOut->start();
}

void KNMusicTreeViewBase::keyReleaseEvent(QKeyEvent *event)
{
    QRect selectedRect;
    //Get the key event
    switch(event->key())
    {
    case Qt::Key_Menu:
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
    m_mimeData->clear();
    //Get the file path and music row of all the selected rows.
    QList<QUrl> fileUrlList;
    QList<QList<QStandardItem *>> songContentRowList;
    KNMusicModel *musicModel=m_proxyModel->musicModel();
    for(auto i=indexes.begin();
             i!=indexes.end();
             ++i)
    {
        //Get the original row of the specific row.
        int currentRow=m_proxyModel->mapToSource(*i).row();
        //Add the file path and music row to list.
        fileUrlList.append(QUrl::fromLocalFile(musicModel->filePathFromRow(currentRow)));
        songContentRowList.append(musicModel->songRow(currentRow));
    }
    //Store the row data.
    KNMusicGlobal::setDragMusicRow(songContentRowList);
    //Set the data to mimedata.
    m_mimeData->setUrls(fileUrlList);
    m_mimeData->setData(KNMusicGlobal::musicRowFormat(),
                        QByteArray(1,0)); //Only a data for flag.
    //Set the mime data to the drag action.
    m_drag->setMimeData(m_mimeData);
    //Do the drag.
    m_drag->exec(Qt::CopyAction);
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

void KNMusicTreeViewBase::onActionMouseInOut(const int &frame)
{
    QPalette pal=palette();
    m_alternateColor.setAlpha(frame);
    m_fontColor.setAlpha(m_fontBase+(frame<<1));
    m_buttonColor.setHsv(m_buttonColor.hue(),
                         m_buttonColor.saturation(),
                         frame<<1);
    pal.setColor(QPalette::AlternateBase, m_alternateColor);
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

void KNMusicTreeViewBase::initialActions()
{
    //Initial the search action
    QAction *findAction=new QAction(this);
    findAction->setShortcut(QKeySequence(QKeySequence::Find));
    findAction->setShortcutContext(Qt::WidgetShortcut);
    connect(findAction, SIGNAL(triggered()),
            KNMusicGlobal::musicSearch(), SLOT(setSearchFocus()));
    addAction(findAction);
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
        m_soloConnections->disConnectAll();
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
        m_multiConnections->disConnectAll();
    }
}

void KNMusicTreeViewBase::playIndex(const QModelIndex &index)
{
    if(index.isValid())
    {
        KNMusicGlobal::nowPlaying()->setPlayingModel(m_proxyModel);
        KNMusicGlobal::nowPlaying()->playMusic(index);
    }
}

void KNMusicTreeViewBase::removeIndex(const QModelIndex &index)
{
    //Check is the current model playing.
    if(KNMusicGlobal::nowPlaying()->playingModel()==m_proxyModel)
    {
        //If so, ask now playing to check the index.
        KNMusicGlobal::nowPlaying()->checkRemovedIndex(index);
    }
    //Remove the row right in the proxy model.
    m_proxyModel->removeSourceMusicRow(m_proxyModel->mapToSource(index).row());
}

void KNMusicTreeViewBase::removeSelections()
{
    //Get the current indexes.
    QModelIndexList selectionList=selectionModel()->selectedRows(Name);
    QList<QPersistentModelIndex> persistentList;
    while(!selectionList.isEmpty())
    {
        persistentList.append(QPersistentModelIndex(selectionList.takeLast()));
    }
    //Remove all the indexes.
    while(!persistentList.isEmpty())
    {
        QPersistentModelIndex currentRemovedIndex=persistentList.takeLast();
        if(currentRemovedIndex.isValid())
        {
            removeIndex(currentRemovedIndex);
        }
    }
}
