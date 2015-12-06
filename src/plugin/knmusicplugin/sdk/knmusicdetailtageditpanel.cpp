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
#include <QLabel>
#include <QFormLayout>

#include "knlabellineedit.h"
#include "kncircleiconbutton.h"
#include "knlocalemanager.h"

#include "knmusicdetailtageditpanel.h"

KNMusicDetailTagEditPanel::KNMusicDetailTagEditPanel(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_button(new KNCircleIconButton(this))
{
    //Initial the layout.
    QFormLayout *mainLayout=new QFormLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(9);
    //Set the layout.
    setLayout(mainLayout);
    //Initial the label palette.
    QPalette captionPal=palette(), contentPal=palette();
    captionPal.setColor(QPalette::WindowText, QColor(0xa0, 0xa0, 0xa0));
    contentPal.setColor(QPalette::WindowText, QColor(72, 72, 72));
    //Add editor into the layout.
    for(int i=0; i<DetailRowsCount; ++i)
    {
        //Generate the label.
        m_rowLabel[i]=new QLabel(this);
        //Set the label.
        m_rowLabel[i]->setPalette(captionPal);
        //Generate the widget.
        m_rowEditor[i]=generateLineEdit();
        //Insert the widget.
        mainLayout->addRow(m_rowLabel[i], m_rowEditor[i]);
    }
    //Configure the button.
    m_button->setIcon(QIcon(":/plugin/music/detaildialog/tab_icon/tag.png"));
    //Link with the locale manager.
    knI18n->link(this, &KNMusicDetailTagEditPanel::retranslate);
    retranslate();
}

QAbstractButton *KNMusicDetailTagEditPanel::tabButton()
{
    return m_button;
}

void KNMusicDetailTagEditPanel::setAnalysisItem(
        const MusicUtil::KNMusicAnalysisItem &item)
{
    ;
}

void KNMusicDetailTagEditPanel::retranslate()
{
    //Update the label.
    m_rowLabel[NameRow]->setText(tr("Song Name"));
    ;
}

inline KNLabelLineEdit *KNMusicDetailTagEditPanel::generateLineEdit()
{
    //Generate a new line edit.
    KNLabelLineEdit *lineEdit=new KNLabelLineEdit(this);
    //Configure the line edit.
    lineEdit->setMinimumLightness(0xC0);
    lineEdit->setMediumLightness(0xE0);
    //Give back the line edit.
    return lineEdit;
}
