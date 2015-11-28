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
#include <QApplication>
#include <QBoxLayout>
#include <QTimeLine>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QCursor>

#include "knimagelabel.h"
#include "knopacityanimebutton.h"
#include "knopacitybutton.h"
#include "knthememanager.h"
#include "knprogressslider.h"
#include "kneditablelabel.h"
#include "knloopscrolllabel.h"
#include "knglobal.h"
#include "knconfigure.h"

#include "knmusicbackend.h"
#include "knmusicnowplayingbase.h"
#include "knmusichscrolllyrics.h"
#include "knmusicglobal.h"
#include "knmusiclyricsmanager.h"

#include "knmusicminiplayer.h"

#include <QDebug>

#define DetectMargin 20

KNMusicMiniPlayer::KNMusicMiniPlayer(QWidget *parent) :
    KNMusicMiniPlayerBase(parent),
    m_pressedPoint(QPoint()),
    m_originalPos(QPoint()),
    m_container(new QWidget(this)),
    m_icon(new KNImageLabel(this)),
    m_position(new KNEditableLabel(this)),
    m_previous(
        generateControlButton(":/plugin/music/player/mini_previous.png")),
    m_playNPause(generateControlButton()),
    m_next(generateControlButton(":/plugin/music/player/mini_next.png")),
    m_mute(generateControlButton()),
    m_restore(generateButton(":/plugin/music/player/mini_maximum.png")),
    m_close(generateButton(":/plugin/music/player/mini_close.png")),
    m_detailLabel(new KNLoopScrollLabel(this)),
    m_progressSlider(new KNProgressSlider(this)),
    m_lyrics(new KNMusicHScrollLyrics(this)),
    m_moving(new QTimeLine(200, this)),
    m_backend(nullptr),
    m_nowPlaying(nullptr),
    m_cacheConfigure(knGlobal->cacheConfigure()->getConfigure("MiniPlayer")),
    m_progressPressed(false),
    m_pressed(false),
    m_freeze(false)
{
    setObjectName("MiniPlayer");
    //Set property.
    setAutoFillBackground(true);
#ifdef Q_OS_MACX
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#else
    setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
#endif
    setFixedSize(fontMetrics().height()*36, fontMetrics().height()<<1);
    //Initial the resource.
    // Mute icons.
    m_muteIcon[0]=QIcon(":/plugin/music/player/mini_mute_false.png");
    m_muteIcon[1]=QIcon(":/plugin/music/player/mini_mute.png");
    // Play and pause icons.
    m_iconPause=QIcon(":/plugin/music/player/mini_pause.png");
    m_iconPlay=QIcon(":/plugin/music/player/mini_play.png");

    //Configure the container.
    m_container->setFixedSize(size());
    m_container->move(0, -height());
    //Configure the position label.
    m_position->setObjectName("MiniPosition");
    m_position->setFixedHeight(fontMetrics().height());
    QFont timeFont=font();
    timeFont.setFamily("096MKSD");
    m_position->setFont(timeFont);
    knTheme->registerWidget(m_position);
    connect(m_position, &KNEditableLabel::startEditing,
            [=]{m_freeze=true;});
    connect(m_position, &KNEditableLabel::endEditing,
            this, &KNMusicMiniPlayer::thawAnime);
    //Configure the icon label.
    m_icon->setPixmap(QPixmap(":/plugin/preference/about.png"));
    m_icon->setFixedSize(16, 16);
    m_icon->setCursor(Qt::SizeAllCursor);
    //Configure default icons.
    m_playNPause->setIcon(m_iconPlay);
    m_mute->setIcon(m_muteIcon[false]);
    //Configure progress bar.
    m_progressSlider->setWheelStep(1000);
    m_progressSlider->setFixedWidth(100);
    connect(m_progressSlider, &KNProgressSlider::sliderPressed,
            [=]{m_progressPressed=true;});
    connect(m_progressSlider, &KNProgressSlider::sliderReleased,
            [=]{m_progressPressed=false;});
    connect(m_progressSlider, &KNProgressSlider::sliderMoved,
            this, &KNMusicMiniPlayer::setPosition);
    connect(m_progressSlider, &KNProgressSlider::valueChanged,
            [=](const qint64 &position)
            {
                //Check if the position label is being editing.
                if(!m_position->isEditing())
                {
                    //Update the position data to new time position
                    m_position->setText(KNMusicUtil::msecondToString(position));
                }
            });
    //Configure the restore and close buttons.
    connect(m_restore, &KNOpacityButton::clicked,
            this, &KNMusicMiniPlayer::requireHidePlayer);
    connect(m_close, &KNOpacityButton::clicked,
            this, &KNMusicMiniPlayer::requireCloseMainWindow);
    //If the platform is Mac OS X, hide the close button.
#ifdef Q_OS_MACX
    m_close->hide();
#endif
    //Configure the scroll lyrics.
    m_lyrics->setObjectName("MiniLyrics");
    m_lyrics->setBackend(knMusicGlobal->lyricsManager()->backend());
    m_lyrics->setFixedSize(size());
    knTheme->registerWidget(m_lyrics);
    //Configure the time line.
    m_moving->setEasingCurve(QEasingCurve::OutCubic);
    m_moving->setUpdateInterval(10);
    connect(m_moving, &QTimeLine::frameChanged,
            this, &KNMusicMiniPlayer::onActionMouseInOut);
    //Configure the container.
    QBoxLayout *containerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                               this);
    containerLayout->setContentsMargins(8,0,8,0);
    containerLayout->setSpacing(4);
    m_container->setLayout(containerLayout);
    //Add widgets to container layout.
    containerLayout->addWidget(m_icon);
    containerLayout->addWidget(m_detailLabel, 1, Qt::AlignVCenter);
    containerLayout->addWidget(m_position);
    containerLayout->addWidget(m_progressSlider);
    containerLayout->addWidget(m_previous);
    containerLayout->addSpacing(4);
    containerLayout->addWidget(m_playNPause);
    containerLayout->addSpacing(4);
    containerLayout->addWidget(m_next);
    containerLayout->addSpacing(4);
    containerLayout->addWidget(m_mute);
    containerLayout->addSpacing(4);
    containerLayout->addWidget(m_restore);
    containerLayout->addWidget(m_close);

    //Register the theme.
    knTheme->registerWidget(this);
    //Move to default position: top left of the desktop.
    move(0, 0);
}

