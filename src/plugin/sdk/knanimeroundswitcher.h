/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNANIMEROUNDSWITCHER_H
#define KNANIMEROUNDSWITCHER_H

#include <QPen>

#include <QWidget>

class QTimeLine;
class KNAnimeRoundSwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNAnimeRoundSwitcher(QWidget *parent = 0);

    bool buttonAtLeft() const;
    void setButtonAtLeft(bool buttonAtLeft);

signals:
    void buttonPositionChange(bool atLeft);

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionMove(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    void launchAnime();
    int m_switcherHeight=20,
        m_switcherWidth=42,
        m_buttonX=0;
    bool m_buttonAtLeft=true, m_mousePressed=false;
    QPainterPath m_backgroundRect;
    QLinearGradient m_backgroundBrush,
                    m_buttonBrush;
    QPen m_buttonBorder;
    QTimeLine *m_moveToLeft, *m_moveToRight;
};

#endif // KNANIMEROUNDSWITCHER_H
