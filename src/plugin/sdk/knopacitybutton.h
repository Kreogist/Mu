/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNOPACITYBUTTON_H
#define KNOPACITYBUTTON_H

#include "knabstractbutton.h"

class QGraphicsOpacityEffect;
class KNOpacityButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNOpacityButton(QWidget *parent = 0);
    QPixmap icon() const;
    void setIcon(const QPixmap &icon);
    QString text() const
    {
        return QString();
    }
    void setText(const QString &text)
    {
        Q_UNUSED(text)
    }
    qreal opacityGap() const;
    void setOpacityGap(const qreal &opacityGap);
    qreal opacity() const;
    void setOpacity(const qreal &opacity);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_icon;
    QGraphicsOpacityEffect *m_opacityEffect;
    qreal m_opacityGap=0.2, m_originalOpacity=1.0;
    bool m_pressed=false;
};

#endif // KNOPACITYBUTTON_H
