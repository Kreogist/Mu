/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QUrl>

#include "knglobal.h"

#include "kndropproxycontainer.h"

KNDropProxyContainer::KNDropProxyContainer(QWidget *parent) :
    QWidget(parent)
{
    //Enabled drops.
    setAcceptDrops(true);
}

void KNDropProxyContainer::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
        emit dragEntered();
    }
}

void KNDropProxyContainer::dropEvent(QDropEvent *event)
{
    emit dropped();
    emit requireAnalysisFiles(KNGlobal::urlToPathList(event->mimeData()->urls()));
}
