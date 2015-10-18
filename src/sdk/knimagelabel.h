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

#ifndef KNIMAGELABEL_H
#define KNIMAGELABEL_H

#include <QWidget>

/*!
 * \brief The KNImageLabel class.
 */
class KNImageLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNImageLabel widget.
     * \param parent The parent widget.
     */
    explicit KNImageLabel(QWidget *parent = 0);

    /*!
     * \brief Get the pixmap of the label.
     * \return The pixmap. If the pixmap is not been set before, it will be a
     * null image.
     */
    QPixmap pixmap() const;

signals:

public slots:
    /*!
     * \brief Set the lable pixmap.
     * \param pixmap The pixmap image.
     */
    void setPixmap(const QPixmap &pixmap);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    inline void updateScaledPixmap();
    QPixmap m_pixmap, m_scaledPixmap;
    int m_pixmapX, m_pixmapY;
};

#endif // KNIMAGELABEL_H
