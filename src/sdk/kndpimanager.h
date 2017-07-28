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

#ifndef KNDPIMANAGER_H
#define KNDPIMANAGER_H

#include <QSize>
#include <QPoint>
#include <QRectF>
#include <QMargins>

#include <QObject>

#define knDpi (KNDpiManager::instance())

/*!
 * \brief The KNDpiManager class provides the ability to scale the position and
 * size of the GUI component.
 */
class KNDpiManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the singleton instance of the DPI manager. Before the
     * initial() has been called, this function will return nullptr.
     * \return The instance of configure manager.
     */
    static KNDpiManager *instance();

    /*!
     * \brief Initial the manager, generate the instance with the given parent
     * object.\n
     * Only the first time will create a instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Set the application window horizontal and vertical DPI. These
     * information should be provided from the main window.
     * \param dpiX The horizontal DPI.
     * \param dpiY The vertical DPI.
     */
    void setDpi(int dpiX, int dpiY);

    /*!
     * \brief Get the scale of the size.
     * \param width The original width.
     * \param height The original height.
     * \return The scaled width and height.
     */
    QSize size(int width, int height);

    /*!
     * \brief Get the scaled position of the point.
     * \param x The X position.
     * \param y The Y position.
     * \return The scaled position.
     */
    QPoint pos(int x, int y);

    /*!
     * \brief Get the scaled margins.
     * \param left The left margin.
     * \param top The top margin.
     * \param right The right margin.
     * \param bottom The bottom margin.
     * \return The scaled margin.
     */
    QMargins margins(int left, int top, int right, int bottom);

    /*!
     * \brief Get the scaled position of the floating point.
     * \param x The X position.
     * \param y The Y position.
     * \return The scaled position.
     */
    QPointF posF(qreal x, qreal y);

    /*!
     * \brief Get the scaled rect.
     * \param x The X pos of the rect.
     * \param y The Y pos of the rect.
     * \param width The width of the rect.
     * \param height The height of the rect.
     * \return The result of the scaled rectangle.
     */
    QRect rect(int x, int y, int width, int height);

    /*!
     * \brief Get the scaled float rect.
     * \param x The X pos of the rect.
     * \param y The Y pos of the rect.
     * \param width The width of the rect.
     * \param height The height of the rect.
     * \return The result of the scaled rectangle.
     */
    QRectF rectF(qreal x, qreal y, qreal width, qreal height);

    /*!
     * \brief This is an override function.\n
     * Get the scaled position of the floating point.
     * \param rawPos The original point.
     * \return The scaled position.
     */
    QPointF posF(const QPointF rawPos)
    {
        return posF(rawPos.x(), rawPos.y());
    }

    /*!
     * \brief Get the scaled height, this is the float point version.
     * \param rawHeight The original height.
     * \return The scaled height.
     */
    qreal heightF(qreal rawHeight);

    /*!
     * \brief Get the scaled width, this is the float point version.
     * \param rawWidth The original width.
     * \return The scaled width.
     */
    qreal widthF(qreal rawWidth);

    /*!
     * \brief Get the scaled height.
     * \param rawHeight The original height.
     * \return The scaled height.
     */
    int height(int rawHeight);

    /*!
     * \brief Get the scaled width.
     * \param rawWidth The original width.
     * \return The scaled width.
     */
    int width(int rawWidth);

signals:

public slots:

private:
    explicit KNDpiManager(QObject *parent = 0);
    KNDpiManager(const KNDpiManager &);
    KNDpiManager(KNDpiManager &&);
    inline int scaleX(int rawWidth)
    {
        return (int)scaleXF((qreal)rawWidth);
    }

    inline qreal scaleXF(qreal rawWidth)
    {
        return m_xScaler*rawWidth;
    }

    inline int scaleY(int rawHeight)
    {
        return (int)scaleYF((qreal)rawHeight);
    }

    inline qreal scaleYF(qreal rawHeight)
    {
        return m_yScaler*rawHeight;
    }

    static KNDpiManager *m_instance;

    qreal m_xScaler, m_yScaler;
};

#endif // KNDPIMANAGER_H
