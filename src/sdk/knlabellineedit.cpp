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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>
#include <QKeyEvent>
#include <QTimeLine>

#include "knthememanager.h"

#include "knlabellineedit.h"

//#include <QDebug>

#define LabelBoxHeight 20
#define LabelBoxIconX 3
#define LabelBoxRoundedRect 4

#define MaximumLightness 0xFF

KNLabelLineEdit::KNLabelLineEdit(QWidget *parent) :
    QLineEdit(parent),
    m_labelIcon(QPixmap()),
    m_baseColor(QColor(0,0,0,0)),
    m_mouseInOut(generateTimeLine()),
    m_focusInOut(generateTimeLine()),
    m_focusSource(nullptr),
    m_minimumLightness(0x3A),
    m_mediumLightness(0x60)
{
    setObjectName("LabelLineEdit");
    //Set properties.
    setFrame(false);
    setContentsMargins(LabelBoxIconX,
                       0,
                       LabelBoxHeight>>1,
                       0);
    setFixedHeight(LabelBoxHeight);

    //Configure the time line, link the frame change with the slot.
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KNLabelLineEdit::onActionMouseInOut);
    connect(m_focusInOut, &QTimeLine::frameChanged,
            this, &KNLabelLineEdit::onActionFocusInOut);

    //Link with the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNLabelLineEdit::onActionThemeChanged);
    //Initial the palette.
    onActionThemeChanged();
    //Update the palette.
    onActionFocusInOut(m_minimumLightness);
}

QPixmap KNLabelLineEdit::labelIcon() const
{
    return m_labelIcon;
}

