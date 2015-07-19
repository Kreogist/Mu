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

#ifndef KNGLASSBUTTON_H
#define KNGLASSBUTTON_H

#include <QBitmap>

#include <QAbstractButton>

class QTimeLine;
class KNGlassButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit KNGlassButton(QWidget *parent = 0);

    int spacing() const;
    void setSpacing(int spacing);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Reimplement from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplement from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplement from QAbstractButton::focusInEvent().
     */
    void focusInEvent(QFocusEvent *event);

    /*!
     * \brief Reimplement from QAbstractButton::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event);

    /*!
     * \brief Reimplement from QAbstractButton::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplement from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionOpacityChanged(const int &opacity);

private:
    inline void startAnime(QTimeLine *timeLine);
    inline QTimeLine *generateTimeLine(const int &endFrame);
    Qt::Alignment m_contentAlign;
    int m_spacing, m_imageOpacity;
    QTimeLine *m_mouseIn, *m_mouseOut, *m_mouseDown, *m_mouseUp;
    QLinearGradient m_highLight, m_highLightMask;
    QPixmap m_maskImage;
    qreal m_highLightOpacity;
};

#endif // KNGLASSBUTTON_H
