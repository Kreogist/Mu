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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "kndpimanager.h"

#include <QDebug>

KNDpiManager *KNDpiManager::m_instance=nullptr;

KNDpiManager *KNDpiManager::instance()
{
    return m_instance;
}

void KNDpiManager::initial(QObject *parent)
{
    //Set the current instance as the singleton instance.
    if(m_instance==nullptr)
    {
        m_instance=new KNDpiManager(parent);
    }
}

void KNDpiManager::setDpi(int dpiX, int dpiY)
{
#ifdef Q_OS_MACX
    Q_UNUSED(dpiX)
    Q_UNUSED(dpiY)
    //Use the 1.0 as the default scaler. Nothing need to be changed.
    //For Mac OS X, I have no idea what is happened, but it seems that it could
    //automatically support the Retina display.
    m_xScaler=1.0;
    m_yScaler=1.0;
    return;
#else
    //For other OS, it needs scaling. The DPI should be several times of 72.
    //When you changing the scaling on Windows, like 150%, 200%, you actually
    //change this settings.
    m_xScaler=(qreal)dpiX/96.0;
    m_yScaler=(qreal)dpiY/96.0;
#endif
}

QSize KNDpiManager::size(int width, int height)
{
    //Scale the width and height.
    return QSize(scaleX(width), scaleY(height));
}

QPoint KNDpiManager::pos(int x, int y)
{
    //Scaled the position.
    return QPoint(scaleX(x), scaleY(y));
}

QMargins KNDpiManager::margins(int left, int top, int right, int bottom)
{
    //Scale the margins.
    return QMargins(scaleX(left), scaleY(top), scaleX(right), scaleY(bottom));
}

QPointF KNDpiManager::posF(qreal x, qreal y)
{
    //Scale the position.
    return QPointF(scaleXF(x), scaleYF(y));
}

QRect KNDpiManager::rect(int x, int y, int width, int height)
{
    return QRect(scaleX(x), scaleY(y), scaleX(width), scaleY(height));
}

QRectF KNDpiManager::rectF(qreal x, qreal y, qreal width, qreal height)
{
    return QRectF(scaleXF(x), scaleYF(y), scaleXF(width), scaleYF(height));
}

qreal KNDpiManager::heightF(qreal rawHeight)
{
    return scaleYF(rawHeight);
}

qreal KNDpiManager::widthF(qreal rawWidth)
{
    return scaleXF(rawWidth);
}

int KNDpiManager::height(int rawHeight)
{
    return scaleY(rawHeight);
}

int KNDpiManager::width(int rawWidth)
{
    return scaleX(rawWidth);
}

KNDpiManager::KNDpiManager(QObject *parent) :
    QObject(parent),
    m_xScaler(1.0),
    m_yScaler(1.0)
{
}
