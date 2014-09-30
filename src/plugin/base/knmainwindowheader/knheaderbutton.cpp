/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QTimeLine>
#include <QRadialGradient>
#include <QGraphicsOpacityEffect>
#include <QLabel>

#include "knheaderbutton.h"

#include <QDebug>

KNHeaderButton::KNHeaderButton(QWidget *parent) :
    KNAnimeCheckedButton(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Connect signal.
    connect(this, &KNHeaderButton::clicked,
            this, &KNHeaderButton::requireShowCategorySwitcher);

    //Set palette.
    configurePalette();

    //Inital widgets.
    initialIcon();
    initialText();
    initialTimeLine();
}

void KNHeaderButton::setIcon(const QPixmap &pixmap)
{
    m_icon->setPixmap(pixmap.scaled(m_iconSize,
                                    m_iconSize,
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}

void KNHeaderButton::setText(const QString &text)
{
    m_text->setText(text);
    updateWidth();
}

QPixmap KNHeaderButton::icon() const
{
    return *m_icon->pixmap();
}

QString KNHeaderButton::text() const
{
    return m_text->text();
}

void KNHeaderButton::restoreButtonState()
{
    //Unfreeze button.
    setChecked(false);
    //Using mouse out anime to restore the geometry.
    startMouseOutAnime();
}

void KNHeaderButton::onActionMouseAnimation(const int &frame)
{
    //Move icon label.
    qreal moved=(qreal)qAbs(frame-m_iconX);
    m_icon->move(frame, m_iconY);
    //Move text label.
    m_text->move(m_leftMargin, m_textY-moved/4);
    //Set text gradient.
    qreal alphaPrefer=moved/m_iconMoved;
    m_opacityColor.setAlphaF(alphaPrefer>1.0?1.0:alphaPrefer);
    m_textGradient.setColorAt(1, m_opacityColor);
    m_textEffect->setOpacityMask(m_textGradient);
    m_text->setGraphicsEffect(m_textEffect);
}

void KNHeaderButton::onActionMouseDownUpAnimation(const int &frame)
{
    //Move icon label.
    m_icon->move(frame, m_iconY);
    //Move text label.
    m_text->move(m_leftMargin, m_textY+(frame-m_iconX)/2);
}

void KNHeaderButton::configurePalette()
{
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(0xff,0xff,0xff));
    setPalette(pal);
}

void KNHeaderButton::initialTimeLine()
{
    //Initial text anime(geometry).
    m_mouseIn=new QTimeLine(200, this);
    initialMouseInOutTimeLine(m_mouseIn);
    m_mouseIn->setEndFrame(m_iconMouseInX);

    m_mouseOut=new QTimeLine(200, this);
    initialMouseInOutTimeLine(m_mouseOut);
    m_mouseOut->setEndFrame(m_iconX);

    m_mouseDown=new QTimeLine(200, this);
    initialMouseDownTimeLine(m_mouseDown);
    m_mouseDown->setEndFrame(-m_iconSize);

    m_mouseUp=new QTimeLine(200, this);
    initialMouseUpTimeLine(m_mouseUp);
    m_mouseUp->setEndFrame(m_iconMouseUpX);
}

void KNHeaderButton::initialIcon()
{
    //Initial the icon displayer.
    m_icon=new QLabel(this);
    m_icon->setScaledContents(true);
    m_icon->setFixedSize(m_iconSize, m_iconSize);
    m_icon->move(m_iconX, m_iconY);
}

void KNHeaderButton::initialText()
{
    //Initial the text displayer.
    m_text=new QLabel(this);
    m_text->move(m_leftMargin, m_textY);
    //Configure the font.
    m_textFont=m_text->font();
    m_textFont.setBold(true);
    m_textFont.setPixelSize(m_textSize);
    m_text->setFont(m_textFont);
    //Update font metrics data.
    m_rawFontMetrics=m_text->fontMetrics();
    //Initial the graphic effects.
    m_textEffect=new QGraphicsOpacityEffect(m_text);
    m_textEffect->setOpacity(0.7);
    m_textGradient=QLinearGradient(QPointF(0,0),
                                   QPointF(0,50));
    m_textGradient.setColorAt(0, QColor(0,0,0,m_textAlpha));
    m_textGradient.setColorAt(1, m_opacityColor);
    m_textEffect->setOpacityMask(m_textGradient);
    m_text->setGraphicsEffect(m_textEffect);
}


void KNHeaderButton::initialMouseInOutTimeLine(QTimeLine *anime)
{
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setUpdateInterval(5);
    connect(anime, &QTimeLine::frameChanged,
            this, &KNHeaderButton::onActionMouseAnimation);
}

void KNHeaderButton::initialMouseDownTimeLine(QTimeLine *anime)
{
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setUpdateInterval(5);
    connect(anime, &QTimeLine::frameChanged,
            this, &KNHeaderButton::onActionMouseDownUpAnimation);
}

void KNHeaderButton::initialMouseUpTimeLine(QTimeLine *anime)
{
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setUpdateInterval(5);
    connect(anime, &QTimeLine::frameChanged,
            this, &KNHeaderButton::onActionMouseDownUpAnimation);
}

void KNHeaderButton::startMouseInAnime()
{
    //Stop all the animation.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set current position
    m_mouseIn->setStartFrame(m_icon->x());
    //Start mouse in animation.
    m_mouseIn->start();
}

void KNHeaderButton::startMouseOutAnime()
{
    //Stop all the animation.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set current position
    m_mouseOut->setStartFrame(m_icon->x());
    //Start mouse in animation.
    m_mouseOut->start();
}

void KNHeaderButton::startMouseDownAnime()
{
    //Stop up and down animations.
    m_mouseUp->stop();
    m_mouseDown->stop();
    //Set start parameter.
    m_mouseDown->setStartFrame(m_icon->x());
    //Start animation.
    m_mouseDown->start();
}

void KNHeaderButton::startMouseUpAnime()
{
    //Stop up and down animations.
    m_mouseDown->stop();
    m_mouseUp->stop();
    //Set start parameter.
    m_mouseUp->setStartFrame(m_icon->x());
    //Start animation.
    m_mouseUp->start();
}

void KNHeaderButton::updateWidth()
{
    int sizeParam=qMax(m_rawFontMetrics.width(m_text->text())+m_leftMargin,
                       110);
    resize(sizeParam, height());
    emit requireSetLeftSpacing(sizeParam);
}
