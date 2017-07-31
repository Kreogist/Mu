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
#include <QBoxLayout>
#include <QLabel>

#include "knhtabgroup.h"
#include "knhwidgetswitcher.h"
#include "kndpimanager.h"

#include "knmusicparser.h"
#include "knmusicproxymodel.h"
#include "knmusicmodel.h"
#include "knmusicdetaildialogpanel.h"
#include "knmusicdetailtageditpanel.h"

#include "knmusicdetaildialog.h"

#include <QDebug>

#define AlbumArtSize 100

using namespace MusicUtil;

KNMusicDetailDialog::KNMusicDetailDialog(QWidget *parent) :
    KNMessageBox(parent),
    m_panelList(QLinkedList<KNMusicDetailDialogPanel *>()),
    m_proxyIndex(QModelIndex()),
    m_proxyModel(nullptr),
    m_panelSwitcher(new KNHTabGroup(this)),
    m_panelContainer(new KNHWidgetSwitcher(this)),
    m_tagEditPanel(nullptr)
{
    //Set properties.
    setTitleText("Information");
    //Configure the panel switcher.
    m_panelSwitcher->setFixedHeight(knDpi->height(25));

    //Link the switcher and container.
    connect(m_panelSwitcher, &KNHTabGroup::currentIndexChange,
            m_panelContainer, &KNHWidgetSwitcher::setCurrentIndex);

    //Generate basic label palette.
    QPalette labelPal=palette();
    labelPal.setColor(QPalette::WindowText, QColor(72, 72, 72));
    labelPal.setColor(QPalette::Highlight, QColor(0xa0, 0xa0, 0xa0));
    labelPal.setColor(QPalette::HighlightedText, QColor(72, 72, 72));
    //Initial the basic information labels
    for(int i=0; i<BasicInformationCount; i++)
    {
        //Simply initial the label.
        m_basicInfoLabel[i]=new QLabel(this);
        //Set properties.
        m_basicInfoLabel[i]->setTextInteractionFlags(
                    Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        m_basicInfoLabel[i]->setFixedWidth(knDpi->width(256));
        //Set the palette.
        m_basicInfoLabel[i]->setPalette(labelPal);
    }
    //Configure the basic information labels.
    m_basicInfoLabel[LabelAlbumArt]->setFixedSize(
                knDpi->size(AlbumArtSize, AlbumArtSize));
    m_basicInfoLabel[LabelAlbumArt]->setAlignment(Qt::AlignCenter);

    //Initial the widget container.
    QWidget *widgetContainer=new QWidget(this);
    widgetContainer->setFixedSize(knDpi->size(400, 465));
    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          widgetContainer);
    mainLayout->setContentsMargins(knDpi->margins(17,12,17,0));
    mainLayout->setSpacing(knDpi->height(9));
    widgetContainer->setLayout(mainLayout);
    //Initial the basic information layout.
    QBoxLayout *basicInfoLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                               mainLayout->widget());
    basicInfoLayout->setContentsMargins(0,0,0,0);
    basicInfoLayout->setSpacing(knDpi->width(6));
    mainLayout->addLayout(basicInfoLayout);
    //Add widget to basic information layout.
    basicInfoLayout->addWidget(m_basicInfoLabel[LabelAlbumArt]);
    //Initial the basic text information layout.
    QBoxLayout *basicTextInfoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                   basicInfoLayout->widget());
    basicTextInfoLayout->setContentsMargins(0,0,0,0);
    basicTextInfoLayout->setSpacing(knDpi->width(6));
    basicInfoLayout->addLayout(basicTextInfoLayout);
    //Add widgets to basic text information layout.
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelName]);
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelDuration]);
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelArtist]);
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelAlbum]);
    basicTextInfoLayout->addStretch();
    //Add widget to main layout.
    mainLayout->addWidget(m_panelSwitcher);
    mainLayout->addWidget(m_panelContainer, 1);
    //Set the detail widget container as the content.
    setContentWidget(widgetContainer);
}

