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

#ifndef KNANIMATIONMENU_H
#define KNANIMATIONMENU_H

#include <QMenu>

class QPropertyAnimation;
/*!
 * \brief The KNAnimationMenu class provides a smooth animation menu for all
 * platform. It solves the animation shake bug under Mac OS X.
 */
class KNAnimationMenu : public QMenu
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAnimationMenu class with given parent.
     * \param parent The parent widget.
     */
    explicit KNAnimationMenu(QWidget *parent = 0);

    /*!
     * \brief Set the mouse down position. The animation start expand point will
     * be the mouse down position.
     * \param mouseDownPos The position mouse pressed down.
     */
    void setMouseDownPos(const QPoint &mouseDownPos);

    /*!
     * \brief This is a overload function.\n
     * You can construct a point via give x and y parameter.
     * \param x The x position of the point.
     * \param y The y position of the point.
     */
    inline void setMouseDownPos(const int &x, const int &y)
    {
        setMouseDownPos(QPoint(x, y));
    }

    /*!
     * \brief Set the separator color via QStyleSheet. Actually this is a UI
     * problem which I cannot solve in C++.
     * \param color The color of the seperator line.
     */
    void setSeparatorColor(const QColor &color);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QMenu::showEvent().
     */
    void showEvent(QShowEvent *event);

    /*!
     * \brief Reimplemented from QMenu::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

private:
    bool m_showContent;
    QPropertyAnimation *m_showAnime;
    QPoint m_mouseDownPos;
};

#endif // KNANIMATIONMENU_H
