/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QJsonObject>
#include <QSpinBox>
#include <QSlider>

#include "knthememanager.h"

#include "knpreferencepanelintitem.h"

KNPreferencePanelIntItem::KNPreferencePanelIntItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_integerViewer(new QSpinBox(this)),
    m_integerSlider(new QSlider(Qt::Horizontal, this))
{
    //Get the palette of the widget.
    m_integerViewer->setPalette(knTheme->getPalette("PreferenceItemSpinBox"));
    m_integerSlider->setPalette(knTheme->getPalette("PreferenceItemSlider"));
    //Configure the viewer and slider.
    connect(m_integerSlider, &QSlider::valueChanged,
            this, &KNPreferencePanelIntItem::onValueChanged);
    connect(m_integerViewer, SIGNAL(valueChanged(int)),
            this, SLOT(onValueChanged(int)));

    //Construct the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(m_integerViewer);
    mainLayout->addWidget(m_integerSlider, 1);
    //Build the widget.
    buildWidgetLayout(mainLayout);
}

QVariant KNPreferencePanelIntItem::value() const
{
    //Use the viewer data as value.
    return m_integerViewer->value();
}

void KNPreferencePanelIntItem::setConfig(const QVariant &config)
{
    //The config would be a list of the int, for the min and max.
    QJsonObject configList=config.toJsonObject();
    //Get the min and max value.
    int min=configList.value("min").toInt(0),
            max=configList.value("max").toInt(100);
    //Set the range value.
    m_integerSlider->setRange(min, max);
    m_integerViewer->setRange(min, max);
}

void KNPreferencePanelIntItem::setWidgetValue(const QVariant &value)
{
    //Sync the widget value.
    syncValue(value.toInt());
}

bool KNPreferencePanelIntItem::isEqual(const QVariant &currentValue,
                                       const QVariant &originalValue)
{
    //Compare the integer.
    return currentValue.toInt()==originalValue.toInt();
}

void KNPreferencePanelIntItem::onValueChanged(int value)
{
    //Sync the value.
    syncValue(value);
}

inline void KNPreferencePanelIntItem::syncValue(int value)
{
    //Block the signal.
    setSignalBlock(true);
    //Save the value.
    m_integerViewer->setValue(value);
    m_integerSlider->setValue(value);
    //Release the block of the signal.
    setSignalBlock(false);
    //Emit the value changed signal.
    emit valueChanged();
}

inline void KNPreferencePanelIntItem::setSignalBlock(bool block)
{
    //Update the block.
    m_integerViewer->blockSignals(block);
    m_integerSlider->blockSignals(block);
}
