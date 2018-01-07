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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>

#include "knlabelbutton.h"
#include "kndpimanager.h"

#include "knpreferencepaneldirlistitem.h"

KNPreferencePanelDirListItem::KNPreferencePanelDirListItem(QWidget *parent) :
    KNPreferencePanelItem(parent)
{
    //Construct the main layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    buttonLayout->setSpacing(knDpi->width(5));
    //Add the default layout to the button layout.
    KNLabelButton *add=generateButton("://preference/folder_add.png",
                                      tr("Add a new path")),
                  *remove=generateButton("://preference/folder_delete.png",
                                         tr("Remove the selected paths"));
    //Add the button to the layout.
    buttonLayout->addWidget(add);
    buttonLayout->addWidget(remove);
    buttonLayout->addStretch();
    //Construct the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(createWidgetLayout(buttonLayout));
    setLayout(mainLayout);
}

QVariant KNPreferencePanelDirListItem::value() const
{
    return QStringList();
}

int KNPreferencePanelDirListItem::itemHeightMultiple() const
{
    return 1;
}

void KNPreferencePanelDirListItem::setWidgetValue(const QVariant &value)
{
    ;
}

bool KNPreferencePanelDirListItem::isEqual(const QVariant &currentValue,
                                           const QVariant &originalValue)
{
    return true;
}

inline KNLabelButton *KNPreferencePanelDirListItem::generateButton(
        const QString &iconPath, const QString &tooltip)
{
    //Generate the button.
    KNLabelButton *button=new KNLabelButton(this);
    //Set the properties of the button.
    button->setPixmap(QPixmap(iconPath));
    button->setToolTip(tooltip);
    //Give back the button.
    return button;
}
