/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMOUSESENSEHEADER_H
#define KNMOUSESENSEHEADER_H

#include <QPixmap>

#include <QHeaderView>

class QTimeLine;
class KNMouseSenseHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit KNMouseSenseHeader(QWidget *parent = 0);

signals:
    void requireChangeVisible(const int &index,
                              const bool &visible);

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintSection(QPainter *painter,
                      const QRect &rect,
                      int logicalIndex) const;

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    QTimeLine *m_mouseIn, *m_mouseOut;
    int m_outBrightness=0x20, m_inBrightness=0x40, m_parameter;
    QColor m_backgroundColor,
           m_lineColor=QColor(255,255,255),
           m_buttonColor=QColor(255,255,255),
           m_buttonTextColor=QColor(255,255,255);
    QPalette m_palette;
    QLinearGradient m_buttonGradient;
    QPixmap m_ascendingIcon, m_descendingIcon;
};

#endif // KNMOUSESENSEHEADER_H
