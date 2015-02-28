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
    qreal opacity() const;
    bool enabled() const;

signals:

public slots:
    void setEnabled(bool enabled);
    void setOpacityGap(const qreal &opacityGap);
    void setOpacity(const qreal &opacity);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    inline void updateScaledIcon();
    inline qreal getPressedOpacity();
    QPixmap m_icon, m_scaledIcon;
    qreal m_opacityGap=0.2, m_originalOpacity=1.0,
          m_opacity=m_originalOpacity;
    bool m_pressed=false, m_disabled=false;
};

#endif // KNOPACITYBUTTON_H
