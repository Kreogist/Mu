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

#ifndef KNROUNDSWITCHBUTTON_H
#define KNROUNDSWITCHBUTTON_H

#include <QPen>

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNRoundSwitchButton class is a switch style widget, however, it
 * could be called as a button. Because the widget is work the same as a check
 * box. It only just in a switch style.
 */
class KNRoundSwitchButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNRoundSwitchButton widget.
     * \param parent The parent widget.
     */
    explicit KNRoundSwitchButton(QWidget *parent = 0);

    /*!
     * \brief Set the button is checked or not without the animation.
     * \param checked The checked state.
     */
    void setForceChecked(bool checked);

signals:
    /*!
     * \brief When the moving animation is complete, this signal will be
     * emitted.
     */
    void animeComplete();

public slots:
    /*!
     * \brief Update the painting palette.
     * \param pal The palette of the round switch button.
     */
    void updatePalette(const QPalette &pal);

protected:
    /*!
     * \brief Reimplemented from KNRoundSwitchButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onFrameChangeMove(int frame);
    void startAnime();

private:
    QPainterPath m_backgroundRect;
    QLinearGradient m_backgroundBrush, m_buttonBrush;
    QPen m_buttonBorder;
    QColor m_buttonFillColor;
    QTimeLine *m_movingAnime;
    int m_buttonX;
};

#endif // KNROUNDSWITCHBUTTON_H
