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

class KNMusicAlbumTitle : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumTitle(QWidget *parent = 0);

    QPixmap albumArt() const;

signals:
    void requireShowAlbumArt();
    void requireHideAlbumArt();

public slots:
    void setAlbumArt(const QPixmap &albumArt);

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
