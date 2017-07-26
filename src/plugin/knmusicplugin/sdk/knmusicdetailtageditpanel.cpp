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
#include <QAbstractItemView>
#include <QBoxLayout>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

#include "knlabellineedit.h"
#include "kncircleiconbutton.h"
#include "knlocalemanager.h"
#include "kndpimanager.h"

#include "knmusicparser.h"
#include "knmusicratingeditor.h"
#include "knmusicglobal.h"

#include "knmusicdetailtageditpanel.h"

KNMusicDetailTagEditPanel::KNMusicDetailTagEditPanel(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_proxyIndex(QModelIndex()),
    m_proxyModel(nullptr),
    m_yearEditor(generateLineEdit()),
    m_bpmEditor(generateLineEdit()),
    m_trackEditorOf(new QLabel(this)),
    m_discEditorOf(new QLabel(this)),
    m_genreEditor(new QComboBox(this)),
    m_ratingEditor(new KNMusicRatingEditor(this)),
    m_button(new KNCircleIconButton(this)),
    m_writeTag(new QPushButton(this))
{
    //Configure the genre editor.
    m_genreEditor->setEditable(true);
    m_genreEditor->addItems(knMusicGlobal->genreList());
    connect(m_genreEditor, &QComboBox::currentTextChanged,
            this, &KNMusicDetailTagEditPanel::onActionItemChanged);
    QPalette pal=m_genreEditor->palette();
    pal.setColor(QPalette::Highlight, QColor(192, 192, 192));
    m_genreEditor->setPalette(pal);
    pal=m_genreEditor->view()->palette();
    pal.setColor(QPalette::HighlightedText, QColor(0,0,0));
    m_genreEditor->view()->setPalette(pal);
    //Configure the button.
    m_button->setIcon(QIcon(":/plugin/music/detaildialog/tab_icon/tag.png"));
    //Configure the rating editor.
    m_ratingEditor->setStarSizeHint(knDpi->height(20));
    m_ratingEditor->setDetectOnMove(false);
    connect(m_ratingEditor, &KNMusicRatingEditor::editingFinished,
            this, &KNMusicDetailTagEditPanel::onActionItemChanged);
    pal=m_ratingEditor->palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    m_ratingEditor->setPalette(pal);
    //Configure the write tag button.
    connect(m_writeTag, &QPushButton::clicked,
            this, &KNMusicDetailTagEditPanel::onActionWriteTag);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(knDpi->width(8));
    setLayout(mainLayout);
    //Initial the basic information layout.
    QFormLayout *basicLayout=new QFormLayout(mainLayout->widget());
    basicLayout->setContentsMargins(0,0,0,0);
    basicLayout->setSpacing(knDpi->width(8));
    basicLayout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignRight);
    //Set the layout.
    mainLayout->addLayout(basicLayout);
    //Initial the label palette.
    QPalette captionPal=palette(), contentPal=palette();
    captionPal.setColor(QPalette::WindowText, QColor(0xa0, 0xa0, 0xa0));
    contentPal.setColor(QPalette::WindowText, QColor(72, 72, 72));
    //Add editor into the layout.
    for(int i=0; i<DetailRowCount; ++i)
    {
        //Generate the label.
        m_rowLabel[i]=new QLabel(this);
        //Configure the label.
        m_rowLabel[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        m_rowLabel[i]->setPalette(captionPal);
        //Generate the widget.
        m_rowEditor[i]=generateLineEdit();
        //Insert the widget.
        basicLayout->addRow(m_rowLabel[i], m_rowEditor[i]);
    }
    //Initial the double column layout.
    QBoxLayout *doubleLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    doubleLayout->setContentsMargins(0,0,0,0);
    doubleLayout->setSpacing(knDpi->width(8));
    mainLayout->addLayout(doubleLayout);
    //Initial the left column layout.
    QFormLayout *leftLayout=new QFormLayout(mainLayout->widget());
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(knDpi->width(8));
    leftLayout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignRight);
    doubleLayout->addLayout(leftLayout, 1);
    //Add editor to left column.
    for(int i=0; i<LeftColumnRowCount; ++i)
    {
        //Generate the label.
        m_leftRowLabel[i]=new QLabel(this);
        //Configure the label.
        m_leftRowLabel[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        m_leftRowLabel[i]->setPalette(captionPal);
    }
    //Generate different widgets for the left column.
    //Add to left column layout.
    leftLayout->addRow(m_leftRowLabel[GenreRow], m_genreEditor);
    leftLayout->addRow(m_leftRowLabel[RatingRow], m_ratingEditor);
    leftLayout->addRow(m_leftRowLabel[BpmRow], m_bpmEditor);
    //Initial the right column layout.
    QFormLayout *rightLayout=new QFormLayout(mainLayout->widget());
    rightLayout->setContentsMargins(0,0,0,0);
    rightLayout->setSpacing(knDpi->width(8));
    rightLayout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignRight);
    doubleLayout->addLayout(rightLayout);
    //Add editor to right column.
    for(int i=0; i<RightColumnRowCount; ++i)
    {
        //Generate the label.
        m_rightRowLabel[i]=new QLabel(this);
        //Set the label.
        m_rightRowLabel[i]->setPalette(captionPal);
    }
    //Generate different widgets for the right column.
    // Generate widgets.
    for(int i=0; i<2; ++i)
    {
        //Generate editor.
        m_trackEditor[i]=generateLineEdit();
        m_discEditor[i]=generateLineEdit();
        //Configure editor.
        m_trackEditor[i]->setMaximumWidth(knDpi->width(25));
        m_discEditor[i]->setMaximumWidth(knDpi->width(25));
    }
    // Generate layouts.
    QBoxLayout *trackLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           mainLayout->widget());
    trackLayout->setContentsMargins(0,0,0,0);
    trackLayout->setSpacing(knDpi->width(8));
    trackLayout->addWidget(m_trackEditor[0]);
    trackLayout->addWidget(m_trackEditorOf);
    trackLayout->addWidget(m_trackEditor[1]);
    trackLayout->addStretch(1);
    QBoxLayout *discLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          mainLayout->widget());
    discLayout->setContentsMargins(0,0,0,0);
    discLayout->setSpacing(8);
    discLayout->addWidget(m_discEditor[0]);
    discLayout->addWidget(m_discEditorOf);
    discLayout->addWidget(m_discEditor[1]);
    discLayout->addStretch(1);
    //Add to right column layout.
    rightLayout->addRow(m_rightRowLabel[YearRow], m_yearEditor);
    rightLayout->addRow(m_rightRowLabel[TrackRow], trackLayout);
    rightLayout->addRow(m_rightRowLabel[DiscRow], discLayout);
    //Add edit button and control buttons.
    mainLayout->addStretch();
    //Insert the push button to the main layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    //Add button layout to main layout.
    mainLayout->addLayout(buttonLayout);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_writeTag);

    //Link with the locale manager.
    knI18n->link(this, &KNMusicDetailTagEditPanel::retranslate);
    retranslate();
}

