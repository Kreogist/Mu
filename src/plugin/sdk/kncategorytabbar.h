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

#ifndef KNCATEGORYTABBAR_H
#define KNCATEGORYTABBAR_H

#include <QList>

#include <QWidget>

class QTimeLine;
class QBoxLayout;
class QSignalMapper;
class KNCategoryButton;
class KNCategoryTabBar : public QWidget
{
    Q_OBJECT
public:
    explicit KNCategoryTabBar(QWidget *parent = 0);
    void addCategory(const QPixmap &pixmap,
                     const QString &string);
    int currentIndex() const;
    int categorySize() const;

signals:
    void currentIndexChanged(int index);

public slots:
    void unlockBar();
    void setCurrentIndex(int buttonIndex);
    void setCategoryText(const int &index, const QString &text);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionInOutMouse(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    QBoxLayout *m_tabLayout;
    QSignalMapper *m_buttonMapper;
    QList<KNCategoryButton *> m_buttonList;
    QPalette m_palette;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QColor m_backgroundColor=QColor(255,255,255);
    int m_currentIndex=-1,
        m_inBridhtness=0x40,
        m_outBridhtness=0x20;
    bool m_locked=false;
};

#endif // KNCATEGORYTABBAR_H
