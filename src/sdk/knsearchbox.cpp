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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>
#include <QKeyEvent>
#include <QTimeLine>

#include "knthememanager.h"

#include "knsearchbox.h"

#include <QDebug>

#define SearchBoxHeight 20
#define SearchBoxIconX 3

#define MinimumLightness 0x3A
#define MediumLightness 0x60
#define MaximumLightness 0xFF

KNSearchBox::KNSearchBox(QWidget *parent) :
    QLineEdit(parent),
    m_searchIcon(QPixmap("://public/search.png")),
    m_baseColor(QColor(0,0,0,0)),
    m_mouseInOut(generateTimeLine()),
    m_focusInOut(generateTimeLine()),
    m_focusSource(nullptr)
{
    setObjectName("SearchBox");
    //Set properties.
    setFrame(false);
    setContentsMargins(SearchBoxHeight+SearchBoxIconX,
                       0,
                       SearchBoxHeight>>1,
                       0);
    setFixedHeight(SearchBoxHeight);

    //Configure the time line, link the frame change with the slot.
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionMouseInOut);
    connect(m_focusInOut, &QTimeLine::frameChanged,
            this, &KNSearchBox::onActionFocusInOut);

    //Link with the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNSearchBox::onActionThemeChanged);
    //Intial the palette.
    onActionThemeChanged();
    //Update the palette.
    onActionFocusInOut(MinimumLightness);
}

void KNSearchBox::enterEvent(QEvent *event)
{
    //Do the original enter event.
    QLineEdit::enterEvent(event);
    //If the text content gets the focus, ignore the mouse in.
    if(hasFocus())
    {
        return;
    }
    //Start the mouse in anime.
    startAnime(m_mouseInOut, MediumLightness);
}

void KNSearchBox::leaveEvent(QEvent *event)
{
    //Do the original leave event.
    QLineEdit::leaveEvent(event);
    //If the text content gets the focus, ignore the mouse out.
    if(hasFocus())
    {
        return;
    }
    //Start the mouse out anime.
    startAnime(m_mouseInOut, MinimumLightness);
}

void KNSearchBox::focusInEvent(QFocusEvent *event)
{
    //Start the focus in anime.
    startAnime(m_focusInOut, MaximumLightness);
    //Do the original focus in event.
    QLineEdit::focusInEvent(event);
}

void KNSearchBox::focusOutEvent(QFocusEvent *event)
{
    //Start the focus out anime.
    startAnime(m_focusInOut, MinimumLightness);
    //Do the original focus in event.
    QLineEdit::focusOutEvent(event);
}

void KNSearchBox::paintEvent(QPaintEvent *event)
{
    //Draw the base first.
    //Initial the painter.
    QPainter painter(this);
    //Set the render hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Configure the painter.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_baseColor);
    //Draw the back content first.
    painter.drawRoundedRect(rect(),
                            height()>>1,
                            height()>>1);
    //Draw the search icon.
    painter.drawPixmap(SearchBoxIconX,
                       0,
                       SearchBoxHeight,
                       SearchBoxHeight,
                       m_searchIcon);
    //Do the original paint event.
    QLineEdit::paintEvent(event);
}

void KNSearchBox::keyPressEvent(QKeyEvent *event)
{
    //If pressed escape, means the user want to lose the focus.
    if(event->key()==Qt::Key_Escape)
    {
        //Check the focus source widget.
        if(m_focusSource!=nullptr)
        {
            //Give the focus to the source widget.
            m_focusSource->setFocus(Qt::MouseFocusReason);
            //Clear the source widget pointer.
            m_focusSource=nullptr;
        }
        return;
    }
    //Or else, do the original one.
    QLineEdit::keyPressEvent(event);
}

void KNSearchBox::onActionThemeChanged()
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

void KNSearchBox::onActionMouseInOut(const int &frame)
{
    //Use the frame as the new lightness of the base color.
    m_baseColor.setHsv(m_baseColor.hue(),
                       m_baseColor.saturation(),
                       frame);
    //Repaint the widget.
    update();
}

void KNSearchBox::onActionFocusInOut(const int &frame)
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

inline QTimeLine *KNSearchBox::generateTimeLine()
{
    //Generate the time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configure the time line.
    timeLine->setUpdateInterval(10);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    //Give back the time line.
    return timeLine;
}

void KNSearchBox::startAnime(QTimeLine *timeLine, const int &end)
{
    //Stop all the animations.
    m_mouseInOut->stop();
    m_focusInOut->stop();
    //Set the time line.
    timeLine->setFrameRange(m_baseColor.value(), end);
    //Start the time line.
    timeLine->start();
}

QWidget *KNSearchBox::focusSource() const
{
    return m_focusSource;
}

void KNSearchBox::setFocusSource(QWidget *focusSource)
{
    m_focusSource = focusSource;
}

QPixmap KNSearchBox::searchIcon() const
{
    return m_searchIcon;
}

void KNSearchBox::setSearchIcon(const QPixmap &searchIcon)
{
    m_searchIcon = searchIcon.scaled(SearchBoxHeight,
                                     SearchBoxHeight,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
}
