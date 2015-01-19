/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>

#include "knhighlightlabel.h"

#include <QDebug>

KNHighlightLabel::KNHighlightLabel(QWidget *parent) :
    QLabel(parent)
{
    //Set properties.
    setScaledContents(true);
    //Initial the highlight gradient.
    m_highlight=QLinearGradient(QPointF(0,0), QPointF(0, height()));
    m_highlight.setColorAt(0, QColor(0xff,0xff,0xff,130));
    m_highlight.setColorAt(1, QColor(0xff,0xff,0xff,0));

    //Initial the highlight area.
    m_highLightArea[0]=QPointF(0,0);
    updateHighlightArea();
}

void KNHighlightLabel::paintEvent(QPaintEvent *event)
{
    //Draw the label contents.
    QLabel::paintEvent(event);
    //Create a antialiasing painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Set pen and brush.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_highlight);
    //Draw the high light.
    painter.drawPolygon(m_highLightArea, 3);
}

void KNHighlightLabel::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QLabel::resizeEvent(event);
    //Update the highlight.
    updateHighlightArea();
}

void KNHighlightLabel::updateHighlightArea()
{
    //Calculate the light area depend on the size.
    m_highlight.setFinalStop(0, height());
    m_highLightArea[1]=QPointF((float)width()*0.7, 0);
    m_highLightArea[2]=QPointF(0, (float)height());
}
