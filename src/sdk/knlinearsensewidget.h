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

#ifndef KNLINEARSENSEWIDGET_H
#define KNLINEARSENSEWIDGET_H

#include <QWidget>

class QTimeLine;
/*!
 * \brief The KNLinearSenseWidget class is the a mouse sensitive widget. It will
 * automatically high light the widget background when the mouse entered.
 */
class KNLinearSenseWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNLinearSenseWidget widget.
     * \param parent The parent widget.
     */
    explicit KNLinearSenseWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplement from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplement from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplement from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

    /*!
     * \brief Reimplement from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    inline QTimeLine *generateTimeLine(const int &endBrightness);
    inline void startAnime(QTimeLine *timeLine);
    QLinearGradient m_highLight;
    QColor m_highLightColor;
    QTimeLine *m_mouseIn, *m_mouseOut;
};

#endif // KNLINEARSENSEWIDGET_H
