/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMESSAGECONTENT_H
#define KNMESSAGECONTENT_H

#include <QWidget>

class KNMessageContent : public QWidget
{
    Q_OBJECT
public:
    explicit KNMessageContent(QWidget *parent = 0);
    int preferHeight();
    QWidget *content() const;
    void setContent(QWidget *content);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_shadowHeight=9;
    QWidget *m_content=nullptr;
};

#endif // KNMESSAGECONTENT_H
