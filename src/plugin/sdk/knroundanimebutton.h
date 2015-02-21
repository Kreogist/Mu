/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNROUNDANIMEBUTTON_H
#define KNROUNDANIMEBUTTON_H

#include "knabstractbutton.h"

class QTimeLine;
class KNRoundAnimeButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNRoundAnimeButton(QWidget *parent = 0);
    ~KNRoundAnimeButton();
    QPixmap icon() const
    {
        return m_icon;
    }
    QString text() const
    {
        return QString();
    }
    void setIcon(const QPixmap &icon)
    {
        m_icon=icon.scaled(QSize(m_iconSize, m_iconSize),
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
        update();
    }
    void setText(const QString &text)
    {
        Q_UNUSED(text)
    }

    bool checked() const;
    void setChecked(bool checked);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionLightnessChanged(const int &frame);

private:
    inline void stopAllAnimations();
    inline void startAnimation(QTimeLine *timeLine);
    inline QTimeLine *generateTimeLine(const int &finalFrame);
    QPixmap m_icon;
    QTimeLine *m_toSelect, *m_toUnselect;
    const int m_minimumLightness=80;
    int m_lightness=m_minimumLightness,
        m_iconSize=13,
        m_buttonSize=25,
        m_iconPos=(m_buttonSize-m_iconSize)>>1;
    bool m_checked=false, m_isPressed=false;
};

#endif // KNROUNDANIMEBUTTON_H
