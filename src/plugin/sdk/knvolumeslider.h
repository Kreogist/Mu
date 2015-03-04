/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNVOLUMESLIDER_H
#define KNVOLUMESLIDER_H

#include "knabstractslider.h"

class QTimeLine;
class KNVolumeSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    explicit KNVolumeSlider(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    inline QTimeLine *generateTimeline(const int &endFrame);
    qint64 posToValue(int position);
    int m_sliderHeight=5, m_spacing=2, m_maxAlpha=255, m_minAlpha=80;
    bool m_pressed=false;
    QColor m_rectColor=QColor(255,255,255,m_minAlpha);
    QTimeLine *m_mouseIn, *m_mouseOut;
};

#endif // KNVOLUMESLIDER_H
