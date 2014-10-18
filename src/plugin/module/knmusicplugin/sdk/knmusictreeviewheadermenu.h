/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICTREEVIEWHEADERMENU_H
#define KNMUSICTREEVIEWHEADERMENU_H

#include "knanimationmenu.h"

#include "knmusicglobal.h"

using namespace KNMusic;

class QAction;
class QSignalMapper;
class KNMusicTreeViewHeaderMenu : public KNAnimationMenu
{
    Q_OBJECT
public:
    explicit KNMusicTreeViewHeaderMenu(QWidget *parent = 0);
    void syncVisibleState(const int &logicalIndex, const bool &ok);

signals:
    void requireResize(const int &index);
    void requireResizeAll();
    void requireSetColumnVisible(const int &column,
                                 const bool &ok);

public slots:
    void retranslate();
    void retranslateAndSet();
    void setMouseDownLogicalIndex(const int &index);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionVisibleChange(const int &index);
    void onActionResizeCurrent();

private:
    void createWidthTweakMenu();
    void createVisibleMenu();
    KNMusicGlobal *m_musicGlobal=KNMusicGlobal::instance();
    QAction *m_tweakAllWidth, *m_tweakWidth, *m_itemVisible[MusicDataCount];
    QString m_tweakAllWidthText,
            m_tweakWidthText;
    QSignalMapper *m_visibleMapper;
    int m_mouseDownLogicalIndex=-1;
};

#endif // KNMUSICTREEVIEWHEADERMENU_H
