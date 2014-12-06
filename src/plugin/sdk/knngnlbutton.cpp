/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>
#include <QTimeLine>
#include <QMouseEvent>

#include "knngnlbutton.h"

#include <QDebug>

KNNGNLButton::KNNGNLButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set the palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(90,90,90));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);

    //Initial the quotation path.
    m_leftQuotation.moveTo(17, 0);
    m_leftQuotation.lineTo(0, 0);
    m_leftQuotation.lineTo(0, 17);
    m_rightQuotation.moveTo(0, 17);
    m_rightQuotation.lineTo(17, 17);
    m_rightQuotation.lineTo(17, 0);

    //Initial the animation time lines.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(0);
    configureInOutTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setStartFrame(0);
    configureInOutTimeLine(m_mouseOut);
}

QPixmap KNNGNLButton::icon() const
{
    return m_icon;
}

void KNNGNLButton::setIcon(const QPixmap &icon)
{
    m_icon = icon;
    //Update the image.
    updateScaledImage();
}

QString KNNGNLButton::text() const
{
    return m_text;
}

void KNNGNLButton::setText(const QString &text)
{
    m_text = text;
    //Update the button.
    update();
}

QSize KNNGNLButton::iconSize() const
{
    return m_iconSize;
}

void KNNGNLButton::setIconSize(const QSize &iconSize)
{
    m_iconSize=iconSize;
    //Update the image.
    updateScaledImage();
}

void KNNGNLButton::enterEvent(QEvent *event)
{
    //Check if we need to launch the time line.
    if(m_mouseIn->state()==QTimeLine::NotRunning)
    {
        stopAllAnimations();
        //Set initial data for mouse in.
        m_halfWidth=width()>>1;
        m_leftQuotePosition=m_halfWidth-m_quoteWidth;
        //Set the parameters.
        m_mouseIn->setStartFrame(m_leftQuotePosition);
        //Start mouse in animation.
        m_mouseIn->start();
    }
    QWidget::enterEvent(event);
}

void KNNGNLButton::leaveEvent(QEvent *event)
{
    if(m_mouseOut->state()==QTimeLine::NotRunning)
    {
        stopAllAnimations();
        //Set initial data for mouse out.
        m_halfWidth=width()>>1;
        m_leftQuotePosition=0;
        //Set the parameters.
        m_mouseOut->setEndFrame(m_halfWidth);
        //Start mouse in animation.
        m_mouseOut->start();
    }
    QWidget::leaveEvent(event);
}

void KNNGNLButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the antialiasing painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Get the palette.
    QPalette pal=palette();
    //Draw the background first.
    painter.setOpacity(m_quoteOpacity);
    //Set the painter.
    painter.setPen(Qt::NoPen);
    painter.setBrush(pal.background());
    //Draw the background.
    painter.drawRect(rect());
    //Restore the opacity.
    painter.setOpacity(1.0);

    //Draw the image first.
    QRect imageRect=QRect(QPoint(m_leftSpacing,
                                 (height()-m_iconSize.height())>>1),
                          m_iconSize);
    painter.drawPixmap(imageRect, m_scaledIcon);

    //Draw the text.
    QPen textPen=QPen(pal.color(QPalette::WindowText));
    painter.setPen(textPen);
    painter.drawText(imageRect.right()+m_spacing,
                     0,
                     width(),
                     height(),
                     Qt::AlignVCenter,
                     m_text);

    //Draw the quotation using with opacity, using text color.
    painter.setOpacity(m_quoteOpacity);
    textPen.setWidth(3);
    painter.setPen(textPen);
    //Draw left quotation.
    painter.save();
    painter.translate(m_leftQuotePosition, 0);
    painter.drawPath(m_leftQuotation);
    //Draw right quotation.
    painter.restore();
    painter.save();
    painter.translate(width()-m_leftQuotePosition-17, height()-17);
    painter.drawPath(m_rightQuotation);
    //Draw the high light text.
    painter.restore();
    textPen.setColor(pal.color(QPalette::HighlightedText));
    painter.setPen(textPen);
    painter.drawText(imageRect.right()+m_spacing,
                     0,
                     width(),
                     height(),
                     Qt::AlignVCenter,
                     m_text);
}

void KNNGNLButton::mousePressEvent(QMouseEvent *event)
{
    KNAbstractButton::mousePressEvent(event);
    //Set pressed flag.
    m_pressed=true;
}

void KNNGNLButton::mouseReleaseEvent(QMouseEvent *event)
{
    KNAbstractButton::mouseReleaseEvent(event);
    //Ensure this button has been mouse down.
    if(m_pressed &&
            event->button()==Qt::LeftButton &&
            rect().contains(event->pos()))
    {
        //Emit the clicked signals.
        emit clicked();
    }
    //Release flag.
    m_pressed=false;
}

void KNNGNLButton::onActionMouseInOut(const int &frame)
{
    //Save the frame as left quote position.
    m_leftQuotePosition=frame;
    //Update the opacity of the button.
    m_quoteOpacity=1.0-(qreal)m_leftQuotePosition/m_halfWidth;
    //Ask to redraw the button.
    update();
}

void KNNGNLButton::stopAllAnimations()
{
    m_mouseIn->stop();
    m_mouseOut->stop();
}

void KNNGNLButton::configureInOutTimeLine(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(5);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNNGNLButton::onActionMouseInOut);
}

void KNNGNLButton::updateScaledImage()
{
    //Update the scaled data.
    m_scaledIcon=m_icon.scaled(m_iconSize,
                               Qt::KeepAspectRatio,
                               Qt::SmoothTransformation);
    //Update the button.
    update();
}
