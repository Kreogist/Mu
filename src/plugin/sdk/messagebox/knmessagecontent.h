/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMESSAGECONTENT_H
#define KNMESSAGECONTENT_H

#include <QWidget>

class QBoxLayout;
class KNMessageContent : public QWidget
{
    Q_OBJECT
public:
    explicit KNMessageContent(QWidget *parent = 0);
    int preferWidth();
    int preferHeight();
    QWidget *content() const;
    void setContent(QWidget *content);
    void showContent();
    void hideContent();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_shadowHeight=9;
    QWidget *m_content=nullptr;
    QBoxLayout *m_mainLayout;
};

#endif // KNMESSAGECONTENT_H
