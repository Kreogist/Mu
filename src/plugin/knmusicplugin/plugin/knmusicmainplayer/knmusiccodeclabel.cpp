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

#include "knmusiccodeclabel.h"

KNMusicCodecLabel::KNMusicCodecLabel(QWidget *parent) :
    QWidget(parent),
    m_currentIndex(-1),
    m_imageSize(0)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //Load the format icon.
    m_formatIcons[IconAAC]=QPixmap(":/plugin/music/formats/format_aac.png");
    m_formatIcons[IconFLAC]=QPixmap(":/plugin/music/formats/format_flac.png");
    m_formatIcons[IconLame]=QPixmap(":/plugin/music/formats/format_lame.png");
    m_formatIcons[IconVorbis]=
            QPixmap(":/plugin/music/formats/format_vorbis.png");
    m_formatIcons[IconWav]=QPixmap(":/plugin/music/formats/format_wav.png");
    m_formatIcons[IconWma]=QPixmap(":/plugin/music/formats/format_wma.png");

    //Initial the suffix icon.
    m_suffixList[IconAAC] << "m4a" << "mp4" << "aac";
    m_suffixList[IconFLAC] << "flac";
    m_suffixList[IconLame] << "mp3" << "mp2";
    m_suffixList[IconVorbis] << "ogg";
    m_suffixList[IconWav] << "wav";
    m_suffixList[IconWma] << "wma";
}

QSize KNMusicCodecLabel::sizeHint() const
{
    return size();
}

void KNMusicCodecLabel::setSuffix(QString suffix)
{
    //Change the suffix to lower.
    suffix=suffix.toLower();
    //Reset the current index.
    m_currentIndex=-1;
    //Check out the suffix.
    for(int i=0; i<EncodeIconCount; ++i)
    {
        //Check the string list.
        if(m_suffixList[i].contains(suffix))
        {
            //Save the index.
            m_currentIndex=i;
            //Update the image.
            update();
            //Mission complete.
            return;
        }
    }
    //When we comes to here, means we cannot find suffix in the lists.
    //Update the image for null.
    update();
    //Mission complete.
    return;
}

void KNMusicCodecLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Check the current index.
    if(m_currentIndex==-1)
    {
        //Ignore invalid painting.
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the format icons
    painter.drawPixmap((width()-m_imageSize)>>1,
                       (height()-m_imageSize)>>1,
                       m_formatIcons[m_currentIndex].scaled(
                           m_imageSize,
                           m_imageSize,
                           Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation));
}

void KNMusicCodecLabel::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Get the latest image size.
    m_imageSize=qMin(width(), height());
}

