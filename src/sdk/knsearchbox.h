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

#ifndef KNSEARCHBOX_H
#define KNSEARCHBOX_H

#include <QLineEdit>

class QTimeLine;
class KNSearchBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit KNSearchBox(QWidget *parent = 0);

    QPixmap searchIcon() const;
    void setSearchIcon(const QPixmap &searchIcon);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionThemeChanged();
    void onActionMouseInOut(const int &frame);
    void onActionFocusInOut(const int &frame);

private:
    inline QTimeLine *generateTimeLine();
    inline void startAnime(QTimeLine *timeLine, const int &end);
    QPixmap m_searchIcon;
    QColor m_baseColor;
    QTimeLine *m_mouseInOut, *m_focusInOut;
};

#endif // KNSEARCHBOX_H
