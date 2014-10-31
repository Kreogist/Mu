/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>

#include "knmessagecontent.h"

KNMessageContent::KNMessageContent(QWidget *parent) :
    QWidget(parent)
{
}

int KNMessageContent::preferHeight()
{
    return (m_shadowHeight<<1)+
            ((m_content==nullptr)?0:m_content->sizeHint().height());
}

void KNMessageContent::paintEvent(QPaintEvent *event)
{
    //Draw original content.
    QWidget::paintEvent(event);
    //Draw shadows.
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    //Draw up bound shadow.
    QLinearGradient shadowGradient(0,0,0,m_shadowHeight);
    shadowGradient.setColorAt(0, QColor(0,0,0,33));
    shadowGradient.setColorAt(1, QColor(0,0,0,0));
    painter.setBrush(shadowGradient);
    painter.drawRect(0,0,width(),m_shadowHeight);
    //Draw down bound shadow.
    painter.translate(0, height()-m_shadowHeight);
    shadowGradient.setColorAt(0, QColor(0,0,0,0));
    shadowGradient.setColorAt(1, QColor(0,0,0,20));
    painter.setBrush(shadowGradient);
    painter.drawRect(0,0,width(),m_shadowHeight);
}

QWidget *KNMessageContent::content() const
{
    return m_content;
}

void KNMessageContent::setContent(QWidget *content)
{
    m_content = content;
}
