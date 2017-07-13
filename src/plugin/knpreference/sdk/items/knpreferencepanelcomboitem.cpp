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

#include "knpreferencepanelcomboitem.h"

#include <QDebug>

KNPreferencePanelComboItem::KNPreferencePanelComboItem(QWidget *parent) :
    KNPreferencePanelItem(parent),
    m_candidates(new QComboBox(this))
{
    //Configure the combo box.
    QPalette comboPalette=knTheme->getPalette("PreferenceItemCombo");
    m_candidates->setPalette(comboPalette);
    m_candidates->view()->setPalette(comboPalette);
    //Link the combo box signal.
    connect(m_candidates, &QComboBox::currentTextChanged,
            this, &KNPreferencePanelComboItem::valueChanged);
    //Build the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    contentLayout->addWidget(m_candidates, 1);
    //Build the widget.
    buildWidgetLayout(contentLayout);
}

QVariant KNPreferencePanelComboItem::value() const
{
    //The current text of the candidate combo is the value.
    return m_candidates->currentText();
}

void KNPreferencePanelComboItem::setConfig(const QVariant &config)
{
    //The config would be a list of the int, for the min and max.
    QJsonObject configList=config.toJsonObject();
    //Update the preference of the combo box.
    m_candidates->setEditable(configList.value("edit").toBool());
    //Update the item texts.
    QJsonArray candidates=configList.value("candidates").toArray();
    for(int i=0; i<candidates.size(); ++i)
    {
        //Loop and set the text.
        m_candidates->addItem(candidates.at(i).toString());
    }
}

void KNPreferencePanelComboItem::setWidgetValue(const QVariant &value)
{
    //Check the editable settings.
    if(m_candidates->isEditable())
    {
        //Directly set the current text.
        m_candidates->setCurrentText(value.toString());
    }
    else
    {
        //Need to search in the model.
        //Match the index in the model.
        QAbstractItemModel *candidateModel=m_candidates->model();
        int matchResult=m_candidates->findText(value.toString());
        //Check the match list.
        if(-1!=matchResult)
        {
            //Set the current index.
            m_candidates->setCurrentIndex(matchResult);
        }
    }
}

bool KNPreferencePanelComboItem::isEqual(const QVariant &currentValue,
                                         const QVariant &originalValue)
{
    //Directly compares the value as the string.
    return currentValue.toString()==originalValue.toString();
}
