/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNDROPPROXYCONTAINER_H
#define KNDROPPROXYCONTAINER_H

#include <QStringList>

#include <QWidget>

class KNDropProxyContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KNDropProxyContainer(QWidget *parent = 0);

signals:
    void requireAnalysisFiles(const QStringList &urls);
    void dragEntered();
    void dropped();

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // KNDROPPROXYCONTAINER_H
