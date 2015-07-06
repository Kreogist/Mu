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

#ifndef KNOPACITYANIMEBUTTON_H
#define KNOPACITYANIMEBUTTON_H

#include <QAbstractButton>

class QTimeLine;
class KNOpacityAnimeButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit KNOpacityAnimeButton(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplement from QAbstractButton::paintEvent(). the button should
     * provide the paint event.
     */
    void paintEvent(QPaintEvent *event);

    qreal imageOpacity();

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionOpacityChanged(const int &opacity);

private:
    inline void startAnime(QTimeLine *timeLine);
    inline QTimeLine *generateTimeLine(const int &endFrame);
    QTimeLine *m_mouseIn, *m_mouseOut, *m_mouseDown, *m_mouseUp;
    qreal m_imageOpacity;
};

#endif // KNOPACITYANIMEBUTTON_H
