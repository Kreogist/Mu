/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNANIMECOLORSWITCHER_H
#define KNANIMECOLORSWITCHER_H

#include <QWidget>

class QTimeLine;
class KNAnimeColorSwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNAnimeColorSwitcher(QWidget *parent = 0);
    bool value() const;
    void setValue(bool value);
    void setHighLightColor(const QColor &highLightColor);
    void setButtonColor(const QColor &buttonColor);

signals:
    void valueChanged(bool value);

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionUpdateParameter(const int &frame);

private:
    void moveButtonLeft();
    void moveButtonRight();
    void configureTimeLine(QTimeLine *timeLine);
    int m_switcherHeight=25, m_switcherWidth=65, m_buttonWidth=40,
        m_buttonX=0;
    bool m_value=false, m_pressed=false;
    QTimeLine *m_moveLeft, *m_moveRight;
    QColor m_buttonBorder;
    QLinearGradient m_buttonGradient,
                    m_blankGradient,
                    m_switchGradient,
                    m_highlightGradient;
};

#endif // KNANIMECOLORSWITCHER_H
