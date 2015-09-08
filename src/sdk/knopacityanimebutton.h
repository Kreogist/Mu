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
/*!
 * \brief The KNOpacityAnimeButton class is a button which will be sensitive to
 * the following mouse events: mouse enter, leave, pressed and release. Even
 * focus in and out can changed the opacity. It will ignore the text, but only
 * paint the icon.
 */
class KNOpacityAnimeButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNOpacityAnimeButton widget.
     * \param parent The parent widget.
     */
    explicit KNOpacityAnimeButton(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent(). the button should
     * provide the paint event.
     */
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Get the current image opacity for painting.
     * \return The opacity, range in 0.0 to 1.0.
     */
    qreal imageOpacity();

    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::focusInEvent().
     */
    void focusInEvent(QFocusEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionOpacityChanged(const int &opacity);

private:
    inline void startAnime(const int &endFrame);
    QTimeLine *m_mouseAnime;
    qreal m_imageOpacity;
};

#endif // KNOPACITYANIMEBUTTON_H
