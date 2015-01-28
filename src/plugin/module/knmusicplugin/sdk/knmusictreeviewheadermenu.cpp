/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QAction>
#include <QFrame>
#include <QSignalMapper>

#include "knlocalemanager.h"

#include "knmusictreeviewheadermenu.h"

#include <QDebug>

KNMusicTreeViewHeaderMenu::KNMusicTreeViewHeaderMenu(QWidget *parent) :
    KNAnimationMenu(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Set seperator style sheet, I can't solve this bug in coding way.
    setSeparatorColor(QColor(255, 255, 255, 100));

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

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicTreeViewHeaderMenu::retranslate);
    //Do retranslate.
    retranslate();
}

void KNMusicTreeViewHeaderMenu::syncVisibleState(const int &logicalIndex,
                                                 const bool &visible)
{
    m_itemVisible[logicalIndex]->setChecked(visible);
}

void KNMusicTreeViewHeaderMenu::retranslate()
{
    //Set tweak width action text.
    m_tweakWidth->setText(tr("Auto Size Column"));
    m_tweakAllWidth->setText(tr("Auto Size All Columns"));
    //Set visible action text.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        m_itemVisible[i]->setText(m_musicGlobal->treeViewHeaderText(i));
    }
}

void KNMusicTreeViewHeaderMenu::setMouseDownLogicalIndex(const int &index)
{
    m_tweakWidth->setVisible(index!=-1 && index<MusicDataCount);
    m_mouseDownLogicalIndex=index;
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
    m_tweakWidth=new QAction(this);
    connect(m_tweakWidth, SIGNAL(triggered()),
            this, SLOT(onActionResizeCurrent()));
    addAction(m_tweakWidth);
    m_tweakAllWidth=new QAction(this);
    connect(m_tweakAllWidth, SIGNAL(triggered()),
            this, SIGNAL(requireResizeAll()));
    addAction(m_tweakAllWidth);

    addSeparator();
}

void KNMusicTreeViewHeaderMenu::createVisibleMenu()
{
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        m_itemVisible[i]=new QAction(this);
        m_itemVisible[i]->setCheckable(true);
        connect(m_itemVisible[i], SIGNAL(triggered()),
                m_visibleMapper, SLOT(map()));
        m_visibleMapper->setMapping(m_itemVisible[i], i);
        addAction(m_itemVisible[i]);
    }
}
