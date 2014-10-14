/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMOUSESENSEWIDGET_H
#define KNMOUSESENSEWIDGET_H

#include <QWidget>

class QTimeLine;
class KNMouseSenseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMouseSenseWidget(QWidget *parent = 0);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);
    int inBrightness() const;
    void setInBrightness(int inBrightness);
    int outBrightness() const;
    void setOutBrightness(int outBrightness);

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor=QColor(255,255,255);
    int m_inBrightness=0x40, m_outBrightness=0x20;
};

#endif // KNMOUSESENSEWIDGET_H