QAbstractButton *KNMusicDetailTagEditPanel::tabButton()
{
    return m_button;
}

void KNMusicDetailTagEditPanel::setAnalysisItem(const KNMusicAnalysisItem &item,
                                                KNMusicProxyModel *proxyModel,
                                                const QModelIndex &proxyIndex)
{
    //Save the item.
    m_analysisItem=item;
    //Save the proxy model and index.
    m_proxyModel=proxyModel;
    m_proxyIndex=proxyIndex;
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=m_analysisItem.detailInfo;
    //Check out the item track list file.
    m_button->setVisible(detailInfo.trackFilePath.isEmpty());
    //Set the item deta to editors.
    m_rowEditor[NameRow]->setText(detailInfo.textLists[Name].toString());
    m_rowEditor[ArtistRow]->setText(detailInfo.textLists[Artist].toString());
    m_rowEditor[AlbumRow]->setText(detailInfo.textLists[Album].toString());
    m_rowEditor[AlbumArtistRow]->setText(
                detailInfo.textLists[AlbumArtist].toString());
    m_rowEditor[ComposerRow]->setText(
                detailInfo.textLists[Composer].toString());

    m_genreEditor->setEditText(detailInfo.textLists[Genre].toString());
    m_ratingEditor->setStarNum(detailInfo.textLists[Rating].toString().toInt());
    m_bpmEditor->setText(detailInfo.textLists[BeatsPerMinuate].toString());
    m_yearEditor->setText(detailInfo.textLists[Year].toString());
    m_trackEditor[0]->setText(detailInfo.textLists[TrackNumber].toString());
    m_trackEditor[1]->setText(detailInfo.textLists[TrackCount].toString());
    m_discEditor[0]->setText(detailInfo.textLists[DiscNumber].toString());
    m_discEditor[1]->setText(detailInfo.textLists[DiscCount].toString());
    //Move all the editor to the beginning of the text editor.
    for(int i=0; i<DetailRowCount; ++i)
    {
        //The beginning is 0.
        m_rowEditor[i]->setCursorPosition(0);
    }
    //Hide the write tag button.
    m_writeTag->setVisible(false);
}

