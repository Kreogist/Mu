/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QScrollBar>
#include <QTimeLine>
#include <QMouseEvent>

#include "knconnectionhandler.h"
#include "knmusicmodel.h"
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
        //Release the current model, and get a new avaliable proxy model.
        m_proxyModelPool->release(m_proxyModel);
        m_proxyModel=m_proxyModelPool->alloct();
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
                showSoloMenu(event);
                break;
            default:
                showMultiMenu(event);
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

void KNMusicTreeViewBase::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMusicTreeViewBase::onActionMouseInOut);
}

void KNMusicTreeViewBase::showSoloMenu(QMouseEvent *event)
{
    //Get the index of the position where mouse pressed.
    QModelIndex pressedIndex=indexAt(event->pos());
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
        soloMenu->setMouseDownPos(event->globalPos());
        //Launch it.
        soloMenu->exec(event->globalPos());
        //Disconnect all the signals.
        m_soloConnections->disConnectAll();
    }
}

void KNMusicTreeViewBase::showMultiMenu(QMouseEvent *event)
{
    if(indexAt(event->pos()).isValid())
    {
        KNMusicMultiMenuBase *multiMenu=KNMusicGlobal::instance()->multiMenu();
        m_multiConnections->addConnectionHandle(
                            connect(multiMenu, &KNMusicMultiMenuBase::requireDeleteSelection,
                                    this, &KNMusicTreeViewBase::removeSelections));
        //Set position.
        multiMenu->setMouseDownPos(event->globalPos());
        //Launch it.
        multiMenu->exec(event->globalPos());
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
    m_proxyModel->removeMusicRow(index.row());
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
