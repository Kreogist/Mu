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

#ifndef KNCIRCLEICONBUTTON_H
#define KNCIRCLEICONBUTTON_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNCircleIconButton class provides you a circle outside the icon.\n
 * This button is mostly used in SAO styled element.
 */
class KNCircleIconButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNCircleIconButton widget.
     * \param parent The parent widget.
     */
    explicit KNCircleIconButton(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionToggleChanged(const int &frame);
    void onActionToggled(bool checked);

private:
    static int m_iconPosition;

    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void startAnime(QTimeLine *timeLine);
    QTimeLine *m_selected, *m_unselected;
    int m_lightness;
};

#endif // KNCIRCLEICONBUTTON_H
