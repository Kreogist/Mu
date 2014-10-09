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
#include <QMouseEvent>

#include "knanimecheckedbutton.h"

KNAnimeCheckedButton::KNAnimeCheckedButton(QWidget *parent) :
    KNAbstractButton(parent)
{
}

bool KNAnimeCheckedButton::checked() const
{
    return m_checked;
}

void KNAnimeCheckedButton::setChecked(bool checked)
{
    if(checked!=m_checked)
    {
        m_checked=checked;
        //If we change the state, so we need to change the UI.
        if(m_checked)
        {
            startMouseUpAnime();
        }
        else
        {
            startMouseOutAnime();
        }
        //Emit toggled signal.
        emit toggled(m_checked);
        onCheckedChanged();
    }
}

void KNAnimeCheckedButton::enterEvent(QEvent *event)
{
    //If we don't freeze the animation, process the animation.
    if(!m_checked)
    {
        //Do a check first.
        if(m_pressed)
        {
            //You have to pressed the button -> move out -> move back and then will
            //come to here.
            //We should show mouse pressed animation here.
            startMouseDownAnime();
            return;
        }
        //For normal, show mouse in animation.
        startMouseInAnime();
    }
    //Do enter event.
    QWidget::enterEvent(event);
}

void KNAnimeCheckedButton::leaveEvent(QEvent *event)
{
    //Do the freeze judge.
    if(!m_checked)
    {
        //Do a judge:
        if(m_pressed)
        {
            //Means the user might want to cancel the operation.
            //Set the state to mouse in(Do mouse in animation).
            startMouseInAnime();
            return;
        }
        startMouseOutAnime();
    }
    //Do enter event.
    QWidget::leaveEvent(event);
}

void KNAnimeCheckedButton::mousePressEvent(QMouseEvent *event)
{
    //Do the freeze judge.
    if(!m_checked)
    {
        if(event->button()==Qt::LeftButton)
        {
            m_pressed=true;
            startMouseDownAnime();
        }
        QWidget::mousePressEvent(event);
    }
}

void KNAnimeCheckedButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the freeze judge
    if(!m_checked)
    {
        //Check is button has been pressed before.
        if(m_pressed)
        {
            m_pressed=false;
            //If we find:
            // 1. Left button pressed.
            // 2. Release the mouse in the button.
            //Do the release anime, this button clicked.
            if(event->button()==Qt::LeftButton
                    && rect().contains(event->pos()))
            {
                //Show release animation.
                startMouseUpAnime();
                //Freeze button.
                setChecked(true);
                //Emit clicked signal.
                emit clicked();
            }
            else
            {
                //This button is not clicked. Return to normal.
                startMouseOutAnime();
            }
        }
        QWidget::mouseReleaseEvent(event);
    }
}

void KNAnimeCheckedButton::onCheckedChanged()
{
    //Do nothing.
    ;
}