void KNMusicDetailTagEditPanel::retranslate()
{
    //Update the label.
    m_rowLabel[NameRow]->setText(tr("Song Name"));
    m_rowLabel[ArtistRow]->setText(tr("Artist"));
    m_rowLabel[AlbumRow]->setText(tr("Album"));
    m_rowLabel[AlbumArtistRow]->setText(tr("Album Artist"));
    m_rowLabel[ComposerRow]->setText(tr("Composer"));
    m_rowLabel[CommentRow]->setText(tr("Comments"));
    m_leftRowLabel[GenreRow]->setText(tr("Genre"));
    m_leftRowLabel[RatingRow]->setText(tr("Rating"));
    m_leftRowLabel[BpmRow]->setText(tr("BPM"));
    m_rightRowLabel[YearRow]->setText(tr("Year"));
    m_rightRowLabel[TrackRow]->setText(tr("Track"));
    m_rightRowLabel[DiscRow]->setText(tr("Disc Number"));
    //Initial the maximum width.
    int maximumLabelWidth=0;
    //Find the maximum size of the row labels.
    for(int i=0; i<DetailRowCount; ++i)
    {
        //Find the maximum width of the label.
        maximumLabelWidth=qMax(maximumLabelWidth, m_rowLabel[i]->width());
    }
    //Find the maximum size of the left labels.
    for(int i=0; i<LeftColumnRowCount; ++i)
    {
        //Find the maximum width of the label.
        maximumLabelWidth=qMax(maximumLabelWidth, m_leftRowLabel[i]->width());
    }
    //Update the left row label width.
    for(int i=0; i<DetailRowCount; ++i)
    {
        //Set it to be the fixed label width.
        m_rowLabel[i]->setFixedWidth(maximumLabelWidth);
    }
    for(int i=0; i<LeftColumnRowCount; ++i)
    {
        m_leftRowLabel[i]->setFixedWidth(maximumLabelWidth);
    }
    //Update the tags.
    m_trackEditorOf->setText(tr("of"));
    m_discEditorOf->setText(tr("of"));

    //Update the write tag button.
    m_writeTag->setText(tr("Save"));
}

void KNMusicDetailTagEditPanel::onActionItemChanged()
{
    //Show the write tag button.
    m_writeTag->setVisible(true);
}

void KNMusicDetailTagEditPanel::onActionWriteTag()
{
    //Get the tag parser.
    KNMusicParser *parser=knMusicGlobal->parser();
    //Check is null or not.
    if(!parser)
    {
        //Ignore the write state.
        return;
    }
    //Generate a new write tag information.
    //Get the detail info from the analysis item.
    KNMusicDetailInfo &detailInfo=m_analysisItem.detailInfo;
    //Update the detail info.
    detailInfo.textLists[Name]=m_rowEditor[NameRow]->text();
    detailInfo.textLists[Artist]=m_rowEditor[ArtistRow]->text();
    detailInfo.textLists[Album]=m_rowEditor[AlbumRow]->text();
    detailInfo.textLists[AlbumArtist]=m_rowEditor[AlbumArtistRow]->text();
    detailInfo.textLists[Composer]=m_rowEditor[ComposerRow]->text();
    detailInfo.textLists[Genre]=m_genreEditor->currentText();
    detailInfo.textLists[Rating]=QString::number(m_ratingEditor->starNum());
    detailInfo.textLists[BeatsPerMinuate]=m_bpmEditor->text();
    detailInfo.textLists[Year]=m_yearEditor->text();
    detailInfo.textLists[TrackNumber]=m_trackEditor[0]->text();
    detailInfo.textLists[TrackCount]=m_trackEditor[1]->text();
    detailInfo.textLists[DiscNumber]=m_discEditor[0]->text();
    detailInfo.textLists[DiscCount]=m_discEditor[1]->text();
    //Okay, write this detail info, using the tag parser to write the tag.
    if(!parser->writeAnalysisItem(m_analysisItem))
    {
        //We cannot write the tag. Simply return.
        return;
    }
    //We have to update the file information.
    emit requireUpdateFileInfo();
    //Hide the write tag button.
    m_writeTag->hide();
}

inline KNLabelLineEdit *KNMusicDetailTagEditPanel::generateLineEdit()
{
    //Generate a new line edit.
    KNLabelLineEdit *lineEdit=new KNLabelLineEdit(this);
    //Configure the line edit.
    lineEdit->setMinimumLightness(0xC0);
    lineEdit->setMediumLightness(0xE0);
    //Link with the item changed slot.
    connect(lineEdit, &KNLabelLineEdit::textChanged,
            this, &KNMusicDetailTagEditPanel::onActionItemChanged);
    //Give back the line edit.
    return lineEdit;
}
