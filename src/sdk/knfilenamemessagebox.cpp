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
#include <QLabel>

#include "knfilenamelineedit.h"

#include "knfilenamemessagebox.h"

KNFileNameMessageBox::KNFileNameMessageBox(QWidget *parent) :
    KNMessageBox(parent),
    m_hintText(new QLabel(this)),
    m_lineEdit(new KNFileNameLineEdit(this)),
    m_newFileName(QString())
{
    //Configure the line edit.
    m_lineEdit->setMinimumLightness(0xC0);
    m_lineEdit->setMediumLightness(0xE0);
    //Link the line edit signal.
    connect(m_lineEdit, &KNFileNameLineEdit::stateChanged,
            this, &KNFileNameMessageBox::onActionStateChanged);
    //Configure the hint text.
    m_hintText->setAlignment(Qt::AlignCenter);

    //Initial the content widget.
    QWidget *container=new QWidget(this);
    //Set the content widget to message box.
    setContentWidget(container);
    //Initial the layout of the container.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          container);
    //Set the main layout to container.
    container->setLayout(mainLayout);
    //Add the hint text to main layout.
    mainLayout->addWidget(m_hintText);
    //Add line edit to main layout.
    mainLayout->addWidget(m_lineEdit);
}

void KNFileNameMessageBox::setFilePath(const QString &filePath)
{
    //Set the file path to line edit.
    m_lineEdit->setFilePath(filePath);
}

bool KNFileNameMessageBox::okayPressed()
{
    //Check out the line edit state, if it's valid then it could be return true.
    if(m_lineEdit->state()==KNFileNameLineEdit::Valid)
    {
        //Save the line edit string.
        m_newFileName=m_lineEdit->text();
    }
}

void KNFileNameMessageBox::cancelPressed()
{
    //Reset the new file name.
    m_newFileName.clear();
}

void KNFileNameMessageBox::onActionStateChanged(int state)
{
    //Check out the state.
    //Change the okay button shown state.
    setOkayButtonVisible(state==KNFileNameLineEdit::Valid);
}
