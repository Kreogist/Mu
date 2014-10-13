/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICCATEGORYLISTVIEWBASE_H
#define KNMUSICCATEGORYLISTVIEWBASE_H

#include <QListView>

class QTimeLine;
class KNMusicCategoryListViewBase : public QListView
{
    Q_OBJECT
public:
    explicit KNMusicCategoryListViewBase(QWidget *parent = 0);

signals:

public slots:

protected:
    void enabledDragDrop();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureTimeLine(QTimeLine *timeLine);
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor=QColor(255,255,255),
           m_textColor=QColor(255,255,255);
    int m_inBrightness=0x50, m_outBrightness=0x30,
        m_textBrightnessGap=0xaf;
};

#endif // KNMUSICCATEGORYLISTVIEWBASE_H