void KNMusicMiniPlayer::setBackend(KNMusicBackend *backend)
{
    //Save the backend pointer.
    m_backend=backend;
    //Check whether the backend is null or not.
    if(m_backend==nullptr)
    {
        return;
    }
    //Link controls.
    connect(m_playNPause, &KNOpacityAnimeButton::clicked,
            [=]
            {
                //Check the state of the backend.
                //If the backend is now at the playing state, pause the backend.
                if(m_backend->state()==Playing)
                {
                    m_backend->pause();
                    return;
                }
                //Start to play the main thread.
                m_backend->play();
            });
    connect(m_mute, &KNOpacityAnimeButton::clicked,
            [=]
            {
                //Change the mute state.
                m_backend->changeMuteState();
            });
    connect(m_position, &KNEditableLabel::contentChanged,
            [=]
            {
                //Get the latest text from the position.
                QString positionText=m_position->text();
                //Find the colon.
                int colonPosition=positionText.indexOf(':');
                //If we cannot find the colon, means it's not format as 'xx:xx'.
                if(-1==colonPosition)
                {
                    //This might be a number, we treat it as second time.
                    //Translate it to a number.
                    bool translated=false;
                    qint64 triedPositon=positionText.toLongLong(&translated);
                    //If we succeed, set the position to that second.
                    if(translated)
                    {
                        m_backend->setPosition(triedPositon*1000);
                    }
                    return;
                }
                //Calculate the ms.
                qint64 minutePart=positionText.left(colonPosition).toInt(),
                       secondPart=positionText.mid(colonPosition+1).toInt(),
                       preferPosition=(minutePart*60+secondPart)*1000;
                //Check result.
                if(preferPosition>-1 &&
                        preferPosition<m_progressSlider->maximum())
                {
                    m_backend->setPosition(preferPosition);
                }
            });
    //Connect the response.
    connect(m_backend, &KNMusicBackend::positionChanged,
            [=](const qint64 &position)
            {
                //Update the value of progress slider when the progress slider
                //is not being edited.
                if(!m_progressPressed)
                {
                    //Update the slider value.
                    m_progressSlider->setValue(position);
                }
            });
    connect(m_backend, &KNMusicBackend::playingStateChanged,
            [=](const int &state)
            {
                //If it's playing, then should display pause icon.
                m_playNPause->setIcon((state==Playing)?m_iconPause:m_iconPlay);
                //Pause the scroll lyrics.
                m_lyrics->setPause(state!=Playing);
            });
    connect(m_backend, &KNMusicBackend::muteStateChanged,
            [=](const bool &mute)
            {
                //Update the icon of volume indicator.
                m_mute->setIcon(m_muteIcon[mute]);
            });
    connect(m_backend, &KNMusicBackend::durationChanged,
            [=](const qint64 &duration)
            {
                //Update the maximum of the slider.
                m_progressSlider->setMaximum(duration);
            });
}

