/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNROUNDANIMEBUTTON_H
#define KNROUNDANIMEBUTTON_H

#include "knabstractbutton.h"

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
        m_icon=icon.scaled(QSize(20, 20),
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
        update();
    }
    void setText(const QString &text)
    {
        Q_UNUSED(text)
    }

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_icon;
    int m_opacity=100;
};

#endif // KNROUNDANIMEBUTTON_H
