/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMESSAGEBLOCK_H
#define KNMESSAGEBLOCK_H

#include <QLabel>

class KNMessageBlock : public QLabel
{
    Q_OBJECT
public:
    explicit KNMessageBlock(QWidget *parent = 0);
    void setTopColor(const QColor &topColor);
    void setButtomColor(const QColor &bottomColor);
    int preferWidth();

signals:
    void requireMoveTo(const QPoint &destination);

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void updateSizeParam();
    void updateBackground();
    QLinearGradient m_backgroundGradient;
    QPalette m_palette;
    bool m_pressed=false;
    QPoint m_referencePoint;
};


#endif // KNMESSAGEBLOCK_H
