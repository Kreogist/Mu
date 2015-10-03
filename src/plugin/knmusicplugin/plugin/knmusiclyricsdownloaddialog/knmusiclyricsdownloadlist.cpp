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
#include <QListView>
#include <QScrollBar>

#include "sao/knsaostyle.h"
#include "knclockwheel.h"
#include "knlocalemanager.h"
#include "knthememanager.h"
#include "knopacitybutton.h"
#include "knprogressslider.h"

#include "knmusicglobal.h"
#include "knmusicbackend.h"
#include "knmusiclrcparser.h"
#include "knmusiclyricsbackend.h"
#include "knmusicscrolllyrics.h"
#include "knmusiclyricsdetaillistmodel.h"

#include "knmusiclyricsdownloadlist.h"

#include <QDebug>

KNMusicLyricsDownloadList::KNMusicLyricsDownloadList(QWidget *parent) :
    QWidget(parent),
    m_playIcon(QIcon(":/plugin/music/player/play_light.png")),
    m_pauseIcon(QIcon(":/plugin/music/player/pause_light.png")),
    m_container(new QWidget(this)),
    m_previewPlayer(new QWidget(this)),
    m_clockWheel(new KNClockWheel(this)),
    m_downloadServer(new QLabel(this)),
    m_lyricsList(new QListView(this)),
    m_lrcParser(new KNMusicLrcParser(this)),
    m_lyricsDetailListModel(new KNMusicLyricsDetailListModel(this)),
    m_previewBackend(new KNMusicLyricsBackend(this)),
    m_scrollLyrics(new KNMusicScrollLyrics(this)),
    m_playNPause(new KNOpacityButton(this)),
    m_progress(new KNProgressSlider(this)),
    m_progressPressed(false)
{
    //Configure the clock wheel.
    m_clockWheel->setFixedSize(36, 36);
    m_clockWheel->hide();
    //Configure the label.
    m_downloadServer->hide();
    //Configure the list view.
    m_lyricsList->setObjectName("DownloadedLyricsListView");
    m_lyricsList->hide();
    m_lyricsList->setAutoFillBackground(true);
    m_lyricsList->setContentsMargins(0, 0, 0, 0);
    m_lyricsList->setUniformItemSizes(true);
    m_lyricsList->setModel(m_lyricsDetailListModel);
    m_lyricsList->setFrameShape(QFrame::NoFrame);
    m_lyricsList->setLayoutMode(QListView::Batched);
    m_lyricsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    KNSaoStyle::styleVerticalScrollBar(m_lyricsList->verticalScrollBar());
    connect(m_lyricsList->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this, &KNMusicLyricsDownloadList::onActionItemPressed);
    //Register the lyrics list to theme manager.
    knTheme->registerWidget(m_lyricsList);
    //Configure the scroll lyrics.
    m_scrollLyrics->setObjectName("DownloadedLyricsPreview");
    knTheme->registerWidget(m_scrollLyrics);
    m_scrollLyrics->setBackend(m_previewBackend);
    m_scrollLyrics->hide();
    //Configure the player.
    QLinearGradient previewPlayerBase(0,0,0,16);
    previewPlayerBase.setColorAt(0, QColor(0x5d, 0x5d, 0x5d));
    previewPlayerBase.setColorAt(0.49, QColor(0x2d, 0x2d, 0x2d));
    previewPlayerBase.setColorAt(0.5, QColor(0x12, 0x12, 0x12));
    previewPlayerBase.setColorAt(0.51, QColor(0,0,0));
    previewPlayerBase.setColorAt(1, QColor(0,0,0));
    m_previewPlayer->setAutoFillBackground(true);
    QPalette pal=m_previewPlayer->palette();
    pal.setBrush(QPalette::Window, previewPlayerBase);
    m_previewPlayer->setPalette(pal);
    m_playNPause->setIcon(m_playIcon);
    m_playNPause->setFixedSize(16, 16);
    m_progress->setWheelStep(1000);
    connect(m_progress, &KNProgressSlider::sliderPressed,
            [=]{m_progressPressed=true;});
    connect(m_progress, &KNProgressSlider::sliderReleased,
            [=]{m_progressPressed=false;});
    m_previewPlayer->hide();

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          m_container);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(10);
    m_container->setLayout(mainLayout);
    //Add the widget to main layout.
    mainLayout->addStretch();
    mainLayout->addWidget(m_clockWheel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_downloadServer, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_lyricsList, 1);
    //Initial the player layout.
    QBoxLayout *playerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_previewPlayer);
    playerLayout->setContentsMargins(0,0,0,0);
    playerLayout->setSpacing(0);
    m_previewPlayer->setLayout(playerLayout);
    //Add player widget to layout.
    playerLayout->addWidget(m_playNPause);
    playerLayout->addWidget(m_progress, 1);
    mainLayout->addWidget(m_previewPlayer);
    mainLayout->addWidget(m_scrollLyrics, 1);
    mainLayout->addStretch();

    //Link the retranslate signal.
    knI18n->link(this, &KNMusicLyricsDownloadList::retranslate);
    retranslate();
}

void KNMusicLyricsDownloadList::hideAllWidgets()
{
    //Cut all the connections.
    cutLinkBackend();
    //Hide the download widget first.
    hideDownloadWidgets();
    //Hide the player and lyrics widget.
    m_previewPlayer->hide();
    m_lyricsList->hide();
}