void KNMusicMiniPlayer::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //Save the now playing pointer.
    m_nowPlaying=nowPlaying;
    //Check whether the now playing is null.
    if(m_nowPlaying==nullptr)
    {
        return;
    }
    //Link the header player's control signal to now playing model.
    connect(m_next, &KNOpacityAnimeButton::clicked,
            m_nowPlaying, &KNMusicNowPlayingBase::playNext);
    connect(m_previous, &KNOpacityAnimeButton::clicked,
            m_nowPlaying, &KNMusicNowPlayingBase::playPrevious);
    //Link the now playing model's response to header player.
    connect(m_nowPlaying, &KNMusicNowPlayingBase::nowPlayingChanged,
            [=](const KNMusicAnalysisItem &analysisItem)
            {
                //Get the artist.
                QString &&artistText=
                        analysisItem.detailInfo.textLists[Artist].toString(),
                        playingSongData =
                        (artistText.isEmpty() ? "" : (artistText+" - ")) +
                            analysisItem.detailInfo.textLists[Name].toString();
                //Set the detail label text.
                m_detailLabel->setText(playingSongData);
                //Check the artist name.
                m_lyrics->setPlaceHolderText(playingSongData);
            });
    connect(m_nowPlaying, &KNMusicNowPlayingBase::nowPlayingReset,
            this, &KNMusicMiniPlayer::reset);
    //Reset the mini player.
    reset();
}

void KNMusicMiniPlayer::reset()
{
    //Clear the slider range.
    m_progressSlider->setMaximum(0);
    //Reset the label.
    m_position->setText("0:00");
    //Reset the detail text.
    m_detailLabel->setText("Kreogist " + qApp->applicationName());
    //Reset the lyrics place holder text.
    m_lyrics->setPlaceHolderText("Kreogist " + qApp->applicationName());
}

void KNMusicMiniPlayer::saveConfigure()
{
    //Set the window position.
    setCacheValue("miniPlayerX", geometry().x());
    setCacheValue("miniPlayerY", geometry().y());
    //Set the current desktop size.
    setCacheValue("desktopWidth", qApp->desktop()->width());
    setCacheValue("desktopHeight", qApp->desktop()->height());
}

void KNMusicMiniPlayer::loadConfigure()
{
    //Check is the last record is complete. If there's no windowWidth property
    //in the configure, means we don't save the last geometry. Ignore the
    //recover request.
    if(m_cacheConfigure->data("miniPlayerX").isNull())
    {
        return;
    }

    //Read the resolution data of the last time closed.
    int lastScreenWidth=getCacheValue("desktopWidth"),
        lastScreenHeight=getCacheValue("desktopHeight"),
        currentScreenWidth=qApp->desktop()->width(),
        currentScreenHeight=qApp->desktop()->height(),
            lastX=getCacheValue("miniPlayerX"),
            lastY=getCacheValue("miniPlayerY");
    //Check is the resolution is the same as the last closed time.
    if(!(lastScreenWidth==currentScreenWidth &&
         lastScreenHeight==currentScreenHeight))
    {
        //The resolution has been changed, recalculate the size parameters.
        //Get the width ratio and the height ratio.
        qreal widthRatio=(qreal)currentScreenWidth/(qreal)lastScreenWidth,
              heightRatio=(qreal)currentScreenHeight/(qreal)lastScreenHeight;
        //Recalculate the last parameters.
        zoomParameter(lastX, widthRatio);
        zoomParameter(lastY, heightRatio);
    }
    //Check the parameter, ensure that one part of the window must be inside
    //screen.
    //If it's not inside the screen, make the top of the window 0.
    if(lastY<0 || lastY>currentScreenHeight)
    {
        lastY=0;
    }
    //Set the geometry.
    move(lastX, lastY);
}

void KNMusicMiniPlayer::enterEvent(QEvent *event)
{
    //Do original event.
    KNMusicMiniPlayerBase::enterEvent(event);
    //Check freeze state first.
    if(!m_freeze)
    {
        //Move to hide the lyrics widget.
        startAnime(height());
    }
}

void KNMusicMiniPlayer::leaveEvent(QEvent *event)
{
    //Do original event
    KNMusicMiniPlayerBase::leaveEvent(event);
    //Check freeze state first.
    if(!m_freeze)
    {
        //Move to show the lyrics widget.
        startAnime(0);
    }
}

