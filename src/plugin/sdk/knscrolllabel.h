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
#ifndef KNSCROLLLABEL_H
#define KNSCROLLLABEL_H

#include <QWidget>

class KNGraphicsGlowEffect;
class KNScrollLabel : public QWidget
{
    Q_OBJECT
public:
    explicit KNScrollLabel(QWidget *parent = 0);
    QSize sizeHint() const;
    QString text() const;
    void setText(const QString &text);
    qreal opacity() const;
    QFont scrollFont() const;
    void setScrollFont(const QFont &scrollFont);

signals:

public slots:
    void setOpacity(const qreal &opacity);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionMoving();
    void onActionWatingFinished();

private:
    void stopAllTimer();
    void updateAnimeParameters();
    KNGraphicsGlowEffect *m_glowEffect;
    QFont m_scrollFont=font();
    QFontMetrics m_scrollFontMetrics=QFontMetrics(m_scrollFont);
    QString m_text;
    QTimer *m_movingAnime, *m_waiting;
    int m_leftMostX=0, m_textX=0;
    qreal m_glowRadius=9.0, m_opacity=1.0;
    bool m_movingLeft=true;
};

#endif // KNSCROLLLABEL_H
