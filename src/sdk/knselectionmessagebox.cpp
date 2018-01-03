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
#include <QButtonGroup>
#include <QScopedPointer>
#include <QLabel>

#include "sao/knsaooptionbutton.h"
#include "kndpimanager.h"

#include "knselectionmessagebox.h"

#include <QDebug>

KNSelectionMessageBox::KNSelectionMessageBox(const QStringList &options,
                                             QWidget *parent) :
    KNMessageBox(parent),
    m_hintText(new QLabel(this)),
    m_optionGroup(new QButtonGroup(this)),
    m_selection(-1)
{
    //Set properties.
    setCancelButtonVisible(true);
    setOkayButtonVisible(false);
    //Configure the hint text.
    m_hintText->setAlignment(Qt::AlignCenter);
    //Configure the group.
    connect(m_optionGroup, static_cast<void(QButtonGroup::*)(int)>(
                &QButtonGroup::buttonClicked),
            [=](int buttonIndex)
            {
                //Check the button clicked index.
                setButtonVisible(true, buttonIndex>-1);
            });

    //Construct the container.
    QWidget *container=new QWidget(this);
    //Initial the layout of the container.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          container);
    mainLayout->setContentsMargins(knDpi->margins(20, 10, 20, 10));
    //Set the main layout to container.
    container->setLayout(mainLayout);
    //Add widgets to layout.
    mainLayout->addWidget(m_hintText);
    //Construct widget buttons for the options.
    QBoxLayout *optionLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    optionLayout->setContentsMargins(0, 0, 0, 0);
    optionLayout->setSpacing(1);
    mainLayout->addLayout(optionLayout);
    //Construct buttons.
    for(int i=0; i<options.size(); ++i)
    {
        //Generate the button.
        KNSaoOptionButton *button=new KNSaoOptionButton(this);
        //Set the text.
        button->setText(options.at(i));
        //Add to button group.
        m_optionGroup->addButton(button, i);
        //Add button to layout.
        optionLayout->addWidget(button);
    }
    //Set the content widget to message box.
    setContentWidget(container);
}

int KNSelectionMessageBox::getSelection(const QString &title,
                                        const QString &hintText,
                                        const QStringList &options)
{
    //Check options list.
    if(options.isEmpty())
    {
        //Invalid options list.
        return -1;
    }
    //Generate a temporary option message box.
    QScopedPointer<KNSelectionMessageBox> messageBox(
                new KNSelectionMessageBox(options));
    //Configure the message box.
    messageBox->setTitleText(title);
    messageBox->setHintText(hintText);
    //Show the message box.
    messageBox->exec();
    //Get the message box selection data.
    return messageBox->selectedIndex();
}

int KNSelectionMessageBox::selectedIndex() const
{
    return m_selection;
}

void KNSelectionMessageBox::setHintText(const QString &hintText)
{
    //Set the hint text.
    m_hintText->setText(hintText);
}

bool KNSelectionMessageBox::okayPressed()
{
    //Save the button index to the selection index.
    m_selection=m_optionGroup->checkedId();
    return true;
}

void KNSelectionMessageBox::cancelPressed()
{
    //Reset the selection index to -1.
    m_selection=-1;
}
