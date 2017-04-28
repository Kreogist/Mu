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

#include "knthememanager.h"
#include "knroundswitchbutton.h"

#include "knpreferencepanelbooleanitem.h"

KNPreferencePanelBooleanItem::KNPreferencePanelBooleanItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_button(new KNRoundSwitchButton(this))
{
    //Configure the button.
    m_button->updatePalette(knTheme->getPalette("PreferenceItemSwitcher"));
    //Link the button.
    connect(m_button, &KNRoundSwitchButton::toggled,
            this, &KNPreferencePanelBooleanItem::valueChanged);
    //Build the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    contentLayout->addWidget(m_button);
    contentLayout->addStretch();
    //Build the widget.
    buildWidgetLayout(contentLayout);
}

QVariant KNPreferencePanelBooleanItem::value() const
{
    //The button checked state is the value.
    return m_button->isChecked();
}

void KNPreferencePanelBooleanItem::setWidgetValue(const QVariant &value)
{
    //Update the widget data.
    m_button->setForceChecked(value.toBool());
}

bool KNPreferencePanelBooleanItem::isEqual(const QVariant &currentValue,
                                           const QVariant &originalValue)
{
    return currentValue.toBool()==originalValue.toBool();
}
