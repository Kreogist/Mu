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

#ifndef KNMUSICCODECLABEL_H
#define KNMUSICCODECLABEL_H

#include <QWidget>

class KNMusicCodecLabel : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicCodecLabel(QWidget *parent = 0);

    QSize sizeHint() const;

signals:

public slots:
    void setSuffix(QString suffix);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    enum EncodeIcon
    {
        IconAAC,
        IconFLAC,
        IconLame,
        IconVorbis,
        IconWav,
        IconWma,
        EncodeIconCount
    };
    QStringList m_suffixList[EncodeIconCount];
    QPixmap m_formatIcons[EncodeIconCount];
    int m_imageSize, m_currentIndex;
};

#endif // KNMUSICCODECLABEL_H
