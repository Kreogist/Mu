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
#include <QLabel>
#include <QBoxLayout>
#include <QFormLayout>
#include <QSignalMapper>

#include "knglobal.h"
#include "knlabelbutton.h"
#include "kneditablelabel.h"
#include "knglassanimebutton.h"
#include "knprogressslider.h"
#include "knopacityanimebutton.h"
#include "knvolumeslider.h"

#include "knmusicheaderplayerbase.h"
#include "knmusicnowplayingbase.h"
#include "knmusicmainlyrics.h"
#include "knmusicbackend.h"
#include "knmusicglobal.h"

#include "knmusicmainplayer.h"

#include <QDebug>

KNMusicMainPlayer::KNMusicMainPlayer(QWidget *parent) :
    KNMusicMainPlayerBase(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,240));
    setPalette(pal);

    //Initial the default album pixmap to no album.
    m_albumArt=KNMusicGlobal::instance()->noAlbumArt();

    //Initial the hide main player button.
    m_hideMainPlayer=new KNOpacityAnimeButton(this);
    m_hideMainPlayer->move(16, 16);
    m_hideMainPlayer->setFixedSize(32, 32);
    m_hideMainPlayer->setIcon(QPixmap(":/plugin/music/player/hide_mainplayer.png"));
    connect(m_hideMainPlayer, &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayer::onActionHideMainPlayer);

    //Initial the widgets.
    initialAlbumArt();
    initialInformationPanel();
    initialLyricsPanel();
    initialPlaylistPanel();
    initialControlPanel();
    initialBanner();

    //Initial main layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    QBoxLayout *panelLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           m_mainLayout->widget());
    panelLayout->setContentsMargins(0,0,0,0);
    panelLayout->setSpacing(0);
    m_mainLayout->addLayout(panelLayout, 1);

    QBoxLayout *songLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          m_mainLayout->widget());
    songLayout->setContentsMargins(0,0,0,0);
    songLayout->setSpacing(0);
    panelLayout->addLayout(songLayout, 1);

    songLayout->addWidget(m_albumArtLabel, 1, Qt::AlignBottom | Qt::AlignHCenter);

    QWidget *informationPanel=new QWidget(this);
    informationPanel->setLayout(m_infoPanelLayout);
    songLayout->addWidget(informationPanel, 0, Qt::AlignTop | Qt::AlignLeft);

    panelLayout->addWidget(m_mainLyrics, 2);

    m_mainLayout->addWidget(m_controlWidget);

    //Link retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicMainPlayer::retranslate);
    retranslate();
}

KNMusicMainPlayer::~KNMusicMainPlayer()
{
}

QWidget *KNMusicMainPlayer::banner()
{
    return m_banner;
}

void KNMusicMainPlayer::setBackend(KNMusicBackend *backend)
{
    m_backend=backend;

    //Calculate the volume stages, divide the volume range into 3 stages.
    int volumeStage=(m_backend->volumeMaximum()-m_backend->volumeMinimal())/3;
    m_firstStageVolume=m_backend->volumeMinimal()+volumeStage;
    m_secondStageVolume=m_firstStageVolume+volumeStage;
    //Set the volume slider range.
    m_volumeSlider->setRange(m_backend->volumeMinimal(),
                             m_backend->volumeMaximum());
    //Change the mouse step based on the range.
    int preferStep=(m_volumeSlider->maximum()-m_volumeSlider->minimal())/100;
    m_volumeSlider->setWheelStep(preferStep<1?1:preferStep);
    //Set the volume slider to the backend volume size.
    m_volumeSlider->setValue(m_backend->volume());
    //Link the volume slider to backend.
    connect(m_volumeSlider, &KNVolumeSlider::valueChanged,
            m_backend, &KNMusicBackend::setVolume);

    //Connect responses.
    connect(m_backend, &KNMusicBackend::positionChanged,
           this, &KNMusicMainPlayer::onActionPositionChanged);
    connect(m_backend, &KNMusicBackend::durationChanged,
            this, &KNMusicMainPlayer::onActionDurationChanged);
    connect(m_backend, &KNMusicBackend::playingStateChanged,
            this, &KNMusicMainPlayer::onActionPlayStateChanged);
    connect(m_backend, &KNMusicBackend::volumeChanged,
            this, &KNMusicMainPlayer::onActionVolumeChanged);
}

