/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>

#include "knroundanimebutton.h"

KNRoundAnimeButton::KNRoundAnimeButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Fixed the size.
    setFixedSize(30, 30);
}

KNRoundAnimeButton::~KNRoundAnimeButton()
{

}

void KNRoundAnimeButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set the painter hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the circle.
    painter.setOpacity((qreal)m_opacity/255.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0,0,0));
    painter.drawEllipse(rect());
    //Paint the icon.
    painter.drawPixmap(5,5,20,20,m_icon);
}