void KNMusicMiniPlayer::mousePressEvent(QMouseEvent *event)
{
    //Do original pressed event.
    KNMusicMiniPlayerBase::mousePressEvent(event);
    //Set pressed flag.
    m_pressed=true;
    //Save the pressed point.
    m_pressedPoint=event->globalPos();
    m_originalPos=pos();
    //Freeze the window.
    m_freeze=true;
}

void KNMusicMiniPlayer::mouseMoveEvent(QMouseEvent *event)
{
    //Do original move event.
    KNMusicMiniPlayerBase::mouseMoveEvent(event);
    //Move the widget if pressed.
    if(m_pressed)
    {
        //Calculate the moving position.
        QPoint targetPosition(m_originalPos+event->globalPos()-m_pressedPoint);
        //Get the desktop widget.
        QDesktopWidget *desktop=qApp->desktop();
        //Check target position.
        if(targetPosition.y()<DetectMargin)
        {
            targetPosition.setY(0);
        }
        else if(targetPosition.y()+height()+DetectMargin>desktop->height())
        {
            targetPosition.setY(desktop->height()-height());
        }
        if(targetPosition.x()<DetectMargin)
        {
            targetPosition.setX(0);
        }
        else if(targetPosition.x()+width()+DetectMargin>desktop->width())
        {
            targetPosition.setX(desktop->width()-width());
        }
        //Move to the position.
        move(targetPosition);
    }
}

void KNMusicMiniPlayer::mouseReleaseEvent(QMouseEvent *event)
{
    //Do original released event.
    KNMusicMiniPlayerBase::mouseReleaseEvent(event);
    //Release the pressed flag.
    m_pressed=false;
    //Check the edit state.
    if(!m_position->isEditing())
    {
        //Thaw the mini player.
        thawAnime();
    }
}

void KNMusicMiniPlayer::onActionMouseInOut(int frame)
{
    //Move the widgets.
    m_lyrics->move(0, frame);
    m_container->move(0, frame-height());
//    //Change the background color.
//    QPalette pal=palette();
//    QColor backColor=pal.color(QPalette::Window);
//    backColor.setHsv(backColor.hue(),
//                     backColor.saturation(),
//                     (qreal)frame/(qreal)height()*255.0);
//    pal.setColor(QPalette::Window, backColor);
    //    setPalette(pal);
}

inline KNOpacityAnimeButton *KNMusicMiniPlayer::generateControlButton(
        const QString &iconPath)
{
    //Generate the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Set the icon.
    if(!iconPath.isEmpty())
    {
        button->setIcon(QIcon(iconPath));
    }
    //Resize the button.
    button->setFixedSize(16, 16);
    //Give back the button.
    return button;
}

inline KNOpacityButton *KNMusicMiniPlayer::generateButton(
        const QString &iconPath)
{
    //Generate the button.
    KNOpacityButton *button=new KNOpacityButton(this);
    //Set the icon.
    if(!iconPath.isEmpty())
    {
        button->setIcon(QIcon(iconPath));
    }
    //Resize the button.
    button->setFixedSize(16, 16);
    //Give back the button.
    return button;
}

inline void KNMusicMiniPlayer::thawAnime()
{
    //Check the freezing state.
    if(!m_freeze)
    {
        //If the mini player is not freeze, we don't need to thaw.
        return;
    }
    //Reset the freeze state.
    m_freeze=false;
    //Check the position.
    if(!rect().contains(mapFromGlobal(QCursor::pos())))
    {
        //When the mouse is not inside the mini player, then start the hiding
        //anime.
        startAnime(0);
    }
}

inline void KNMusicMiniPlayer::startAnime(int targetFrame)
{
    //Stop the time line.
    m_moving->stop();
    //Configure the time line.
    m_moving->setFrameRange(m_lyrics->y(), targetFrame);
    //Start the time line.
    m_moving->start();
}

void KNMusicMiniPlayer::setPosition(const qint64 &position)
{
    //Check the backend before.
    if(m_backend)
    {
        //Call the set position of the backend.
        m_backend->setPosition(position);
    }
}

inline void KNMusicMiniPlayer::setCacheValue(const QString &valueName,
                                             const int &value)
{
    m_cacheConfigure->setData(valueName, value);
}

inline void KNMusicMiniPlayer::zoomParameter(int &parameter, const qreal &ratio)
{
    parameter=(qreal)parameter*ratio;
}

inline int KNMusicMiniPlayer::getCacheValue(const QString &valueName)
{
    return m_cacheConfigure->data(valueName).toInt();
}
