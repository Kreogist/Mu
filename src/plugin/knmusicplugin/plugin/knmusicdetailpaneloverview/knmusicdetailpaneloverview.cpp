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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QFormLayout>
#include <QLabel>

#include "knlocalemanager.h"
#include "kncircleiconbutton.h"

#include "knmusicdetailpaneloverview.h"

using namespace MusicUtil;

KNMusicDetailPanelOverview::KNMusicDetailPanelOverview(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_button(new KNCircleIconButton(this))
{
    //Configure the button.
    m_button->setIcon(QIcon(":/plugin/music/detaildialog/tab_icon/detail.png"));

    //Initial the layout.
    QFormLayout *mainLayout=new QFormLayout(this);
    mainLayout->setVerticalSpacing(9);
    mainLayout->setLabelAlignment(Qt::AlignRight);
    mainLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    mainLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    setLayout(mainLayout);

    //Initial the labels.
    for(int i=0; i<DetailInformationCount; i++)
    {
        //Construct caption label.
        m_caption[i]=new QLabel(this);
        //Configure caption.
        m_caption[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_caption[i]->setMinimumWidth(100);
        //Construct detail label.
        m_information[i]=new QLabel(this);

        //Add label to layout.
        mainLayout->addRow(m_caption[i], m_information[i]);
    }

    //Link retranslate.
    knI18n->link(this, &KNMusicDetailPanelOverview::retranslate);
    retranslate();
}

QAbstractButton *KNMusicDetailPanelOverview::tabButton()
{
    return m_button;
}

void KNMusicDetailPanelOverview::setAnalysisItem(
        const MusicUtil::KNMusicAnalysisItem &item)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=item.detailInfo;
    //Set the analysis item to the label.
    m_information[DetailYear]->setText(detailInfo.textLists[Year]);
    m_information[DetailGenre]->setText(detailInfo.textLists[Genre]);
    m_information[DetailKind]->setText(detailInfo.textLists[Kind]);
    m_information[DetailSize]->setText(detailInfo.textLists[Size]);
    m_information[DetailBitRate]->setText(detailInfo.textLists[BitRate]);
    m_information[DetailSampleRate]->setText(detailInfo.textLists[SampleRate]);
    m_information[DetailDateModified]->setText(
                detailInfo.textLists[DateModified]);
}

void KNMusicDetailPanelOverview::retranslate()
{
    //Update the captions.
    m_caption[DetailYear]->setText(tr("Year"));
    m_caption[DetailGenre]->setText(tr("Genre"));
    m_caption[DetailKind]->setText(tr("Kind"));
    m_caption[DetailSize]->setText(tr("Size"));
    m_caption[DetailBitRate]->setText(tr("Bit Rate"));
    m_caption[DetailSampleRate]->setText(tr("Sample Rate"));
    m_caption[DetailDateModified]->setText(tr("Date Modified"));
}

