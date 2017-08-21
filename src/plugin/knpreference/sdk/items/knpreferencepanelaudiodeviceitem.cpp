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
    //Build the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    contentLayout->addWidget(m_candidates, 1);
    //Build the widget.
    buildWidgetLayout(contentLayout);
}

QVariant KNPreferencePanelAudioDeviceItem::value() const
{
    //The current text of the candidate combo is the value.
    return m_candidates->currentData(Qt::UserRole);
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
        QString deviceName=deviceInfo.value("Name").toString();
        //Check the device Id is the default value or not.
        if(deviceInfo.value("Id").toInt()==-1)
        {
            //Set the device name as the default value.
            deviceName=tr("Use Default Output Device");
        }
        //Append the device name to the combo widget.
        m_candidates->addItem(deviceName, deviceInfo);
    }
    //Link the combo box signal.
    connect(m_candidates,
            static_cast<void (QComboBox::*)(int)>(
                &QComboBox::currentIndexChanged),
            this, &KNPreferencePanelAudioDeviceItem::valueChanged);
}

void KNPreferencePanelAudioDeviceItem::setWidgetValue(const QVariant &value)
{
    //If the value can be cast to json object and device Id is existed, we could
    //move on.
    if(!value.canConvert<QJsonObject>())
    {
        //Failed to recast.
        return;
    }
    //Check the Id.
    QJsonObject userDevice=value.toJsonObject();
    if(!userDevice.contains("Id"))
    {
        //No need to move on.
        return;
    }
    //Get the user device Id.
    int deviceId=userDevice.value("Id").toInt();
    //Need to search in the candidate combo data.
    for(int i=0, deviceCount=m_candidates->count(); i<deviceCount; ++i)
    {
        //Get the user data from the model.
        QJsonObject deviceInfo=
                m_candidates->itemData(i, Qt::UserRole).toJsonObject();
        if(deviceInfo.value("Id").toInt()==deviceId)
        {
            //This is what we want.
            m_candidates->setCurrentIndex(i);
            //Mission complete.
            return;
        }
    }
    //Or else, leave it as what it is.
}

bool KNPreferencePanelAudioDeviceItem::isEqual(const QVariant &currentValue,
                                         const QVariant &originalValue)
{
    //Directly compares the value as the string.
    return currentValue.toString()==originalValue.toString();
}