void KNMusicMainPlayer::setHeaderPlayer(KNMusicHeaderPlayerBase *headerPlayer)
{
    //Save the header player.
    m_headerPlayer=headerPlayer;
    //Set the header player to main lyrics.
    m_mainLyrics->setHeaderPlayer(m_headerPlayer);
    //Link the header player.
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::analysisItemUpdated,
            this, &KNMusicMainPlayer::onActionAnalysisItemUpdate);
    //Hack the current player button to the header player.
    connect(m_controlButtons[ButtonPrev], &KNGlassAnimeButton::clicked,
            m_headerPlayer, &KNMusicHeaderPlayerBase::requirePlayPrevious);
    connect(m_controlButtons[ButtonNext], &KNGlassAnimeButton::clicked,
            m_headerPlayer, &KNMusicHeaderPlayerBase::requirePlayNext);
}

void KNMusicMainPlayer::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //Save now playing.
    m_nowPlaying=nowPlaying;
    //Link now playing.
    connect(m_loopMode, &KNOpacityAnimeButton::clicked,
            m_nowPlaying, &KNMusicNowPlayingBase::changeLoopState);
    connect(m_nowPlaying, &KNMusicNowPlayingBase::loopStateChanged,
            this, &KNMusicMainPlayer::onActionLoopStateChanged);
    //Sync the data with now playing.
    onActionLoopStateChanged(m_nowPlaying->loopState());
}

void KNMusicMainPlayer::retranslate()
{
    ;
}

void KNMusicMainPlayer::onActionProgressPressed()
{
    m_progressPressed=true;
}

void KNMusicMainPlayer::onActionProgressReleased()
{
    m_progressPressed=false;
}

void KNMusicMainPlayer::onActionHideMainPlayer()
{
    //Hide the banner.
    m_banner->hide();
    //Emit hide main player signal.
    emit requireHideMainPlayer();
}

void KNMusicMainPlayer::onActionPlayNPauseClicked()
{
    //Ignore when there's no file path.
    if(m_backend==nullptr ||
            m_headerPlayer->currentAnalysisItem().detailInfo.filePath.isEmpty())
    {
        return;
    }
    //When play & pause pressed and the button is displaying play icon,
    //It means now is stopping or pausing. Should emit play signal.
    if(m_isShownPlay)
    {
        m_backend->play();
        return;
    }
    m_backend->pause();
}

void KNMusicMainPlayer::onActionPositionEdited()
{
    //Get the latest text
    QString positionText=m_position->text();
    //Find the colon.
    int colonPosition=positionText.indexOf(':');
    //If we cannot find the colon, means it's not format as 'xx:xx'.
    if(-1==colonPosition)
    {
        //This might be a number, we treat it as second time.
        //Translate it to a number.
        bool translateSuccess=false;
        qint64 triedPositon=positionText.toLongLong(&translateSuccess);
        //If we succeed, set the position to that second.
        if(translateSuccess)
        {
            setPosition(triedPositon*1000);
        }
        return;
    }
    //Calculate the ms.
    qint64 minuatePart=positionText.left(colonPosition).toInt(),
           secondPart=positionText.mid(colonPosition+1).toInt(),
           preferPosition=(minuatePart*60+secondPart)*1000;
    if(preferPosition>0 && preferPosition<m_progress->maximum())
    {
        setPosition(preferPosition);
    }
}

void KNMusicMainPlayer::onActionPositionChanged(const qint64 &position)
{
    if(!m_progressPressed)
    {
        m_progress->setValue(position);
    }
}

void KNMusicMainPlayer::onActionDurationChanged(const qint64 &duration)
{
    //Change the progress slider range.
    m_progress->setMaximum(duration);
    //Set duration display text.
    m_duration->setText(KNMusicGlobal::msecondToString(duration));
}

void KNMusicMainPlayer::onActionPlayStateChanged(const int &state)
{
    //If it's playing, then should display pause icon.
    if(state==PlayingState)
    {
        setPauseIconMode();
        return;
    }
    //Or else, whatever stopped or paused state, should display play icon.
    setPlayIconMode();
}

