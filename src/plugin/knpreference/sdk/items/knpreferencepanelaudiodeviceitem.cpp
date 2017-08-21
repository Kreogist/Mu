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
#include <QAbstractItemView>
#include <QBoxLayout>
#include <QComboBox>
#include <QJsonObject>
#include <QJsonArray>

#include "knthememanager.h"

#include "knmusicbackend.h"
#include "knmusicglobal.h"

#include "knpreferencepanelaudiodeviceitem.h"

#include <QDebug>

KNPreferencePanelAudioDeviceItem::KNPreferencePanelAudioDeviceItem(
        QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_candidates(new QComboBox(this))
{
    //Configure the combo box.
    QPalette comboPalette=knTheme->getPalette("PreferenceItemCombo");
    m_candidates->setPalette(comboPalette);
    m_candidates->setEditable(false);
    m_candidates->view()->setPalette(comboPalette);
    //Link the combo box signal.
    connect(m_candidates, &QComboBox::currentTextChanged,
            this, &KNPreferencePanelAudioDeviceItem::valueChanged);
    //Build the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    contentLayout->addWidget(m_candidates, 1);
    //Build the widget.
    buildWidgetLayout(contentLayout);
}

QVariant KNPreferencePanelAudioDeviceItem::value() const
{
    //The current text of the candidate combo is the value.
    return m_candidates->currentText();
}

void KNPreferencePanelAudioDeviceItem::setConfig(const QVariant &config)
{
    Q_UNUSED(config)
    //Get the information of the output device.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the backend pointer.
    if(!backend)
    {
        //Ignore for null pointer.
        return;
    }
    //Get the device informations.
    QJsonArray deviceData=backend->deviceList();
    //Update the item texts.
    for(auto device : deviceData)
    {
        //Loop and set the text.
        QJsonObject deviceInfo=device.toObject();
        //Get the device name.
        QString deviceName=deviceInfo.value("name").toString();
        //Add the data to the configure list.
        if(deviceName.isEmpty())
        {
            //Set the device name as the default value.
            deviceName=tr("Use Default Output Device");
        }
        //Append the device name to the combo widget.
        m_candidates->addItem(deviceName, deviceInfo);
    }
}

void KNPreferencePanelAudioDeviceItem::setWidgetValue(const QVariant &value)
{
    //Need to search in the model.
    //Match the index in the model.
    int matchResult=m_candidates->findText(value.toString());
    //Check the match list.
    if(-1!=matchResult)
    {
        //Set the current index.
        m_candidates->setCurrentIndex(matchResult);
    }
}

bool KNPreferencePanelAudioDeviceItem::isEqual(const QVariant &currentValue,
                                         const QVariant &originalValue)
{
    //Directly compares the value as the string.
    return currentValue.toString()==originalValue.toString();
}
