/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMNUMBER_H
#define KNPREFERENCEITEMNUMBER_H

#include <QWidget>

#include "knpreferenceitembase.h"

class QSpinBox;
class QSlider;
class KNPreferenceItemNumber : public KNPreferenceItemBase
{
    Q_OBJECT
public:
    explicit KNPreferenceItemNumber(QWidget *parent = 0);
    void setRange(int min, int max);
    QVariant defaultValue() const;
    QVariant value() const;

public slots:
    void setDefaultValue(const QVariant &defaultValue);
    void setValue(const QVariant &value);

private:
    QSlider *m_slider;
    QSpinBox *m_spinBox;
    int m_defaultValue=0;
};

#endif // KNPREFERENCEITEMNUMBER_H
