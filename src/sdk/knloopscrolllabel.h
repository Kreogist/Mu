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

#ifndef KNLOOPSCROLLLABEL_H
#define KNLOOPSCROLLLABEL_H

#include <QWidget>

class KNLoopScrollLabel : public QWidget
{
    Q_OBJECT
public:
    explicit KNLoopScrollLabel(QWidget *parent = 0);

    QString text() const;

    QSize sizeHint() const;

    qreal opacity() const;

    int alignment() const;

signals:

public slots:
    void setAlignment(int alignment);
    void setOpacity(const qreal &opacity);
    void setText(const QString &text);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void moveText();

private:
    inline void stopTimers();
    inline void updateAnimeParameters();
    QString m_text;
    QTimer *m_move, *m_wait;
    qreal m_opacity;
    int m_alignment, m_textLeftMostX, m_textX;
    bool m_looped;
};

#endif // KNLOOPSCROLLLABEL_H
