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
#include <QLabel>

#include "knthememanager.h"
#include "kndpimanager.h"
#include "knroundswitchbutton.h"
#include "knfileassociationmanager.h"

#include "knmusicglobal.h"

#include "knpreferencepanelfiletypeitem.h"

#include <QDebug>

KNPreferencePanelFileTypeItem::KNPreferencePanelFileTypeItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_suffix(QString()),
    m_typeHandler(QString()),
    m_button(new KNRoundSwitchButton(this)),
    m_fullName(new QLabel(this))
{
    //Configure the button.
    m_button->updatePalette(knTheme->getPalette("PreferenceItemSwitcher"));
    //Update the palette.
    m_fullName->setPalette(palette());
    //Build the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    contentLayout->addWidget(m_button);
    contentLayout->addSpacing(knDpi->width(8));
    contentLayout->addWidget(m_fullName);
    contentLayout->addStretch();
    //Link the button.
    connect(m_button, &KNRoundSwitchButton::clicked,
            this, &KNPreferencePanelFileTypeItem::onClicked);
    //Build the widget.
    buildWidgetLayout(contentLayout);
}

QVariant KNPreferencePanelFileTypeItem::value() const
{
    //Get the data from the manager.
    return isTypeBind();
}

void KNPreferencePanelFileTypeItem::setConfig(const QVariant &config)
{
    //The config would be string map for the suffix and handler.
    QJsonObject configObject=config.toJsonObject();
    //Save the value.
    m_suffix=configObject.value("suffix").toString();
    m_typeHandler=configObject.value("handler").toString();
    //Set the description
    m_fullName->setText(knMusicGlobal->typeDescription(m_suffix.mid(1)));
}

void KNPreferencePanelFileTypeItem::setWidgetValue(const QVariant &value)
{
    //Set the file association.
    setAssociations(value.toBool());
}

bool KNPreferencePanelFileTypeItem::isEqual(const QVariant &currentValue,
                                            const QVariant &originalValue)
{
    //Treat the value as bool.
    return currentValue.toBool()==originalValue.toBool();
}

QVariant KNPreferencePanelFileTypeItem::getValueFromConfig(
        const QVariant &defaultValue)
{
    //Ignore the application setting for the default value.
    Q_UNUSED(defaultValue);
    return isTypeBind();
}

void KNPreferencePanelFileTypeItem::initialValue(const QVariant &defaultValue)
{
    //Update the switch with the value.
    m_button->setChecked(defaultValue.toBool());
}

void KNPreferencePanelFileTypeItem::onClicked()
{
    //Get the current button value.
    bool bindState=setAssociations(m_button->isChecked());
    //Check the value.
    if(bindState!=m_button->isChecked())
    {
        //Update the button state.
        m_button->setForceChecked(bindState);
    }
}

inline bool KNPreferencePanelFileTypeItem::isTypeBind() const
{
#ifdef Q_OS_WIN
    if(m_suffix.isEmpty())
#else
    if(m_typeHandler.isEmpty())
#endif
    {
        return false;
    }
    //Check using the file association manager.
    return knFileType->isFileTypeBinded(m_suffix, m_typeHandler);
}

bool KNPreferencePanelFileTypeItem::setAssociations(bool bind)
{
    //The value is a boolean type value.
    if(bind)
    {
        //Bind the file type.
        return knFileType->bindFileTypes(
                    m_suffix, m_typeHandler,
                    knMusicGlobal->typeDescription(m_suffix.mid(1)));
    }
    else
    {
        //Unbind the file type.
        return !knFileType->unbindFileTypes(m_suffix, m_typeHandler);
    }
}
