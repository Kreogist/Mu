/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QAction>
#include <QFrame>
#include <QPainter>
#include <QSignalMapper>

#include "knmusictreeviewheadermenu.h"

#include <QDebug>

KNMusicTreeViewHeaderMenu::KNMusicTreeViewHeaderMenu(QWidget *parent) :
    KNAnimationMenu(parent)
{
    retranslate();

    //Set properties.
    setAutoFillBackground(true);

    //Set seperator style sheet, I can't solve this bug in coding way.
    setStyleSheet("QMenu::separator {height:1px;"
                  "background: rgba(255, 255, 255, 100);"
                  "margin-left: 5px;"
                  "margin-right: 5px;}");

    //Initial signal mapper.
    m_visibleMapper=new QSignalMapper(this);
    connect(m_visibleMapper, SIGNAL(mapped(int)),
            this, SLOT(onActionVisibleChange(int)));

    //Initial menu.
    createWidthTweakMenu();
    createVisibleMenu();

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    pal.setColor(QPalette::Text, QColor(255,255,255,224));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);
}

void KNMusicTreeViewHeaderMenu::syncVisibleState(const int &logicalIndex,
                                                 const bool &visible)
{
    m_itemVisible[logicalIndex]->setChecked(visible);
}

void KNMusicTreeViewHeaderMenu::retranslate()
{
    m_tweakWidthText=tr("Auto Size Column");
    m_tweakAllWidthText=tr("Auto Size All Columns");
}

void KNMusicTreeViewHeaderMenu::retranslateAndSet()
{
    retranslate();
}

void KNMusicTreeViewHeaderMenu::setMouseDownLogicalIndex(const int &index)
{
    m_tweakWidth->setVisible(index!=-1 && index<MusicDataCount);
    m_mouseDownLogicalIndex=index;
}

void KNMusicTreeViewHeaderMenu::paintEvent(QPaintEvent *event)
{
    //Draw menu content.
    KNAnimationMenu::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Draw rects.
    painter.drawRect(rect());
}

void KNMusicTreeViewHeaderMenu::onActionVisibleChange(const int &index)
{
    emit requireSetColumnVisible(index,
                                 !m_itemVisible[index]->isChecked());
}

void KNMusicTreeViewHeaderMenu::onActionResizeCurrent()
{
    emit requireResize(m_mouseDownLogicalIndex);
}

void KNMusicTreeViewHeaderMenu::createWidthTweakMenu()
{
    //Add two tweak widget.
    m_tweakWidth=new QAction(m_tweakWidthText,
                             this);
    connect(m_tweakWidth, SIGNAL(triggered()),
            this, SLOT(onActionResizeCurrent()));
    addAction(m_tweakWidth);
    m_tweakAllWidth=new QAction(m_tweakAllWidthText,
                                this);
    connect(m_tweakAllWidth, SIGNAL(triggered()),
            this, SIGNAL(requireResizeAll()));
    addAction(m_tweakAllWidth);

    addSeparator();
}

void KNMusicTreeViewHeaderMenu::createVisibleMenu()
{
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        m_itemVisible[i]=new QAction(m_musicGlobal->treeViewHeaderText(i),
                                     this);
        m_itemVisible[i]->setCheckable(true);
        connect(m_itemVisible[i], SIGNAL(triggered()),
                m_visibleMapper, SLOT(map()));
        m_visibleMapper->setMapping(m_itemVisible[i], i);
        addAction(m_itemVisible[i]);
    }
}
