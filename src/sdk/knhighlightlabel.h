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

#ifndef KNHIGHLIGHTLABEL_H
#define KNHIGHLIGHTLABEL_H

#include <QWidget>

/*!
 * \brief The KNHighLightLabel class provides a pixmap label with high light
 * layer on it. This version is now implemented from QWidget and draw all the
 * content itself.
 */
class KNHighLightLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNHighLightLabel widget.
     * \param parent The parent widget.
     */
    explicit KNHighLightLabel(QWidget *parent = 0);

    /*!
     * \brief Set the pixmap content of the label.
     * \param pixmap The raw pixmap.
     */
    void setPixmap(const QPixmap &pixmap);

    /*!
     * \brief Reimplemented from QWidget::sizeHint().
     */
    QSize sizeHint() const;

signals:

public slots:

protected:
    /*!
     * \brief Reimplement from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Reimplement from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private:
    inline void updateHighLightArea();
    inline void updateScaledContent();
    QLinearGradient m_highlight;
    QPoint m_contentPosition;
    QPixmap m_rawContent, m_scaledContent;
    QPointF m_highLightArea[3];
};

#endif // KNHIGHLIGHTLABEL_H
