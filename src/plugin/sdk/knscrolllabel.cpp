/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QTimer>
#include <QPainter>
#include <QSizePolicy>

#include "kngraphicsgloweffect.h"

#include "knscrolllabel.h"

KNScrollLabel::KNScrollLabel(QWidget *parent) :
    QWidget(parent)
{
    //Set default font.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::Minimum,
                              QSizePolicy::Label));
    setScrollFont(font());

    //Initial animation timer.
    m_movingAnime=new QTimer(this);
    m_movingAnime->setInterval(50);
    connect(m_movingAnime, &QTimer::timeout,
            this, &KNScrollLabel::onActionMoving);

    //Initial waiting timer.
    m_waiting=new QTimer(this);
    m_waiting->setInterval(5000);
    m_waiting->setSingleShot(true);
    connect(m_waiting, &QTimer::timeout,
            this, &KNScrollLabel::onActionWatingFinished);

    //Initial glow effect
    m_glowEffect=new KNGraphicsGlowEffect(this);
    m_glowEffect->setGlowColor(QColor(0,0,0));
    m_glowEffect->setRadius(m_glowRadius);
    setGraphicsEffect(m_glowEffect);
}

QSize KNScrollLabel::sizeHint() const
{
    return QSize(QWidget::sizeHint().width(),
                 m_scrollFontMetrics.height()+m_glowRadius);
}

QString KNScrollLabel::text() const
{
    return m_text;
}

void KNScrollLabel::setText(const QString &text)
{
    //Stop timers before.
    stopAllTimer();
    //Reset moving direction and position parameters.
    m_movingLeft=true;
    m_textX=m_glowRadius/4;
    //Reset the Timer.
    m_waiting->setInterval(5000);
    //Set datas.
    m_text=text;
    //Update animation parameters.
    updateAnimeParameters();
    //Update viewport.
    update();
}

void KNScrollLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    //Initial the text
    QPainter painter(this);
    //Set render hints.
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Prepare the font.
    painter.setFont(m_scrollFont);
    //Set opacity.
    painter.setOpacity(m_opacity);
    //Set contents margins.
    painter.translate(contentsMargins().left(),
                      contentsMargins().top());
    //Draw text.
    painter.setPen(palette().color(QPalette::Text));
    painter.drawText(m_textX,
                     m_glowRadius/4,
                     m_scrollFontMetrics.width(m_text)+m_glowRadius,
                     height(),
                     Qt::AlignLeft,
                     m_text);
}

void KNScrollLabel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Update the animations.
    updateAnimeParameters();
    //Check is it still need to scroll, here's a hack:
    //If the left most is 0, means no need to move.
    if(m_leftMostX==0)
    {
        //Just stop all timer and repaint.
        stopAllTimer();
        m_textX=m_glowRadius/4;
        update();
    }
}

void KNScrollLabel::onActionMoving()
{
    //If moving left,
    if(m_movingLeft)
    {
        //Check is it at the most left, if sure.
        if(m_textX<=m_leftMostX)
        {
            //Change direction.
            m_movingLeft=false;
            //Stop moving, start short waiting.
            stopAllTimer();
            m_waiting->setInterval(2500);
            m_waiting->start();
            return;
        }
        //Move left.
        m_textX--;
        update();
        return;
    }
    //Moving right, Check is at most right or not, if sure.
    if(m_textX>=m_glowRadius/4)
    {
        //Change direction,
        m_movingLeft=true;
        //Stop moving, start long waitng.
        stopAllTimer();
        m_waiting->setInterval(5000);
        m_waiting->start();
        return;
    }
    //Move right.
    m_textX++;
    update();
}

void KNScrollLabel::onActionWatingFinished()
{
    //Stop waiting and start moving.
    m_waiting->stop();
    m_movingAnime->start();
}

void KNScrollLabel::stopAllTimer()
{
    m_waiting->stop();
    m_movingAnime->stop();
}

void KNScrollLabel::updateAnimeParameters()
{
    //Get the text width
    int textWidth=m_scrollFontMetrics.width(m_text)+m_glowRadius;
    //If text is too long, enabled the scrolling.
    if(textWidth>contentsRect().width())
    {
        //Stop the animation timer.
        stopAllTimer();
        //Set tooltip.
        setToolTip(m_text);
        //Calculate most left X. This can control the right spacing.
        m_leftMostX=contentsRect().width()-textWidth;
        //Start waiting.
        m_waiting->start();
        return;
    }
    //Clear the tooltip, and reset text position.
    m_leftMostX=0;
    setToolTip("");
}

QFont KNScrollLabel::scrollFont() const
{
    return m_scrollFont;
}

void KNScrollLabel::setScrollFont(const QFont &scrollFont)
{
    //Update scroll font.
    m_scrollFont=scrollFont;
    //Update metrics.
    m_scrollFontMetrics=QFontMetrics(m_scrollFont);
    //Update fixed size.
    setFixedHeight(m_scrollFontMetrics.height()+m_glowRadius/2);
}

qreal KNScrollLabel::opacity() const
{
    return m_opacity;
}

void KNScrollLabel::setOpacity(const qreal &opacity)
{
    m_opacity=opacity;
    update();
}

