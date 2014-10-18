/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNLINEARSENSEWIDGET_H
#define KNLINEARSENSEWIDGET_H

#include <QWidget>

class QTimeLine;
class KNLinearSenseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNLinearSenseWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor=QColor(0x27,0x27,0x27),
           m_linearColor=m_backgroundColor;
    QLinearGradient m_background;
    int m_inBrightness=0x60, m_outBrightness=m_backgroundColor.value();
};

#endif // KNLINEARSENSEWIDGET_H