void KNLabelLineEdit::setLabelIcon(const QPixmap &labelIcon)
{
    //Check the validation of the label icon.
    if(labelIcon.isNull())
    {
        //Reset the label icon.
        m_labelIcon=QPixmap();
        //Reset the content margin.
        setContentsMargins(LabelBoxIconX,
                           0,
                           LabelBoxHeight>>1,
                           0);
        //Mission complete.
        return;
    }
    //Save the scaled icon.
    m_labelIcon = labelIcon.scaled(LabelBoxHeight,
                                   LabelBoxHeight,
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
    //Update the content margin.
    setContentsMargins(LabelBoxHeight+LabelBoxIconX,
                       0,
                       LabelBoxHeight>>1,
                       0);
}

QWidget *KNLabelLineEdit::focusSource() const
{
    return m_focusSource;
}

void KNLabelLineEdit::setFocusSource(QWidget *focusSource)
{
    m_focusSource = focusSource;
}

void KNLabelLineEdit::updateObjectName(const QString &itemName)
{
    //Change the object name.
    setObjectName(itemName);
    //Update the palette.
    onActionThemeChanged();
    //Update the lightness.
    onActionFocusInOut(m_baseColor.value());
}

void KNLabelLineEdit::setMinimumLightness(int minimumLightness)
{
    //Set the minimum lightness.
    m_minimumLightness=minimumLightness;
    //Check out the base color value.
    if(m_baseColor.value()<m_minimumLightness)
    {
        //Update the value of base color.
        m_baseColor.setHsv(m_baseColor.hue(),
                           m_baseColor.saturation(),
                           m_minimumLightness);
        //Update the lightness.
        onActionFocusInOut(m_minimumLightness);
    }
}

void KNLabelLineEdit::enterEvent(QEvent *event)
{
    //Do the original enter event.
    QLineEdit::enterEvent(event);
    //If the text content gets the focus, ignore the mouse in.
    if(hasFocus() || !isEnabled())
    {
        return;
    }
    //Start the mouse in anime.
    startAnime(m_mouseInOut, m_mediumLightness);
}

void KNLabelLineEdit::leaveEvent(QEvent *event)
{
    //Do the original leave event.
    QLineEdit::leaveEvent(event);
    //If the text content gets the focus, ignore the mouse out.
    if(hasFocus() || !isEnabled())
    {
        return;
    }
    //Start the mouse out anime.
    startAnime(m_mouseInOut, m_minimumLightness);
}

void KNLabelLineEdit::focusInEvent(QFocusEvent *event)
{
    //Check if the text content is enabled
    if(isEnabled())
    {
        //Start the focus in anime.
        startAnime(m_focusInOut, MaximumLightness);
    }
    //Do the original focus in event.
    QLineEdit::focusInEvent(event);
}

void KNLabelLineEdit::focusOutEvent(QFocusEvent *event)
{
    //Check if the text content is enabled
    if(isEnabled())
    {
        //Start the focus out anime.
        startAnime(m_focusInOut, m_minimumLightness);
    }
    //Do the original focus in event.
    QLineEdit::focusOutEvent(event);
}

void KNLabelLineEdit::paintEvent(QPaintEvent *event)
{
    //Draw the base first.
    //Initial the painter.
    QPainter painter(this);
    //Set the render hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Configure the painter.
    painter.setPen(QColor(0,0,0,m_baseColor.value()>>1));
    painter.setBrush(m_baseColor);
    //Draw the back content first.
    painter.drawRoundedRect(rect(),
                            LabelBoxRoundedRect,
                            LabelBoxRoundedRect);
    //Draw the search icon.
    if(!m_labelIcon.isNull())
    {
        painter.drawPixmap(LabelBoxIconX,
                           0,
                           LabelBoxHeight,
                           LabelBoxHeight,
                           m_labelIcon);
    }
    //Do the original paint event.
    QLineEdit::paintEvent(event);
}

void KNLabelLineEdit::keyPressEvent(QKeyEvent *event)
{
    //If pressed escape, means the user want to lose the focus.
    switch(event->key())
    {
    case Qt::Key_Escape:
        //Check the focus source widget.
        if(m_focusSource!=nullptr)
        {
            //Give the focus to the source widget.
            m_focusSource->setFocus(Qt::MouseFocusReason);
            //Clear the source widget pointer.
            m_focusSource=nullptr;
        }
        break;
    case Qt::Key_Tab:
        QLineEdit::keyPressEvent(event);
        break;
    default:
        //Or else, do the original one.
        QLineEdit::keyPressEvent(event);
        break;
    }
}

void KNLabelLineEdit::onActionThemeChanged()
{
    //Get the palette from the theme manager.
    QPalette pal=knTheme->getPalette(objectName());
    //Backup the base color.
    m_baseColor=pal.color(QPalette::Base);
    //Reset the base color.
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    //Set the palette.
    setPalette(pal);
}

void KNLabelLineEdit::onActionMouseInOut(const int &frame)
{
    //Use the frame as the new lightness of the base color.
    m_baseColor.setHsv(m_baseColor.hue(),
                       m_baseColor.saturation(),
                       frame);
    //Repaint the widget.
    update();
}

void KNLabelLineEdit::onActionFocusInOut(const int &frame)
{
    //Use the frame as the new lightness of the base color.
    m_baseColor.setHsv(m_baseColor.hue(),
                       m_baseColor.saturation(),
                       frame);
    //Get the palette.
    QPalette pal=palette();
    //Update the text color.
    QColor textColor=pal.color(QPalette::Text);
    textColor.setHsv(textColor.hue(),
                     textColor.saturation(),
                     0xFF-frame);
    //Set back the text color.
    pal.setColor(QPalette::Text, textColor);
    //Set back the palette.
    setPalette(pal);
}

inline QTimeLine *KNLabelLineEdit::generateTimeLine()
{
    //Generate the time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configure the time line.
    timeLine->setUpdateInterval(16);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    //Give back the time line.
    return timeLine;
}

inline void KNLabelLineEdit::startAnime(QTimeLine *timeLine, const int &end)
{
    //Stop all the animations.
    m_mouseInOut->stop();
    m_focusInOut->stop();
    //Set the time line.
    timeLine->setFrameRange(m_baseColor.value(), end);
    //Start the time line.
    timeLine->start();
}

void KNLabelLineEdit::setMediumLightness(int mediumLightness)
{
    //Set the medium lightness.
    m_mediumLightness = mediumLightness;
}