void KNMusicLyricsDownloadList::showDownloadWidgets()
{
    //Show up the clock wheel widget.
    m_clockWheel->show();
    m_downloadServer->show();
    //Start ticking the clock wheel.
    m_clockWheel->startTick();
}

void KNMusicLyricsDownloadList::hideDownloadWidgets()
{
    //Hide the download widgets.
    m_clockWheel->hide();
    m_downloadServer->hide();
    //Stop ticking.
    m_clockWheel->stopTick();
}

void KNMusicLyricsDownloadList::setDownloadServerText(int current, int total)
{
    //Set the server text to server label.
    m_downloadServer->setText(m_downloadServerText.arg(QString::number(current),
                                                       QString::number(total)));
}

void KNMusicLyricsDownloadList::setLyricsList(
        const QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList)
{
    //Set the lyrics list to the lyrics detail list model.
    m_lyricsDetailListModel->setLyricsList(lyricsList);
    //Check out the lyrics size
    if(lyricsList.isEmpty())
    {
        //Hide the lyrics list.
        m_lyricsList->setVisible(false);
        //Disconnect the backend.
        cutLinkBackend();
        //Hide the player.
        m_previewPlayer->hide();
        m_scrollLyrics->hide();
        //Mission complete.
        return;
    }
    //Change the visiblity of the list view.
    m_lyricsList->setVisible(true);
    //Select the first index.
    m_lyricsList->setCurrentIndex(m_lyricsDetailListModel->index(0));
    m_lyricsList->setFocus();
    //Link the connections.
    linkBackend();
    //Show the player and lyrics preview.
    m_previewPlayer->show();
    m_scrollLyrics->show();
}

void KNMusicLyricsDownloadList::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the container.
    m_container->resize(size());
}

void KNMusicLyricsDownloadList::retranslate()
{
    //Update the download status and download server text.
    m_downloadServerText=tr("Searching lyrics on server (%1/%2)");
}

void KNMusicLyricsDownloadList::onActionItemPressed(const QModelIndex &index)
{
    //Get the lryics text from the index.
    QString lyricsData=index.data(Qt::UserRole).toString();
    //Generate the list.
    QList<qint64> positionList;
    QStringList textList;
    //Parse the data from the index.
    if(m_lrcParser->parseText(lyricsData, positionList, textList))
    {
        //Set parsed data to preview backend.
        m_previewBackend->setLyricsData(positionList, textList);
    }
    else
    {
        //Reset the backend.
        m_previewBackend->reset();
    }
}

void KNMusicLyricsDownloadList::onActionPlayStateChanged(const int &state)
{
    //When the state is playing, set it to pause icon for pausing.
    if(Playing==state)
    {
        //Set the pause icon.
        m_playNPause->setIcon(m_pauseIcon);
        return;
    }
    //Set the play icon.
    m_playNPause->setIcon(m_playIcon);
}

void KNMusicLyricsDownloadList::onActionPlayNPauseClick()
{
    //Get the backend.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the backend pointer.
    if(backend)
    {
        //Check out the preview playing state.
        if(Playing==backend->previewState())
        {
            //Pause the preview.
            backend->previewPause();
        }
        else
        {
            //Play the preview.
            backend->previewPlay();
        }
    }
}

void KNMusicLyricsDownloadList::onActionPositionChanged(const qint64 &position)
{
    //Check the pressed state.
    if(m_progressPressed)
    {
        //Ignore the position changed signal.
        return;
    }
    //Update the progress position.
    m_progress->setValue(position);
}

inline void KNMusicLyricsDownloadList::linkBackend()
{
    //Get the backend.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the backend pointer, and we havn't link before.
    if(backend!=nullptr && m_previewLinker.isEmpty())
    {
        //Link the backend with the preview backend.
        //Connect response.
        m_previewLinker.append(
                 connect(backend, &KNMusicBackend::previewPositionChanged,
                         m_previewBackend, &KNMusicLyricsBackend::setPosition));
        m_previewLinker.append(
                 connect(backend, &KNMusicBackend::previewPlayingStateChanged,
                         this,
                         &KNMusicLyricsDownloadList::onActionPlayStateChanged));
        m_previewLinker.append(
                connect(backend, &KNMusicBackend::previewPositionChanged,
                        this,
                        &KNMusicLyricsDownloadList::onActionPositionChanged));
        //Connect controls.
        m_previewLinker.append(
                connect(m_playNPause, &KNOpacityButton::clicked,
                        this,
                        &KNMusicLyricsDownloadList::onActionPlayNPauseClick));
        m_previewLinker.append(
                connect(m_progress, &KNProgressSlider::sliderMoved,
                        backend, &KNMusicBackend::setPreviewPosition));
        //Sync the duration from the backend.
        m_progress->setMaximum(backend->previewDuration());
    }
}

inline void KNMusicLyricsDownloadList::cutLinkBackend()
{
    //Disconnect all links.
    m_previewLinker.disconnectAll();
    //Reset the preview thread.
    //Get the backend.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the backend pointer.
    if(backend!=nullptr)
    {
        //Reset the preview backend.
        backend->previewStop();
    }
}