void KNMusicMainPlayer::onActionVolumeChanged(const int &volumeSize)
{
    if(volumeSize==m_backend->volumeMinimal()) //Check is the size is the minimum.
    {
        m_volumeIcon->setPixmap(m_volumeSizeIcon[NoVolume]);
    }
    else if(volumeSize<m_firstStageVolume) //Check is the size in the first stage.
    {
        m_volumeIcon->setPixmap(m_volumeSizeIcon[Volume1]);
    }
    else if(volumeSize<m_secondStageVolume) //Check is the size in the second stage.
    {
        m_volumeIcon->setPixmap(m_volumeSizeIcon[Volume2]);
    }
    else //Then all the other volume is in the third stage.
    {
        m_volumeIcon->setPixmap(m_volumeSizeIcon[Volume3]);
    }
    //Check the volume slider is the same as the current volume.
    if(volumeSize!=m_volumeSlider->value())
    {
        m_volumeSlider->blockSignals(true);
        m_volumeSlider->setValue(volumeSize);
        m_volumeSlider->blockSignals(false);
    }
}

void KNMusicMainPlayer::setPositionText(const qint64 &position)
{
    if(!m_position->editing())
    {
        m_position->setText(KNMusicGlobal::msecondToString(position));
    }
}

void KNMusicMainPlayer::setPosition(const qint64 &position)
{
    m_backend->setPosition(position);
}

void KNMusicMainPlayer::onActionRequireShowIn(const int &label)
{
    //Emit show in signal.
    switch(label)
    {
    case ElementTitle:
        emit requireShowInSongs();
        break;
    case ElementArtist:
        emit requireShowInArtists();
        break;
    case ElementAlbum:
        emit requireShowInAlbums();
        break;
    case ElementGenre:
        emit requireShowInGenres();
        break;
    default:
        //Should never comes here.
        break;
    }
    //Hide the main player.
    onActionHideMainPlayer();
}

void KNMusicMainPlayer::setPlayIconMode()
{
    //Set the icon and the flag.
    m_controlButtons[ButtonPlayNPause]->setIcon(m_playIcon);
    m_isShownPlay=true;
}

void KNMusicMainPlayer::setPauseIconMode()
{
    //Set the icon and the flag.
    m_controlButtons[ButtonPlayNPause]->setIcon(m_pauseIcon);
    m_isShownPlay=false;
}

void KNMusicMainPlayer::onActionAnalysisItemUpdate()
{
    //Set detail information.
    const KNMusicAnalysisItem &analysisItem=
            m_headerPlayer->currentAnalysisItem();
    if(analysisItem.coverImage.isNull())
    {
        m_albumArt=KNMusicGlobal::instance()->noAlbumArt();
    }
    else
    {
        //Save the album art.
        m_albumArt=QPixmap::fromImage(analysisItem.coverImage);
    }
    //Update the panel.
    updateInformationPanel();
}

void KNMusicMainPlayer::onActionLoopStateChanged(const int &state)
{
    //Change the icon.
    m_loopMode->setIcon(m_loopStateIcon[state]);
}

void KNMusicMainPlayer::showEvent(QShowEvent *event)
{
    //Show the banner.
    m_banner->show();
    //Start draw the lyrics.
    m_mainLyrics->setDrawLyrics(true);
    //Do show event.
    KNMusicMainPlayerBase::showEvent(event);
}

void KNMusicMainPlayer::hideEvent(QHideEvent *event)
{
    //Stop to draw lyrics.
    m_mainLyrics->setDrawLyrics(false);
    //Do hide event.
    KNMusicMainPlayerBase::hideEvent(event);
}

