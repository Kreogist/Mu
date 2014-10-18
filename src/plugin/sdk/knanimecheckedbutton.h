/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNANIMECHECKEDBUTTON_H
#define KNANIMECHECKEDBUTTON_H

#include "knabstractbutton.h"

class KNAnimeCheckedButton : public KNAbstractButton
{
    Q_OBJECT
public:
    explicit KNAnimeCheckedButton(QWidget *parent = 0);
    bool checked() const;

signals:

public slots:
    void setChecked(bool checked);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    virtual void startMouseInAnime()=0;
    virtual void startMouseOutAnime()=0;
    virtual void startMouseDownAnime()=0;
    virtual void startMouseUpAnime()=0;

protected slots:
    virtual void onCheckedChanged();

private slots:

private:
    bool m_pressed=false, m_checked=false;
};

#endif // KNANIMECHECKEDBUTTON_H
