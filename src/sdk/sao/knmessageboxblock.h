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

#ifndef KNMESSAGEBOXBLOCK_H
#define KNMESSAGEBOXBLOCK_H

#include <QLabel>

/*!
 * \brief The KNMessageBoxBlock class is the top and the bottom part of a SAO
 * style message box.
 */
class KNMessageBoxBlock : public QLabel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMessageBoxBlock widget with given message box.
     * \param parent The parent widget.
     */
    explicit KNMessageBoxBlock(QWidget *parent = 0);

    /*!
     * \brief Set the gradient color of the block.
     * \param topColor The top color of the gradient.
     * \param bottomColor The bottom color of the gradient.
     */
    void setGradientColor(const QColor &topColor, const QColor &bottomColor);

    /*!
     * \brief Get the prefer width of the block.
     * \return The width which the block prefer.
     */
    int widthHint();

signals:

public slots:

protected:
    /*!
     * \brief Reimplement from QLabel::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private:
    inline void updateSizeParameters();
    inline void updatePalette();
    QLinearGradient m_backgroundGradient;
};

#endif // KNMESSAGEBOXBLOCK_H
