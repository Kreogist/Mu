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

#ifndef KNCLOCKWHEEL_H
#define KNCLOCKWHEEL_H

#include <QIcon>

#include <QWidget>

class KNClockWheel : public QWidget
{
    Q_OBJECT
public:
    explicit KNClockWheel(QWidget *parent = 0);

signals:

public slots:
    void startTick();
    void pauseTick();
    void stopTick();

protected:
    /*!
     * \brief resizeEvent
     * \param event
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief paintEvent
     * \param event
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void tick();

private:
    static QIcon m_clockFrames[24];
    QTimer *m_tickTimer;
    int m_imageX, m_imageY, m_imageSize;
    quint8 m_frameCounter;
};

#endif // KNCLOCKWHEEL_H
