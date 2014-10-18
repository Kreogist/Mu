/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNSIDESHADOWWIDGET_H
#define KNSIDESHADOWWIDGET_H

#include <QWidget>

namespace KNSideShadow
{
enum ShadowDirection
{
    TopShadow,
    LeftShadow,
    RightShadow,
    BottomShadow
};
}

using namespace KNSideShadow;

class KNSideShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNSideShadowWidget(QWidget *parent = 0);
    explicit KNSideShadowWidget(int direction=0, QWidget *parent=0);

    int direction() const;
    void setDirection(int direction);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void updateGradient();
    int m_direction;
    QLinearGradient m_shadow;
};

#endif // KNSIDESHADOWWIDGET_H
