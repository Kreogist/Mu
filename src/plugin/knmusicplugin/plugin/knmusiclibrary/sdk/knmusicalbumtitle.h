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

#ifndef KNMUSICALBUMTITLE_H
#define KNMUSICALBUMTITLE_H

#include <QWidget>

/*!
 * \brief The KNMusicAlbumTitle class provides the album art label widget which
 * could display the album art image and auto scaled it to fit the size.
 */
class KNMusicAlbumTitle : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAlbumTitle widget with given parent.
     * \param parent The parent widget.
     */
    explicit KNMusicAlbumTitle(QWidget *parent = 0);

    /*!
     * \brief Get the current album art pixmap.
     * \return The album art image.
     */
    QPixmap albumArt() const;

signals:
    /*!
     * \brief When the mouse is moving on this widget, this signal is emitted to
     * ask to show the album art.
     */
    void requireShowAlbumArt();

    /*!
     * \brief When the mouse is moving out from this widget, the signal is
     * emitted to ask to hide the album art widget.
     */
    void requireHideAlbumArt();

public slots:
    /*!
     * \brief Set the album art image.
     * \param albumArt The album art pixmap image.
     */
    void setAlbumArt(const QPixmap &albumArt);

    /*!
     * \brief Clear the album art image, release the memory resource.
     */
    void clearAlbumArt();

protected:
    /*!
     * \brief Reimplemnted from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemnted from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemnted from QWidget::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemnted from QWidget::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemnted from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemnted from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    inline void setMouseIn()
    {
        //Cut down the calling and check the flag.
        if(!m_mouseIn)
        {
            //Set the flag.
            m_mouseIn=true;
            //Ask to show the album art.
            emit requireShowAlbumArt();
        }
    }
    inline void updateScaledAlbumArt();
    QPixmap m_albumArt, m_scaledAlbumArt;
    bool m_mouseIn;
};

#endif // KNMUSICALBUMTITLE_H
