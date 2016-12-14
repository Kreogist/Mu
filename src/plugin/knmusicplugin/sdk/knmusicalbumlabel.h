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

#ifndef KNMUSICALBUMLABEL_H
#define KNMUSICALBUMLABEL_H

#include <QWidget>

/*!
 * \brief The KNMusicAlbumLabel class provides a simple label widget for image
 * only. And it will make the album art content a little bit rotate on z-axis.
 */
class KNMusicAlbumLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAlbumLabel widget.
     * \param parent The parent widget.
     */
    explicit KNMusicAlbumLabel(QWidget *parent = 0);

    /*!
     * \brief Get the artwork of the album label.
     * \return The QPixmap format artwork of the current label.
     */
    QPixmap pixmap() const;

signals:

public slots:
    /*!
     * \brief Set the artwork of the album art.
     * \param artwork The cover image of the album.
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
    inline void updateArtwork();
    QTransform m_transform;
    QPixmap m_artwork, m_originalArtwork;
};

#endif // KNMUSICALBUMLABEL_H
