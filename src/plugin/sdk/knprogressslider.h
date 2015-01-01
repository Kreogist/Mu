/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPROGRESSSLIDER_H
#define KNPROGRESSSLIDER_H

#include "knabstractslider.h"

class QTimeLine;
class KNProgressSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    explicit KNProgressSlider(QWidget *parent = 0);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    void updateButtonSize();
    qint64 posToValue(int position);
    int m_sliderHeight=4, m_glowWidth=5, m_buttonSize=14,
        m_spacing=2;
    bool m_pressed=false;
    QColor m_rectColor=QColor(255,255,255,70),
           m_backgroundColor=QColor(255,255,255,80),
           m_buttonColor=QColor(255,255,255,110);
    QRadialGradient m_buttonGradient;
    QTimeLine *m_mouseIn, *m_mouseOut;
    qreal m_mouseOutOpacity=0.65,
          m_backOpacity=m_mouseOutOpacity;
};

#endif // KNPROGRESSSLIDER_H
