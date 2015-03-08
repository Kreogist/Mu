/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNGLASSANIMEBUTTON_H
#define KNGLASSANIMEBUTTON_H

#include "knabstractbutton.h"

class QTimeLine;
class KNGlassAnimeButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNGlassAnimeButton(QWidget *parent = 0);
    ~KNGlassAnimeButton();
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
    void setLeftLineVisible(const bool &visible);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionHighlightChanged(const int &highlight);

private:
    QLinearGradient m_lineGradient, m_backGradient;

    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void updateScaledIcon();
    inline void stopAllAnimations();
    inline void startAnimation(QTimeLine *timeline);
    QTimeLine *m_mouseDown, *m_mouseUp, *m_mouseIn, *m_mouseOut;
    bool m_isPressed=false, m_leftline=false;
    int m_currentHighlight=0;
    QPixmap m_originalIcon, m_scaledIcon;
};

#endif // KNGLASSANIMEBUTTON_H
