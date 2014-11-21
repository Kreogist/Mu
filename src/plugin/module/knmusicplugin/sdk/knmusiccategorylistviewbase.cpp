/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QStyleFactory>
#include <QTimeLine>

#include "knmusiccategorylistviewbase.h"

KNMusicCategoryListViewBase::KNMusicCategoryListViewBase(QWidget *parent) :
    QListView(parent)
{
    //Set viewport properties.
    viewport()->setContentsMargins(0,0,0,0);

    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setLineWidth(0);
    setLayoutMode(QListView::Batched); //For speed up.
    setMinimumWidth(200);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSpacing(2);
    setUniformItemSizes(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    //Initial the background color and mouse timeline.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             m_outBrightness);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, QColor(0x30, 0x30, 0x30));
    m_palette.setColor(QPalette::Button, QColor(0x30, 0x30, 0x30));
    m_palette.setColor(QPalette::Text, QColor(0x8f, 0x8f, 0x8f, 0xaf));
    m_palette.setColor(QPalette::Highlight, QColor(0xff, 0xff, 0xff, 0x40));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);
    //Initial the background by mouse in out function.
    onActionMouseInOut(m_outBrightness);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(m_inBrightness);
    configureTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(m_outBrightness);
    configureTimeLine(m_mouseOut);
}

void KNMusicCategoryListViewBase::enabledDragDrop()
{
    //Enabled viewport drag and drop
    viewport()->setAcceptDrops(true);
    //Enabled drag and drop properties.
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDropIndicatorShown(true);
}

void KNMusicCategoryListViewBase::enterEvent(QEvent *event)
{
    //Stop animations.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameters.
    m_mouseIn->setStartFrame(m_backgroundColor.value());
    m_mouseIn->start();
    QListView::enterEvent(event);
}

void KNMusicCategoryListViewBase::leaveEvent(QEvent *event)
{
    //Stop animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseOut->setStartFrame(m_backgroundColor.value());
    m_mouseOut->start();
    QListView::leaveEvent(event);
}

void KNMusicCategoryListViewBase::onActionMouseInOut(const int &frame)
{
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             frame);
    m_textColor.setHsv(m_textColor.hue(),
                       m_textColor.saturation(),
                       frame+m_textBrightnessGap);
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Text, m_textColor);
    setPalette(m_palette);
}

void KNMusicCategoryListViewBase::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(5);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMusicCategoryListViewBase::onActionMouseInOut);
}
