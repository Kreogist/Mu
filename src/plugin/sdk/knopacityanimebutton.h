/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNOPACITYANIMEBUTTON_H
#define KNOPACITYANIMEBUTTON_H

#include "knabstractbutton.h"

class QPropertyAnimation;
class QGraphicsOpacityEffect;
class KNOpacityAnimeButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNOpacityAnimeButton(QWidget *parent = 0);
    QPixmap icon() const;
    QSize sizeHint() const;
    void setIcon(const QPixmap &icon);
    QString text() const
    {
        return QString();
    }
    void setText(const QString &text)
    {
        Q_UNUSED(text)
    }

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void configureAnimation(QPropertyAnimation *animation);
    QGraphicsOpacityEffect *m_effect;
    QPropertyAnimation *m_mouseDown, *m_mouseUp, *m_mouseIn, *m_mouseOut;
    qreal m_startOpacity=0.4, m_inoutOpacity=0.8;
    bool m_isPressed=false;
    QPixmap m_icon;
};

#endif // KNOPACITYPIXMAPBUTTON_H
