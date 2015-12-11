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
#include <QBoxLayout>
#include <QComboBox>

#include "knlabellineedit.h"
#include "kncircleiconbutton.h"
#include "knlocalemanager.h"

#include "knmusicglobal.h"

#include "knmusicdetailtageditpanel.h"

KNMusicDetailTagEditPanel::KNMusicDetailTagEditPanel(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_yearEditor(generateLineEdit()),
    m_bpmEditor(generateLineEdit()),
    m_trackEditorOf(new QLabel(this)),
    m_discEditorOf(new QLabel(this)),
    m_genreEditor(new QComboBox(this)),
    m_button(new KNCircleIconButton(this))
{
    //Configure the genre editor.
    m_genreEditor->setEditable(true);
    m_genreEditor->addItems(knMusicGlobal->genreList());
    //Configure the button.
    m_button->setIcon(QIcon(":/plugin/music/detaildialog/tab_icon/tag.png"));

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(8);
    setLayout(mainLayout);
    //Initial the basic information layout.
    QFormLayout *basicLayout=new QFormLayout(mainLayout->widget());
    basicLayout->setContentsMargins(0,0,0,0);
    basicLayout->setSpacing(8);
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
    doubleLayout->setSpacing(8);
    mainLayout->addLayout(doubleLayout);
    //Initial the left column layout.
    QFormLayout *leftLayout=new QFormLayout(mainLayout->widget());
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(8);
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
    leftLayout->addRow(m_leftRowLabel[RatingRow]);
    leftLayout->addRow(m_leftRowLabel[BpmRow], m_bpmEditor);
    //Initial the right column layout.
    QFormLayout *rightLayout=new QFormLayout(mainLayout->widget());
    rightLayout->setContentsMargins(0,0,0,0);
    rightLayout->setSpacing(8);
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
        m_trackEditor[i]->setMaximumWidth(25);
        m_discEditor[i]->setMaximumWidth(25);
    }
    // Generate layouts.
    QBoxLayout *trackLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           mainLayout->widget());
    trackLayout->setContentsMargins(0,0,0,0);
    trackLayout->setSpacing(8);
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
    ;


    //Link with the locale manager.
    knI18n->link(this, &KNMusicDetailTagEditPanel::retranslate);
    retranslate();
}

QAbstractButton *KNMusicDetailTagEditPanel::tabButton()
{
    return m_button;
}

void KNMusicDetailTagEditPanel::setAnalysisItem(const KNMusicAnalysisItem &item)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=item.detailInfo;
    //Set the item deta to editors.
    m_rowEditor[NameRow]->setText(detailInfo.textLists[Name].toString());
    m_rowEditor[ArtistRow]->setText(detailInfo.textLists[Artist].toString());
    m_rowEditor[AlbumRow]->setText(detailInfo.textLists[Album].toString());
    m_rowEditor[AlbumArtistRow]->setText(
                detailInfo.textLists[AlbumArtist].toString());
    m_rowEditor[ComposerRow]->setText(
                detailInfo.textLists[Composer].toString());

    m_genreEditor->setEditText(detailInfo.textLists[Genre].toString());
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
        //Set it to be the maximum label width.
        m_rowLabel[i]->setFixedWidth(maximumLabelWidth);
    }
    for(int i=0; i<LeftColumnRowCount; ++i)
    {
        m_leftRowLabel[i]->setFixedWidth(maximumLabelWidth);
    }

    m_trackEditorOf->setText(tr("of"));
    m_discEditorOf->setText(tr("of"));
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
