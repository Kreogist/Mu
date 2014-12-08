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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef KNCATEGORYBUTTON_H
#define KNCATEGORYBUTTON_H

#include "knabstractbutton.h"

class QTimeLine;
class KNCategoryButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNCategoryButton(QWidget *parent = 0);
    bool checked() const;
    QPixmap icon() const;
    QString text() const;
    void setIcon(const QPixmap &icon);
    void setText(const QString &text);
    void setChecked(bool checked);
    static int buttonHeight();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onCheckedChanged();
    void mouseInOutAnimeFrame(const int &frame);
    void mouseUpDownAnimeFrame(const int &frame);

private:
    void startMouseInAnime();
    void startMouseOutAnime();
    void startMouseDownAnime();
    void startMouseUpAnime();
    void updatePositionParameter();
    void configureInOutTimeLine(QTimeLine *timeline);
    void configureMouseUpDownTimeLine(QTimeLine *timeline);

    //Button information.
    QPixmap m_icon;
    QString m_text;

    //Animation timeline and parameters.
    QTimeLine *m_mouseIn, *m_mouseOut, *m_mouseDown, *m_mouseUp;
    QLinearGradient m_background;
    QColor m_highLightColor=QColor(200,200,200,0),
           m_gradientColor=QColor(255,255,255,0);
    qreal m_textAlpha=0.0;

    //Button states.
    bool m_checked=false, m_pressed=false;

    //Auxiliary painting parameters.
    int m_iconCenterX=0;
    int m_textWidth=0;
    int m_textFullWidth=0;

    //UI Tweaks Parameters
    static int m_fixedHeight;
    int m_maxMouseInAlpha=100, m_maxMouseDownAlpha=150;
    int m_iconSize=20;
    int m_iconY=5;
    int m_textY=5;
    int m_spacing=5;
};

#endif // KNCATEGORYBUTTON_H
