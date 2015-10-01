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
#ifndef KNMUSICRATINGEDITOR_H
#define KNMUSICRATINGEDITOR_H

#include <QWidget>

class KNMusicRatingEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicRatingEditor(QWidget *parent = 0);
    QSize sizeHint() const Q_DECL_OVERRIDE
    {
        return QSize(m_editorWidth, m_starSizeHint);
    }
    int starSizeHint() const;
    void setStarSizeHint(int starSizeHint);
    int starNum() const;
    void setStarNum(int starNum);

signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int starAtPosition(int x);
    QPixmap m_star, m_unstar, m_scaleStar, m_scaleUnstar;
    int m_starSizeHint, m_editorWidth, m_starNum, m_halfStarSizeHint;
};

#endif // KNMUSICRATINGEDITOR_H
