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

#include "knutil.h"
#include "knlocalemanager.h"
#include "knlabelbutton.h"
#include "kncircleiconbutton.h"

#include "knmusicdetailpaneloverview.h"

using namespace MusicUtil;

KNMusicDetailPanelOverview::KNMusicDetailPanelOverview(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_pathCaption(new QLabel(this)),
    m_pathInformation(new KNLabelButton(this)),
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

    //Initial the label palette.
    QPalette captionPal=palette(), contentPal=palette();
    captionPal.setColor(QPalette::WindowText, QColor(0xa0, 0xa0, 0xa0));
    contentPal.setColor(QPalette::WindowText, QColor(72, 72, 72));
    //Configure the path information.
    m_pathInformation->setWordWrap(true);
    m_pathInformation->setCursor(Qt::PointingHandCursor);
    connect(m_pathInformation, &KNLabelButton::clicked,
            this, &KNMusicDetailPanelOverview::onActionPathClicked);
    //Configure the path caption.
    m_pathCaption->setAlignment(Qt::AlignRight | Qt::AlignTop);
    m_pathCaption->setMinimumWidth(100);
    m_pathCaption->setPalette(captionPal);
    //Initial the labels.
    for(int i=0; i<DetailInformationCount; i++)
    {
        //Construct caption label.
        m_caption[i]=new QLabel(this);
        //Configure caption.
        m_caption[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_caption[i]->setMinimumWidth(100);
        m_caption[i]->setPalette(captionPal);
        //Construct detail label.
        m_information[i]=new QLabel(this);
        m_information[i]->setPalette(contentPal);

        //Add label to layout.
        mainLayout->addRow(m_caption[i], m_information[i]);
    }
    //Add path information to layout.
    mainLayout->addRow(m_pathCaption, m_pathInformation);

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
    m_information[DetailYear]->setText(detailInfo.textLists[Year].toString());
    m_information[DetailGenre]->setText(detailInfo.textLists[Genre].toString());
    m_information[DetailKind]->setText(detailInfo.textLists[Kind].toString());
    m_information[DetailSize]->setText(detailInfo.textLists[Size].toString());
    m_information[DetailBitRate]->setText(
                detailInfo.textLists[BitRate].toString());
    m_information[DetailSampleRate]->setText(
                detailInfo.textLists[SampleRate].toString());
    m_information[DetailDateModified]->setText(
                detailInfo.textLists[DateModified].toString());
    //Set the file path.
    m_pathInformation->setText(detailInfo.filePath);
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
    m_pathCaption->setText(tr("Path"));
}

void KNMusicDetailPanelOverview::onActionPathClicked()
{
    //Show the file in graphics shell.
    KNUtil::showInGraphicalShell(m_pathInformation->text());
}
