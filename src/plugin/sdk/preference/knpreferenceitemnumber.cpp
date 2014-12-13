/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QSlider>
#include <QSpinBox>
#include <QBoxLayout>

#include "knpreferenceitemnumber.h"

KNPreferenceItemNumber::KNPreferenceItemNumber(QWidget *parent) :
    KNPreferenceItemBase(parent)
{
    //Initial the editor widget.
    QWidget *numberEditorWidget=new QWidget(this);
    numberEditorWidget->setContentsMargins(0,0,0,0);

    //Initial the editor widget layout.
    QBoxLayout *numberEditorLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                numberEditorWidget);
    numberEditorLayout->setContentsMargins(0,0,0,0);
    numberEditorLayout->setSpacing(5);
    numberEditorWidget->setLayout(numberEditorLayout);

    //Initial the slider.
    m_spinBox=new QSpinBox(numberEditorWidget);
    QPalette pal=m_spinBox->palette();
    pal.setColor(QPalette::Base, QColor(0x40, 0x40, 0x40));
    pal.setColor(QPalette::Button, QColor(0x50, 0x50, 0x50));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    pal.setColor(QPalette::Text, QColor(0xA0, 0xA0, 0xA0));
    m_spinBox->setPalette(pal);
    numberEditorLayout->addWidget(m_spinBox);

    m_slider=new QSlider(Qt::Horizontal, numberEditorWidget);
    m_slider->setFixedWidth(200);
    connect(m_slider, &QSlider::valueChanged, m_spinBox, &QSpinBox::setValue);
    connect(m_spinBox, SIGNAL(valueChanged(int)), m_slider, SLOT(setValue(int)));
    pal.setColor(QPalette::Button, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    m_slider->setPalette(pal);
    numberEditorLayout->addWidget(m_slider);

    numberEditorLayout->addStretch();

    //Insert the control widget.
    insertControlWidget(numberEditorWidget);
}

void KNPreferenceItemNumber::setRange(int min, int max)
{
    m_spinBox->setRange(min, max);
    m_slider->setRange(min, max);
}

QVariant KNPreferenceItemNumber::defaultValue() const
{
    return m_defaultValue;
}

QVariant KNPreferenceItemNumber::value() const
{
    return m_spinBox->value();
}

void KNPreferenceItemNumber::setDefaultValue(const QVariant &defaultValue)
{
    //Set the default value.
    m_defaultValue=defaultValue.toInt();
    //When default value changed, set the value to the default value.
    setValue(m_defaultValue);
}

void KNPreferenceItemNumber::setValue(const QVariant &value)
{
    m_spinBox->setValue(value.toInt());
}
