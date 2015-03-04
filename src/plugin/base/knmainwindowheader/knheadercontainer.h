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
#ifndef KNHEADERCONTAINER_H
#define KNHEADERCONTAINER_H

#include <QWidget>

class QTimeLine;
class KNHeaderContainer : public QWidget
{
    Q_OBJECT
public:
    explicit KNHeaderContainer(QWidget *parent = 0);
    void setHeaderSwitcher(QWidget *switcher);
    void setHeaderButton(QWidget *button);
    void setLeftSpacing(const int &spacing);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void changeBackgroundColor(const int &frameData);

private:
    inline void configurePalette();
    inline QTimeLine *generateTimeline(const int &endFrame);
    inline void updateSwitcherPosition();
    QPalette m_palette;
    int minimumBrightness=0x10,
        maximumBrightness=0x50,
        m_leftSpacing=0;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QWidget *m_switcher=nullptr;
    QColor m_backgroundColor=QColor(0,0,0);
};

#endif // KNHEADERCONTAINER_H
