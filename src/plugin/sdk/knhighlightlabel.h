/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNHIGHLIGHTLABEL_H
#define KNHIGHLIGHTLABEL_H

#include <QLabel>

class KNHighlightLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KNHighlightLabel(QWidget *parent = 0);
    inline void setAlbumSize(const int &albumSize)
    {
        setFixedSize(albumSize, albumSize);
    }

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void updateHighlightArea();
    QLinearGradient m_highlight;
    QPointF m_highLightArea[3];
};

#endif // KNHIGHLIGHTLABEL_H
