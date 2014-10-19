/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QScrollBar>
#include <QTimeLine>

#include "knmusicmodel.h"
#include "knmusictreeviewheader.h"
#include "knmusicproxymodelpool.h"

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

    //Set animation header.
    KNMusicTreeViewHeader *header=new KNMusicTreeViewHeader(this);
    connect(header, &KNMusicTreeViewHeader::requireResizeColumnToContents,
            this, &KNMusicTreeViewBase::resizeColumnToContents);
    setHeader(header);

    //Initial proxy model pool.
    m_proxyModelPool=KNMusicProxyModelPool::instance();

    //Initial mouse sense animation.
    m_mouseIn=new QTimeLine(200, this);
    configureTimeLine(m_mouseIn);
    m_mouseIn->setEndFrame(m_maxOpacity);
    m_mouseOut=new QTimeLine(200, this);
    configureTimeLine(m_mouseOut);
    m_mouseOut->setEndFrame(0);
}

KNMusicModel *KNMusicTreeViewBase::musicModel()
{
    return m_proxyModel==nullptr?nullptr:m_proxyModel->musicModel();
}

void KNMusicTreeViewBase::setMusicModel(KNMusicModel *musicModel)
{
    bool resetHeader=false;
    //Check is the current proxy model is the playing model.
    if(m_proxyModel==m_proxyModelPool->playing())
    {
        //Get a new avaliable proxy model.
        m_proxyModel=m_proxyModelPool->alloct();
        //Set the proxy model.
        setModel(m_proxyModel);
        //Set header reset flag.
        resetHeader=true;
    }
    //Set the source model.
    m_proxyModel->setSourceModel(musicModel);
    //Check and do header reset.
    if(resetHeader)
    {
        resetHeaderState();
    }
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

void KNMusicTreeViewBase::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMusicTreeViewBase::onActionMouseInOut);
}