void KNMusicMainPlayer::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    KNMusicMainPlayerBase::resizeEvent(event);
    //Calculate the information layout spacing.
    int parameterSize=qMin(width(), height());
    //Update pixmap size.
    m_pixmapSize=parameterSize/3;
    //Update the font size.
    int fontSize=parameterSize/40;
    m_infoPanelLayout->setVerticalSpacing(fontSize);
    if(fontSize<15)
    {
        fontSize=15;
    }
    m_mainLayout->setContentsMargins(fontSize, fontSize, fontSize, 0);
    m_mainLayout->setSpacing(fontSize>>1);
    m_infoPanelLayout->setContentsMargins(0, fontSize, 0, fontSize);
    m_infoPanelLayout->setHorizontalSpacing(fontSize);

    //Change the font size.
    QFont captionFont=m_informationElements[0]->font();
    captionFont.setPixelSize(fontSize);
    //Set font to all the labels.
    int maxLabelWidth=0;
    for(int i=0; i<InformationElementsCount; i++)
    {
        maxLabelWidth=qMax(maxLabelWidth,
                           m_infoPanelLayout->itemAt(i, QFormLayout::LabelRole)->widget()->width());
        m_informationElementIcons[i]->setFixedSize(fontSize+5,
                                                   fontSize+5);
        m_informationElements[i]->setFont(captionFont);
    }
    QFont timeFont=m_duration->font();
    timeFont.setPixelSize(fontSize);
    m_duration->setFont(timeFont);
    m_position->setFont(timeFont);
    //Set lyrics font.
    QFont lyricsFont=m_mainLyrics->font();
    lyricsFont.setPixelSize(fontSize+fontSize/4);
    m_mainLyrics->setFont(lyricsFont);
    m_mainLyrics->setSpacing(fontSize>>1);
    m_mainLyrics->setLeftSpacing(fontSize<<1);
    //Change the button size.
    int buttonSize=fontSize*3+1;
    for(int i=0; i<ControlButtonsCount; i++)
    {
        m_controlButtons[i]->setFixedSize(buttonSize, buttonSize);
    }
    int iconButtonSize=buttonSize>>1;
    m_loopMode->setFixedSize(iconButtonSize, iconButtonSize);
    m_volumeIcon->setFixedSize(iconButtonSize, iconButtonSize);
    //Change the layout spacing.
    m_buttonLeftLayout->setContentsMargins(buttonSize>>2,0,buttonSize>>2,0);
    m_buttonRightLayout->setContentsMargins(buttonSize>>2,0,buttonSize>>2,0);
    //Calculate the elements label maximum width.
    m_maxElementWidth=width()/3-(fontSize+5+maxLabelWidth);
    //Update information.
    updateInformationPanel();
}

void KNMusicMainPlayer::initialAlbumArt()
{
    m_albumArtLabel=new QLabel(this);
    m_albumArtLabel->setAlignment(Qt::AlignCenter);
}

void KNMusicMainPlayer::initialInformationPanel()
{
    //We will set the parent later.
    m_infoPanelLayout=new QFormLayout;
    m_infoPanelLayout->setFormAlignment(Qt::AlignHCenter);

    QString elementIconPath[InformationElementsCount];
    elementIconPath[ElementTitle]=":/plugin/music/category/01_musics.png";
    elementIconPath[ElementArtist]=":/plugin/music/category/02_artists.png";
    elementIconPath[ElementAlbum]=":/plugin/music/category/03_ablums.png";
    elementIconPath[ElementGenre]=":/plugin/music/category/04_genres.png";

    QSignalMapper *iconClicked=new QSignalMapper(this);
    QSignalMapper *labelClicked=new QSignalMapper(this);
    connect(iconClicked, SIGNAL(mapped(int)),
            this, SLOT(onActionRequireShowIn(int)));
    connect(labelClicked, SIGNAL(mapped(int)),
            this, SLOT(onActionRequireShowIn(int)));
    //Initial the element labels.
    for(int i=0; i<InformationElementsCount; i++)
    {
        //Initial the element caption.
        QWidget *caption=new QWidget(this);
        caption->setContentsMargins(0,0,0,0);
        QBoxLayout *captionLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                 caption);
        captionLayout->setContentsMargins(0,0,0,0);
        captionLayout->setSpacing(15);
        caption->setLayout(captionLayout);
        //Initial the element icon.
        m_informationElementIcons[i]=new KNLabelButton(this);
        m_informationElementIcons[i]->setScaledContents(true);
        m_informationElementIcons[i]->setPixmap(QPixmap(elementIconPath[i]));
        captionLayout->addWidget(m_informationElementIcons[i]);
        //Initial text label.
        m_informationElements[i]=new KNLabelButton(this);
        //Set mapping.
        connect(m_informationElementIcons[i], SIGNAL(clicked()),
                iconClicked, SLOT(map()));
        iconClicked->setMapping(m_informationElementIcons[i], i);
        connect(m_informationElements[i], SIGNAL(clicked()),
                labelClicked, SLOT(map()));
        labelClicked->setMapping(m_informationElements[i], i);
        //Add to layout.
        m_infoPanelLayout->addRow(caption,
                                  m_informationElements[i]);
    }
    QPalette elementsPal=m_informationElements[0]->palette();
    elementsPal.setColor(QPalette::WindowText, QColor(0xff, 0xff, 0xff));
    for(int i=0; i<InformationElementsCount; i++)
    {
        m_informationElements[i]->setPalette(elementsPal);
    }
}

