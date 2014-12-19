/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
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
    QPropertyAnimation *m_inAnimation, *m_outAnimation;

private slots:
    void hideMoveOutWidget();

private:
    inline void configureAnimation(QPropertyAnimation *animation);
    inline void setWidgetVisible(const int &index, const bool &ok);
    QList<QWidget *> m_widgets;
    QParallelAnimationGroup *m_movingAnimationGroup;
};

#endif // KNWIDGETSWITCHER_H
