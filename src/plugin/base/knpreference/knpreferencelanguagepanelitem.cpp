/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QTimeLine>
#include <QBoxLayout>
#include <QPainter>

#include "knpreferencelanguagepanelitem.h"

KNPreferenceLanguagePanelItem::KNPreferenceLanguagePanelItem(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(m_itemHeight);

    //Initial highlight.
    m_highlight=QLinearGradient(0,0,0,m_itemHeight);
    m_highlight.setColorAt(0, QColor(0x79, 0x79, 0x79));
    m_highlight.setColorAt(1, QColor(0x4a, 0x4a, 0x4a));

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(40,40,40));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);

    //Initial the layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(10,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //Initial mouse sense timeline.
    m_mouseIn=new QTimeLine(100, this);
    m_mouseIn->setEndFrame(100);
    configureMouseInOutTimeline(m_mouseIn);
    m_mouseOut=new QTimeLine(100, this);
    m_mouseOut->setEndFrame(0);
    configureMouseInOutTimeline(m_mouseOut);
}

void KNPreferenceLanguagePanelItem::enterEvent(QEvent *event)
{
    //Running parent's enter event.
    QWidget::enterEvent(event);
    //Stop all animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set start parameters.
    m_mouseIn->setStartFrame(m_highLightOpacity*100);
    m_mouseIn->start();
}

void KNPreferenceLanguagePanelItem::leaveEvent(QEvent *event)
{
    //Running parent's leave event.
    QWidget::leaveEvent(event);
    //Stop all animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set start parameters.
    m_mouseOut->setStartFrame(m_highLightOpacity*100);
    m_mouseOut->start();
}

void KNPreferenceLanguagePanelItem::paintEvent(QPaintEvent *event)
{
    //Paint other things.
    QWidget::paintEvent(event);
    //Paint the highlight at the opacity.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Draw border.
    painter.setOpacity(1);
    painter.setPen(QColor(0x19, 0x19, 0x19));
    painter.drawLine(0,0,width(),0);
    //Draw highlight background.
    painter.setOpacity(m_highLightOpacity);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_highlight);
    painter.drawRect(rect());
    painter.setPen(QColor(0x52, 0x52, 0x52));
    painter.drawLine(0,0,width(),0);
    //Draw the icon.
    painter.setOpacity(1);
    painter.drawPixmap(m_iconX,0,m_languageIcon);
    //Draw the text.
    painter.setPen(QColor(0xff, 0xff, 0xff));
    painter.drawText(m_textX,0,width()-m_textX,m_itemHeight,
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_languageName);
}

void KNPreferenceLanguagePanelItem::onActionChangeHighlight(const int &frame)
{
    m_highLightOpacity=(qreal)frame/100.0;
    update();
}

void KNPreferenceLanguagePanelItem::configureMouseInOutTimeline(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNPreferenceLanguagePanelItem::onActionChangeHighlight);
}

QPixmap KNPreferenceLanguagePanelItem::languageIcon() const
{
    return m_languageIcon;
}

void KNPreferenceLanguagePanelItem::setLanguageIcon(const QPixmap &languageIcon)
{
    m_languageIcon = languageIcon.scaled(m_iconSize,
                                         m_iconSize,
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
}

QString KNPreferenceLanguagePanelItem::languageName() const
{
    return m_languageName;
}

void KNPreferenceLanguagePanelItem::setLanguageName(const QString &languageName)
{
    m_languageName = languageName;
}
