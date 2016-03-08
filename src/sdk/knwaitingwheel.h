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

#ifndef KNWAITINGWHEEL_H
#define KNWAITINGWHEEL_H

#include <QWidget>

/*!
 * \brief The KNWaitingWheel class provides a smaller waiting wheel. It will be
 * fixed size as 18px x 18px. It's especially for small size waiting wheel.
 */
class KNWaitingWheel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNWaitingWheel widget.
     * \param parent The parent widget.
     */
    explicit KNWaitingWheel(QWidget *parent = 0);

public slots:
    /*!
     * \brief Start the tick animation.
     */
    void startTick();

    /*!
     * \brief Pause the tick animation.
     */
    void pauseTick();

    /*!
     * \brief Stop the tick animation.
     */
    void stopTick();

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void tick();

private:
    QPixmap m_clockFrames[8];
    QTimer *m_tickTimer;
    quint8 m_frameCounter;
};

#endif // KNWAITINGWHEEL_H
