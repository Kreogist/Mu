/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmessageblock.h"

KNMessageBlock::KNMessageBlock(QWidget *parent) :
    QLabel(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setAlignment(Qt::AlignCenter);

    //Update palette.
    m_palette=palette();
    m_palette.setColor(QPalette::WindowText, QColor(0,0,0,104));
    setPalette(m_palette);

    //Update the gradient.
    m_backgroundGradient.setStart(0,0);
    updateSizeParam();
    updateBackground();
}

void KNMessageBlock::setTopColor(const QColor &topColor)
{
    m_backgroundGradient.setColorAt(0, topColor);
    updateBackground();
}

void KNMessageBlock::setButtomColor(const QColor &bottomColor)
{
    m_backgroundGradient.setColorAt(1, bottomColor);
    updateBackground();
}

int KNMessageBlock::preferWidth()
{
    return qMax(332, fontMetrics().width(text())+150);
}

void KNMessageBlock::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    //Update the size parameters.
    updateSizeParam();
    updateBackground();
}

void KNMessageBlock::updateSizeParam()
{
    m_backgroundGradient.setFinalStop(0,height());
    QFont textFont=font();
    int preferSize=(height()>>1)-9;
    textFont.setPixelSize(preferSize<2?2:preferSize);
    setFont(textFont);
}

void KNMessageBlock::updateBackground()
{
    m_palette.setBrush(QPalette::Window, m_backgroundGradient);
    setPalette(m_palette);
}
