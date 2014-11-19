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
#include <QCryptographicHash>

#include "knhashpixmaplist.h"

KNHashPixmapList::KNHashPixmapList(QObject *parent) :
    QObject(parent)
{
}

QString KNHashPixmapList::appendImage(const QImage &image)
{
    //Calculate the hash data of the image.
    QByteArray hashResult=QCryptographicHash::hash(QByteArray((char *)image.bits(), image.byteCount()),
                                                   QCryptographicHash::Md5);
    //Get the image key.
    QString imageKey;
    for(int i=0; i<hashResult.size(); i++)
    {
        imageKey+=hashResult.at(i)<16?
                    "0"+QString::number((quint8)hashResult.at(i), 16):
                    QString::number((quint8)hashResult.at(i), 16);
    }
    //Save the image.
    m_imageList.insert(imageKey, image);
    //Return the image key.
    return imageKey;
}
