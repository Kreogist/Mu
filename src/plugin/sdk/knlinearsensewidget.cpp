/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QTimeLine>

#include "knlinearsensewidget.h"

KNLinearSenseWidget::KNLinearSenseWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Initial the gradient background color and mouse timeline.
    m_background=QLinearGradient(QPoint(0,0), QPoint(0, height()));
    m_background.setColorAt(0, m_linearColor);
    m_background.setColorAt(1, m_backgroundColor);
    m_palette=palette();
    m_palette.setBrush(QPalette::Window, QBrush(m_background));
    setPalette(m_palette);

    m_mouseIn=new QTimeLine(200, this);
    configureTimeLine(m_mouseIn);
    m_mouseIn->setEndFrame(m_inBrightness);

    m_mouseOut=new QTimeLine(200, this);
    configureTimeLine(m_mouseOut);
    m_mouseOut->setEndFrame(m_outBrightness);
}

void KNLinearSenseWidget::enterEvent(QEvent *event)
{
    //Stop timeline.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseIn->setStartFrame(m_linearColor.value());
    m_mouseIn->start();
    QWidget::enterEvent(event);
}

void KNLinearSenseWidget::leaveEvent(QEvent *event)
{
    //Stop timeline.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_mouseOut->setStartFrame(m_linearColor.value());
    m_mouseOut->start();
    QWidget::leaveEvent(event);
}

void KNLinearSenseWidget::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QWidget::resizeEvent(event);
    //Change the background.
    m_background.setFinalStop(QPoint(0,height()));
    m_palette.setBrush(QPalette::Window, m_background);
    setPalette(m_palette);
}

void KNLinearSenseWidget::onActionMouseInOut(const int &frame)
{
    //Update gradient color.
    m_linearColor.setHsv(m_backgroundColor.hue(),
                         m_backgroundColor.saturation(),
                         frame);
    //Set palette.
    m_background.setColorAt(0, m_linearColor);
    m_palette.setBrush(QPalette::Window, m_background);
    setPalette(m_palette);
}

void KNLinearSenseWidget::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(16);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNLinearSenseWidget::onActionMouseInOut);
}
