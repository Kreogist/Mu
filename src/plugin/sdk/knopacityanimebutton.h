/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNOPACITYANIMEBUTTON_H
#define KNOPACITYANIMEBUTTON_H

#include "knabstractbutton.h"

class QTimeLine;
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
    void resizeEvent(QResizeEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionOpacityChanged(const int &opacity);

private:
    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void updateScaledIcon();
    inline void stopAllAnimations();
    inline void startAnimation(QTimeLine *timeline);
    QTimeLine *m_mouseDown, *m_mouseUp, *m_mouseIn, *m_mouseOut;
    bool m_isPressed=false;
    int m_startOpacity=400, m_inoutOpacity=800, m_currentOpacity=m_startOpacity;
    qreal m_opacity=m_currentOpacity/1000.0;
    QPixmap m_originalIcon, m_scaledIcon;
};

#endif // KNOPACITYPIXMAPBUTTON_H
