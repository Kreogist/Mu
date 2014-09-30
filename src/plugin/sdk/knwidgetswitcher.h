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

#ifndef KNWIDGETSWITCHER_H
#define KNWIDGETSWITCHER_H

#include <QList>
#include <QWidget>

class QPropertyAnimation;
class QParallelAnimationGroup;
class KNWidgetSwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNWidgetSwitcher(QWidget *parent = 0);
    void addWidget(QWidget *widget);
    int count() const;
    int currentIndex() const;
    QWidget *widgetAt(const int &index);
    QWidget *currentWidget() const;

signals:
    void movedComplete();

public slots:
    void setCurrentIndex(int currentIndex);

protected:
    void resizeEvent(QResizeEvent *event);
    virtual void updateAnimationPosition()=0;
    int m_currentIndex=-1, m_outWidgetIndex=-1;
    QPropertyAnimation *m_inAnimation,
                       *m_outAnimation;

private slots:
    void hideMoveOutWidget();

private:
    void configureAnimation(QPropertyAnimation *animation);
    void setWidgetVisible(const int &index,
                          const bool &visible);
    QList<QWidget *> m_widgets;
    QParallelAnimationGroup *m_movingAnimationGroup;
};

#endif // KNWIDGETSWITCHER_H