void KNMusicDetailDialog::addPanel(KNMusicDetailDialogPanel *panel)
{
    //Link the panel to detail panel.
    connect(panel, &KNMusicDetailDialogPanel::requireUpdateFileInfo,
            this, &KNMusicDetailDialog::onActionUpdateFileInfo);
    //Add panel to the panel container.
    m_panelContainer->addWidget(panel);
    //Add the switcher button to switcher.
    m_panelSwitcher->addTab(panel->tabButton());
    //Save the panel to panel list.
    m_panelList.append(panel);
}

void KNMusicDetailDialog::showDialog(KNMusicAnalysisItem analysisItem,
                                     KNMusicProxyModel *proxyModel,
                                     const QModelIndex &proxyIndex)
{
    //Save the proxy model and proxy index.
    m_proxyIndex=proxyIndex;
    m_proxyModel=proxyModel;
    //Get the parser.
    KNMusicParser *parser=knMusicGlobal->parser();
    //Reanalysis the analysis item.
    parser->reanalysisItem(analysisItem);

    //Update the analysis item information.
    updateAnalysisItem(analysisItem);
    //Reset the current button to the first one, it should be the overview tab.
    m_panelSwitcher->setCurrentIndex(0);
    //Show the dialog.
    exec();
}

void KNMusicDetailDialog::onActionUpdateFileInfo()
{
    //Reload the detail info from the proxy model and current item.
    if(m_proxyModel==nullptr || !m_proxyIndex.isValid())
    {
        //Ignore the process.
        return;
    }
    //Get the raw detail info.
    KNMusicDetailInfo &&originalDetailInfo=
            m_proxyModel->rowDetailInfo(m_proxyIndex.row());
    //Generate the analysis item.
    KNMusicAnalysisItem currentItem;
    //Get the detail info.
    currentItem.detailInfo.filePath=originalDetailInfo.filePath;
    currentItem.detailInfo.trackFilePath=originalDetailInfo.trackFilePath;
    currentItem.detailInfo.trackIndex=originalDetailInfo.trackIndex;
    //Update the current item.
    KNMusicParser *parser=knMusicGlobal->parser();
    //Check out the parser pointer.
    if(!parser)
    {
        //Ignore the parser.
        return;
    }
    //Reanalysis the parser.
    parser->reanalysisItem(currentItem);
    //Update the model.
    if(m_proxyModel->musicModel())
    {
        //Update the music model.
        m_proxyModel->musicModel()->updateRow(
                    m_proxyModel->mapToSource(m_proxyIndex).row(),
                    currentItem);
    }
    //Update the analysis item.
    updateAnalysisItem(currentItem);
}

inline void KNMusicDetailDialog::updateAnalysisItem(
        const KNMusicAnalysisItem &analysisItem)
{
    //Check whether the album art is null.
    m_basicInfoLabel[LabelAlbumArt]->setPixmap(
                ((analysisItem.coverImage.isNull())?
                    knMusicGlobal->noAlbumArt():
                    QPixmap::fromImage(analysisItem.coverImage)).scaled(
                    knDpi->size(AlbumArtSize, AlbumArtSize),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
    //Get the detail information.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Set the text basic information.
    m_basicInfoLabel[LabelName]->setText(detailInfo.textLists[Name].toString());
    m_basicInfoLabel[LabelDuration]->setText(
                detailInfo.textLists[Time].toString());
    m_basicInfoLabel[LabelArtist]->setText(
                detailInfo.textLists[Artist].toString());
    m_basicInfoLabel[LabelAlbum]->setText(
                detailInfo.textLists[Album].toString());

    //Ask all the panel to update the information.
    for(auto i : m_panelList)
    {
        //Update the panel information.
        //Set the model and item index to the tag.
        i->setAnalysisItem(analysisItem, m_proxyModel, m_proxyIndex);
    }
}

KNMusicDetailTagEditPanel *KNMusicDetailDialog::tagEditPanel() const
{
    return m_tagEditPanel;
}

void KNMusicDetailDialog::addTagEditPanel(
        KNMusicDetailTagEditPanel *tagEditPanel)
{
    //Save the tag edit panel.
    m_tagEditPanel = tagEditPanel;
    //Add the panel to the detail dialog.
    addPanel(m_tagEditPanel);
}
