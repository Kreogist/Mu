/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QTimeLine>
#include <QPainter>

#include "knmousesenseheader.h"

KNMouseSenseHeader::KNMouseSenseHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    //Initial icons.
    m_ascendingIcon=QPixmap("://public/items/AscendingIndicator.png");
    m_descendingIcon=QPixmap("://public/items/DescendingIndicator.png");

    //Set properties.
    setAutoFillBackground(true);
    setAutoScroll(true);
    setContentsMargins(0,0,0,0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDefaultAlignment(Qt::AlignLeft);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setSectionsMovable(true);

    //Set palette.
    m_parameter=m_outBrightness;
    m_lineColor.setHsv(m_lineColor.hue(),
                       m_lineColor.saturation(),
                       m_outBrightness);
    m_backgroundColor=m_lineColor;
    m_palette=palette();
    m_palette.setColor(QPalette::Base, QColor(0,0,0,0));
    m_palette.setColor(QPalette::Window, QColor(0,0,0,0));
    m_buttonGradient=QLinearGradient(QPoint(0,0), QPoint(0, height()));
    m_buttonColor.setHsv(m_buttonColor.hue(),
                         m_buttonColor.saturation(),
                         m_outBrightness+0x28);
    m_buttonGradient.setColorAt(0, m_buttonColor);
    m_buttonGradient.setColorAt(1, m_buttonColor);
    m_palette.setBrush(QPalette::Button, QBrush(m_buttonGradient));
    m_buttonTextColor.setHsv(m_buttonTextColor.hue(),
                             m_buttonTextColor.saturation(),
                             (m_outBrightness<<1)+0x7f);
    m_palette.setColor(QPalette::ButtonText, QColor(0xbf, 0xbf, 0xbf));
    setPalette(m_palette);

    //Initial the mouse timeline.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(m_inBrightness);
    configureTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(m_outBrightness);
    configureTimeLine(m_mouseOut);

    //Use mouse out to set the default value.
    onActionMouseInOut(m_outBrightness);
}

void KNMouseSenseHeader::enterEvent(QEvent *event)
{
    //Stop animations
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameters.
    m_mouseIn->setStartFrame(m_parameter);
    m_mouseIn->start();
    QHeaderView::enterEvent(event);
}

void KNMouseSenseHeader::leaveEvent(QEvent *event)
{
    //Stop animations
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseOut->setStartFrame(m_parameter);
    m_mouseOut->start();
    QHeaderView::leaveEvent(event);
}

void KNMouseSenseHeader::paintSection(QPainter *painter,
                                      const QRect &rect,
                                      int logicalIndex) const
{
    QRect contentRect;
    //Enabled Antialiasing.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform,
                            true);
    //Draw Sort Indicator if it is shown, update the content rect.
    if(logicalIndex==sortIndicatorSection() && isSortIndicatorShown())
    {
        painter->drawPixmap(QPoint(rect.x()+rect.width()-20,
                                   rect.y()+((rect.height()-20)>>1)),
                            sortIndicatorOrder()==Qt::AscendingOrder?
                                m_ascendingIcon:m_descendingIcon);
        contentRect=QRect(rect.x()+4,
                          rect.y()+1,
                          rect.width()-28,
                          rect.height()-2);
    }
    else
    {
        contentRect=QRect(rect.x()+4,
                          rect.y()+1,
                          rect.width()-8,
                          rect.height()-2);
    }
    painter->drawText(contentRect,
                      model()->headerData(logicalIndex, Qt::Horizontal, Qt::TextAlignmentRole).toInt(),
                      model()->headerData(logicalIndex, Qt::Horizontal).toString());
    painter->setPen(m_lineColor);
    painter->drawLine(rect.topRight(), rect.bottomRight());
}

void KNMouseSenseHeader::onActionMouseInOut(const int &frame)
{
    m_parameter=frame;
    m_lineColor.setHsv(m_lineColor.hue(),
                       m_lineColor.saturation(),
                       m_parameter<<1);
    m_buttonColor.setHsv(m_buttonColor.hue(),
                         m_buttonColor.saturation(),
                         m_parameter+0x38);
    m_buttonTextColor.setHsv(m_buttonTextColor.hue(),
                             m_buttonTextColor.saturation(),
                             (m_parameter<<1)+0x7f);
    m_buttonGradient.setColorAt(0, m_buttonColor);
    m_palette.setBrush(QPalette::Button, QBrush(m_buttonGradient));
    m_palette.setColor(QPalette::ButtonText, m_buttonTextColor);

    setPalette(m_palette);
}

void KNMouseSenseHeader::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMouseSenseHeader::onActionMouseInOut);
}
