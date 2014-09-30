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
#ifndef KNHEADERBUTTON_H
#define KNHEADERBUTTON_H

#include <QFontMetrics>

#include "knanimecheckedbutton.h"

class QGraphicsOpacityEffect;
class QTimeLine;
class QLabel;
class KNHeaderButton : public KNAnimeCheckedButton
{
    Q_OBJECT
public:
    explicit KNHeaderButton(QWidget *parent = 0);
    void setIcon(const QPixmap &pixmap);
    void setText(const QString &text);
    QPixmap icon() const;
    QString text() const;

signals:
    void requireShowCategorySwitcher();
    void requireSetLeftSpacing(int leftSpacing);

public slots:
    void restoreButtonState();

protected:
    void startMouseInAnime();
    void startMouseOutAnime();
    void startMouseDownAnime();
    void startMouseUpAnime();

private slots:
    void onActionMouseAnimation(const int &frame);
    void onActionMouseDownUpAnimation(const int &frame);

private:
    void configurePalette();
    void initialIcon();
    void initialText();
    void initialTimeLine();
    void initialMouseInOutTimeLine(QTimeLine *anime);
    void initialMouseDownTimeLine(QTimeLine *anime);
    void initialMouseUpTimeLine(QTimeLine *anime);
    void updateWidth();
    QTimeLine *m_mouseIn, *m_mouseOut, *m_mouseUp, *m_mouseDown;
    QLabel *m_icon, *m_text;
    QGraphicsOpacityEffect *m_textEffect;
    QRadialGradient m_iconGradient;
    QLinearGradient m_textGradient;
    QColor m_opacityColor=QColor(255,255,255,0);
    QFont m_textFont;
    QFontMetrics m_rawFontMetrics=QFontMetrics(m_textFont);
    int m_iconSize=190,
        m_textY=25,
        m_iconX=-80,
        m_iconMouseInX=-120,
        m_iconMouseUpX=-250,
        m_iconY=-85,
        m_textSize=40,
        m_textAlpha=220,
        m_leftMargin=63,
        m_spacing=5;
    qreal m_iconMoved=qAbs((qreal)m_iconMouseInX-(qreal)m_iconX),
          m_iconDownMoved=qAbs((qreal)m_iconSize-(qreal)m_iconX),
          m_iconSelectMoved=qAbs((qreal)m_iconMouseUpX-(qreal)m_iconX);
};

#endif // KNHEADERBUTTON_H
