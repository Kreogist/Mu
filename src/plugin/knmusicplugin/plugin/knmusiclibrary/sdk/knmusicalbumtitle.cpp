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
#include <QCursor>

#include "knmusicglobal.h"

#include "knmusicalbumtitle.h"

#include <QDebug>

KNMusicAlbumTitle::KNMusicAlbumTitle(QWidget *parent) :
    QWidget(parent),
    m_albumArt(QPixmap()),
    m_scaledAlbumArt(QPixmap()),
    m_mouseIn(false)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
}

void KNMusicAlbumTitle::enterEvent(QEvent *event)
{
    //Do original enter event.
    QWidget::enterEvent(event);
    //Set and ask to show the album art.
    setMouseIn();
}

void KNMusicAlbumTitle::leaveEvent(QEvent *event)
{
    //Do original leave event.
    QWidget::leaveEvent(event);
    //Release the flag.
    m_mouseIn=false;
    //Ask to hide the album art.
    emit requireHideAlbumArt();
}

void KNMusicAlbumTitle::mouseMoveEvent(QMouseEvent *event)
{
    //Do original move event.
    QWidget::mouseMoveEvent(event);
    //Set and ask to show the album art.
    setMouseIn();
}

void KNMusicAlbumTitle::mouseReleaseEvent(QMouseEvent *event)
{
    //Ignore the release event.
    Q_UNUSED(event)
    //Set and ask to show the album art.
    setMouseIn();
}

void KNMusicAlbumTitle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Set the painter.
    QPainter painter(this);
    //Set the rendering hint.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Fill up the background.
    painter.fillRect(rect(), QColor(0,0,0));
    //Check the validation of the album art data.
    if(!m_scaledAlbumArt.isNull())
    {
        //Draw the content.
        painter.drawPixmap((width()-m_scaledAlbumArt.width())>>1,
                           (height()-m_scaledAlbumArt.height())>>1,
                           m_scaledAlbumArt);
    }
}

void KNMusicAlbumTitle::resizeEvent(QResizeEvent *event)
{
    //Resize the album title widget.
    QWidget::resizeEvent(event);
    //Update the scaled image.
    updateScaledAlbumArt();
}

inline void KNMusicAlbumTitle::updateScaledAlbumArt()
{
    //Check out the album art.
    if(m_albumArt.isNull())
    {
        //Clear up the scaled image.
        m_scaledAlbumArt=QPixmap();
        //Finished.
        return;
    }
    //Scaled the image.
    m_scaledAlbumArt=
            size().isEmpty()?
                m_albumArt:
                m_albumArt.scaled(size(),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation);
    //We should update the widget.
    update();
}

QPixmap KNMusicAlbumTitle::albumArt() const
{
    return m_albumArt;
}

void KNMusicAlbumTitle::setAlbumArt(const QPixmap &albumArt)
{
    //Save the album art data.
    m_albumArt = albumArt.isNull()?knMusicGlobal->noAlbumArt():albumArt;
    //Update the scaled image.
    updateScaledAlbumArt();
}

void KNMusicAlbumTitle::clearAlbumArt()
{
    //Clear the album art.
    m_albumArt=QPixmap();
    //Update the scaled image.
    updateScaledAlbumArt();
}

bool KNMusicAlbumTitle::checkMousePosition()
{
    //Check the mouse position.
    return rect().contains(mapFromGlobal(QCursor::pos()));
}
