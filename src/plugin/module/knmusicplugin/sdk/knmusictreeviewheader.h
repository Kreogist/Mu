/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICTREEVIEWHEADER_H
#define KNMUSICTREEVIEWHEADER_H

#include "knmousesenseheader.h"

#include "knmusicglobal.h"

using namespace KNMusic;

class KNMusicTreeViewHeaderMenu;
class KNMusicTreeViewHeader : public KNMouseSenseHeader
{
    Q_OBJECT
public:
    explicit KNMusicTreeViewHeader(QWidget *parent = 0);
    ~KNMusicTreeViewHeader();

signals:
    void requireResizeColumnToContents(int column);

public slots:

private slots:
    void resizeAllColumns();
    void resizeColumn(const int &logicalIndex);
    void showCustomContextMenu(const QPoint &mousePoint);

private:
    KNMusicTreeViewHeaderMenu *m_headerMenu;
};

#endif // KNMUSICTREEVIEWHEADER_H
