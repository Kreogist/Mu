/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QTimeLine>

#include "knmousesensewidget.h"

KNMouseSenseWidget::KNMouseSenseWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    //Initial the background color and mouse timeline.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             m_outBrightness);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, QColor(0,0,0,0));
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    m_palette.setColor(QPalette::Button, QColor(0x30, 0x30, 0x30));
    m_palette.setColor(QPalette::Text, QColor(0x8f, 0x8f, 0x8f, 0xaf));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(m_inBrightness);
    configureTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(m_outBrightness);
    configureTimeLine(m_mouseOut);
}

void KNMouseSenseWidget::enterEvent(QEvent *event)
{
    //Stop animations.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameters.
    m_mouseIn->setStartFrame(m_backgroundColor.value());
    m_mouseIn->start();
    QWidget::enterEvent(event);
}

void KNMouseSenseWidget::leaveEvent(QEvent *event)
{
    //Stop animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseOut->setStartFrame(m_backgroundColor.value());
    m_mouseOut->start();
    QWidget::leaveEvent(event);
}

void KNMouseSenseWidget::onActionMouseInOut(const int &frame)
{
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             frame);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}

void KNMouseSenseWidget::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(5);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMouseSenseWidget::onActionMouseInOut);
}

int KNMouseSenseWidget::outBrightness() const
{
    return m_outBrightness;
}

void KNMouseSenseWidget::setOutBrightness(int outBrightness)
{
    m_outBrightness=outBrightness;
    m_mouseOut->setEndFrame(m_outBrightness);
}

int KNMouseSenseWidget::inBrightness() const
{
    return m_inBrightness;
}

void KNMouseSenseWidget::setInBrightness(int inBrightness)
{
    m_inBrightness=inBrightness;
    m_mouseIn->setEndFrame(m_inBrightness);
}

QColor KNMouseSenseWidget::backgroundColor() const
{
    return m_backgroundColor;
}

void KNMouseSenseWidget::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor.setHsv(backgroundColor.hue(),
                             backgroundColor.saturation(),
                             m_backgroundColor.value());
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}