void KNMusicMainPlayer::updateInformationPanel()
{
    //Update album art.
    m_albumArtLabel->setPixmap(m_albumArt.scaled(m_pixmapSize,
                                                 m_pixmapSize,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
    //Update detail captions.
    const KNMusicDetailInfo &detailInfo=m_headerPlayer->currentAnalysisItem().detailInfo;
    setEliedLabelText(m_informationElements[ElementTitle], detailInfo.textLists[Name], m_maxElementWidth);
    setEliedLabelText(m_informationElements[ElementArtist], detailInfo.textLists[Artist], m_maxElementWidth);
    setEliedLabelText(m_informationElements[ElementAlbum], detailInfo.textLists[Album], m_maxElementWidth);
    setEliedLabelText(m_informationElements[ElementGenre], detailInfo.textLists[Genre], m_maxElementWidth);
}

void KNMusicMainPlayer::initialLyricsPanel()
{
    m_mainLyrics=new KNMusicMainLyrics(this);
    //Hide the lyrics by default.
    m_mainLyrics->setDrawLyrics(false);
}

void KNMusicMainPlayer::initialBanner()
{
    //Initial the banner.
    m_banner=new QWidget(this);
    //Hide the banner at default.
    m_banner->hide();
}

void KNMusicMainPlayer::initialPlaylistPanel()
{
    ;
}

void KNMusicMainPlayer::initialControlPanel()
{
    //Initial icons and integers.
    m_playIcon=QPixmap(":/plugin/music/mainplayer/play.png");
    m_pauseIcon=QPixmap(":/plugin/music/mainplayer/pause.png");
    m_loopStateIcon[NoRepeat]=QPixmap(":/plugin/music/loopmode/NoRepeat.png");
    m_loopStateIcon[RepeatTrack]=QPixmap(":/plugin/music/loopmode/RepeatSingle.png");
    m_loopStateIcon[RepeatAll]=QPixmap(":/plugin/music/loopmode/Repeat.png");
    m_loopStateIcon[Shuffle]=QPixmap(":/plugin/music/loopmode/Random.png");

    m_volumeSizeIcon[NoVolume]=QPixmap(":/plugin/music/mainplayer/volume_0.png");
    m_volumeSizeIcon[Volume1]=QPixmap(":/plugin/music/mainplayer/volume_1.png");
    m_volumeSizeIcon[Volume2]=QPixmap(":/plugin/music/mainplayer/volume_2.png");
    m_volumeSizeIcon[Volume3]=QPixmap(":/plugin/music/mainplayer/volume_3.png");
    const int buttonSize=46;
    //Initial the control widgets.
    m_controlWidget=new QWidget(this);
    //Initial the progress bar and position labels.
    m_progress=new KNProgressSlider(this);
    m_progress->setWheelStep(1000);
    m_duration=new QLabel(this);
    m_position=new KNEditableLabel(this);
    m_loopMode=new KNOpacityAnimeButton(this);
    m_loopMode->setFixedSize(buttonSize>>1, buttonSize>>1);
    m_volumeIcon=new KNLabelButton(this);
    m_volumeIcon->setScaledContents(true);
    m_volumeIcon->setPixmap(m_volumeSizeIcon[NoVolume]);
    m_volumeIcon->setFixedSize(buttonSize>>1, buttonSize>>1);
    m_volumeSlider=new KNVolumeSlider(this);
    m_volumeSlider->setMinimumWidth(50);
    m_volumeSlider->setMaximumWidth(100);
    //Configure label font.
    QPalette pal=m_duration->palette();
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    m_duration->setPalette(pal);
    m_position->setPalette(pal);
    QFont timeFont=m_duration->font();
    timeFont.setFamily("096MKSD");
    m_duration->setFont(timeFont);
    m_position->setFont(timeFont);
    //Link the progress changed signal.
    connect(m_progress, &KNProgressSlider::sliderPressed,
            this, &KNMusicMainPlayer::onActionProgressPressed);
    connect(m_progress, &KNProgressSlider::sliderReleased,
            this, &KNMusicMainPlayer::onActionProgressReleased);
    connect(m_progress, &KNProgressSlider::valueChanged,
            this, &KNMusicMainPlayer::setPositionText);
    connect(m_progress, &KNProgressSlider::sliderMoved,
            this, &KNMusicMainPlayer::setPosition);
    connect(m_position, &KNEditableLabel::editingFinished,
            this, &KNMusicMainPlayer::onActionPositionEdited);
    //Initial the control buttons.
    for(int i=0; i<ControlButtonsCount; i++)
    {
        m_controlButtons[i]=new KNGlassAnimeButton(this);
        m_controlButtons[i]->setFixedSize(buttonSize, buttonSize);
    }
    m_controlButtons[0]->setLeftLineVisible(true);
    //Configure buttons.
    m_controlButtons[ButtonPrev]->setIcon(QPixmap(":/plugin/music/mainplayer/previous.png"));
    m_controlButtons[ButtonPlayNPause]->setIcon(m_playIcon);
    m_controlButtons[ButtonNext]->setIcon(QPixmap(":/plugin/music/mainplayer/next.png"));
    //Link the button.
    connect(m_controlButtons[ButtonPlayNPause], &KNGlassAnimeButton::clicked,
            this, &KNMusicMainPlayer::onActionPlayNPauseClicked);

    //Initial the layouts, add the widget to the layouts.
    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             m_controlWidget);
    controlLayout->setContentsMargins(0,0,0,0);
    controlLayout->setSpacing(0);
    m_controlWidget->setLayout(controlLayout);

    controlLayout->addWidget(m_progress);

    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            controlLayout->widget());
    buttonLayout->setContentsMargins(0,0,0,0);
    buttonLayout->setSpacing(0);
    controlLayout->addLayout(buttonLayout, 1);

    buttonLayout->addWidget(m_position, 0, Qt::AlignTop);
    m_buttonLeftLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                      controlLayout->widget());
    m_buttonLeftLayout->setContentsMargins(buttonSize>>2,0,buttonSize>>2,0);
    m_buttonLeftLayout->setSpacing(0);
    m_buttonLeftLayout->addStretch();
    m_buttonLeftLayout->addWidget(m_loopMode, 0, Qt::AlignVCenter);
    buttonLayout->addLayout(m_buttonLeftLayout, 1);
    for(int i=0; i<ControlButtonsCount; i++)
    {
        buttonLayout->addWidget(m_controlButtons[i], 0, Qt::AlignBottom);
    }
    m_buttonRightLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                       controlLayout->widget());
    m_buttonRightLayout->setContentsMargins(buttonSize>>2,0,buttonSize>>2,0);
    m_buttonRightLayout->setSpacing(0);
    m_buttonRightLayout->addWidget(m_volumeIcon);
    m_buttonRightLayout->addWidget(m_volumeSlider);
    m_buttonRightLayout->addStretch();
    buttonLayout->addLayout(m_buttonRightLayout, 1);
    buttonLayout->addWidget(m_duration, 0, Qt::AlignTop);

    //Set default value.
    setPositionText(0);
    onActionDurationChanged(0);
    onActionLoopStateChanged(NoRepeat);
}

void KNMusicMainPlayer::setEliedLabelText(QLabel *label,
                                          const QString &text,
                                          const int &width)
{
    if(label->fontMetrics().width(text)>width)
    {
        label->setText(label->fontMetrics().elidedText(text,
                                                       Qt::ElideRight,
                                                       width));
        label->setToolTip(text);
    }
    else
    {
        label->setText(text);
        label->setToolTip("");
    }
}
