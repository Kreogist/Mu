/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QBoxLayout>
#include <QPainter>

#include "knmessagecontent.h"

#include <QDebug>

KNMessageContent::KNMessageContent(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    //Initial the layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                this);
    m_mainLayout->setContentsMargins(0,m_shadowHeight,0,m_shadowHeight);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

int KNMessageContent::preferWidth()
{
    return (m_content==nullptr)?0:m_content->sizeHint().width();
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
    painter.fillRect(QRect(0,0,width(),m_shadowHeight), shadowGradient);
    //Draw down bound shadow.
    painter.translate(0, height()-m_shadowHeight);
    shadowGradient.setColorAt(0, QColor(0,0,0,0));
    shadowGradient.setColorAt(1, QColor(0,0,0,20));
    painter.fillRect(QRect(0,0,width(),m_shadowHeight), shadowGradient);
}

QWidget *KNMessageContent::content() const
{
    return m_content;
}

void KNMessageContent::setContent(QWidget *content)
{
    m_content = content;
    m_mainLayout->addWidget(m_content);
}

void KNMessageContent::showContent()
{
    if(m_content!=nullptr)
    {
        m_content->show();
    }
}

void KNMessageContent::hideContent()
{
    if(m_content!=nullptr)
    {
        m_content->hide();
    }
}
