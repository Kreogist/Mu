/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNNGNLBUTTON_H
#define KNNGNLBUTTON_H

#include "knabstractbutton.h"

class QTimeLine;
class KNNGNLButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNNGNLButton(QWidget *parent = 0);
    QPixmap icon() const;
    void setIcon(const QPixmap &icon);
    QString text() const;
    void setText(const QString &text);
    QSize iconSize() const;
    void setIconSize(const QSize &iconSize);
    inline void setIconSize(int w, int h)
    {
        setIconSize(QSize(w,h));
    }

signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    void configureInOutTimeLine(QTimeLine *timeLine);
    inline void stopAllAnimations();
    inline void updateScaledImage();
    int m_leftQuotePosition=-1, m_quoteWidth=10, m_halfWidth=0,
        m_leftSpacing=10, m_spacing=5;
    bool m_pressed=false;
    QPainterPath m_leftQuotation, m_rightQuotation;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPixmap m_icon, m_scaledIcon;
    QSize m_iconSize;
    QString m_text;
    qreal m_quoteOpacity=0.0;
};

#endif // KNNGNLBUTTON_H
