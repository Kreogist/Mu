/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNABSTRACTSLIDER_H
#define KNABSTRACTSLIDER_H

#include <QWidget>

class KNAbstractSlider : public QWidget
{
    Q_OBJECT
public:
    KNAbstractSlider(QWidget *parent = 0):QWidget(parent){}
    virtual qint64 maximum() const;
    virtual qint64 minimal() const;
    virtual qint64 value() const;
    virtual qint64 range() const;
    virtual qreal percentage() const;
    virtual qint64 wheelStep() const;
    virtual void setRange(qint64 min, qint64 max);
    virtual void setValue(qint64 value);
    virtual void setWheelStep(qint64 wheelStep);

signals:
    void rangeChanged(float min, float max);
    void sliderMoved(float value);
    void sliderPressed();
    void sliderReleased();
    void valueChanged(float value);

public slots:
    virtual void setMaximum(qint64 maximum);
    virtual void setMinimal(qint64 minimal);

protected:
    void wheelEvent(QWheelEvent *event);

private:
    void updateRange();
    qint64 m_maximum=100, m_minimal=0, m_value=0, m_range=100, m_wheelStep=1;
    qreal m_percentage=0;
};

#endif // KNABSTRACTSLIDER_H
