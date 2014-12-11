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
#ifndef KNMUSICDETAILTOOLTIPARTWORK_H
#define KNMUSICDETAILTOOLTIPARTWORK_H

#include <QTransform>

#include <QWidget>

class KNMusicDetailTooltipArtwork : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicDetailTooltipArtwork(QWidget *parent = 0);
    QPixmap artwork() const;
    void setArtwork(const QPixmap &artwork);
    void setArtworkSize(QSize size);
    inline void setArtworkSize(int w, int h)
    {
        setArtworkSize(QSize(w, h));
    }

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_artwork;
    QTransform m_transform;
    int m_spaceParamter=10;
};

#endif // KNMUSICDETAILTOOLTIPARTWORK_H
