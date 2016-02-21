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

#ifndef KNSAOBUTTON_H
#define KNSAOBUTTON_H

#include <QAbstractButton>

class QTimeLine;
class KNSaoButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNSaoButton widget with given parent.
     * \param parent The parent widget.
     */
    explicit KNSaoButton(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractButton::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionMouseAnime(const int &frame);

private:
    inline void startAnime(const int &endFrame);
    qreal m_brightness;
    QTimeLine *m_mouseAnime;
};

#endif // KNSAOBUTTON_H
