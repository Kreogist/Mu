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
#include "knmusicglobal.h"
#include "knmusiclyricsmanager.h"
#include "knmusicminiplayerlyrics.h"

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
    m_lyrics(new KNMusicMiniPlayerLyrics(this)),
    m_moving(new QTimeLine(200, this)),
    m_backend(nullptr),
    m_nowPlaying(nullptr),
    m_cacheConfigure(knGlobal->cacheConfigure()->getConfigure("MiniPlayer")),
    m_minimalX(0),
    m_minimalY(0),
    m_progressPressed(false),
    m_pressed(false),
    m_freeze(false)
{
    setObjectName("MiniPlayer");
    //Set property.
    setAutoFillBackground(true);
    setAcceptDrops(true);
#ifdef Q_OS_LINUX
    setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
#else
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
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
            this, &KNMusicMiniPlayer::onActionClose);
    //Configure the scroll lyrics.
    m_lyrics->setObjectName("MiniLyrics");
    m_lyrics->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_lyrics->setBackend(knMusicGlobal->lyricsManager()->backend());
    m_lyrics->setFixedSize(size());
    m_lyrics->setFocusProxy(this);
    knTheme->registerWidget(m_lyrics);
    connect(m_lyrics, &KNMusicMiniPlayerLyrics::urlsDropped,
            this, &KNMusicMiniPlayer::urlsDropped);
    //Configure the time line.
    m_moving->setEasingCurve(QEasingCurve::OutCubic);
    m_moving->setUpdateInterval(16);
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
    //If the system is UNIX-like, we have to check out the desktop environment.
#ifdef Q_OS_UNIX
    configureMinimalXAndY();
#endif
    //Move to default position: top left of the desktop.
    move(m_minimalX, m_minimalY);
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
            m_backend, &KNMusicBackend::playNPause);
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
                //Stop label animation if it's not shown.
                if(m_lyrics->y()==0)
                {
                    //Stop label animation.
                    m_detailLabel->stopAnime();
                }
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
    //Get the desktop widget.
    QDesktopWidget *desktopWidget=qApp->desktop();
    //Get the screen index.
    int screenIndex=desktopWidget->screenNumber(this);
    //Save the index.
    setCacheValue("miniPlayerScreenIndex", screenIndex);
    //Get the screen.
    QRect screenRect=desktopWidget->screenGeometry(screenIndex);
    //Save the screen size.
    setCacheValue("miniPlayerScreenX", screenRect.x());
    setCacheValue("miniPlayerScreenY", screenRect.y());
    setCacheValue("miniPlayerScreenWidth", screenRect.width());
    setCacheValue("miniPlayerScreenHeight", screenRect.height());
    //Save the current player position.
    setCacheValue("miniPlayerX", geometry().x());
    setCacheValue("miniPlayerY", geometry().y());
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

    //Read the screen information.
    int screenIndex=getCacheValue("miniPlayerScreenIndex"),
        lastScreenX=getCacheValue("miniPlayerScreenX"),
        lastScreenY=getCacheValue("miniPlayerScreenY"),
        lastScreenWidth=getCacheValue("miniPlayerScreenWidth"),
        lastScreenHeight=getCacheValue("miniPlayerScreenHeight"),
        lastX=getCacheValue("miniPlayerX"),
        lastY=getCacheValue("miniPlayerY"),
        targetScreenX=0,
        targetScreenY=0, targetScreenWidth=1, targetScreenHeight=1,
        playerWidth=width(),
        playerHeight=height();
    //Check whether we have this screen anymore or not.
    //Get the desktop widget.
    QDesktopWidget *desktopWidget=qApp->desktop();
    //Check the desktop screen size.
    if(screenIndex<desktopWidget->screenCount())
    {
        //The screen is still exist.
        //Get the screen rect.
        QRect screenRect=desktopWidget->screenGeometry(screenIndex);
        //Check whether the screen is still valid.
        if(!screenRect.isEmpty())
        {
            //The resolution may be changed or not, if changed, we need to
            //recalculate the size according to the new resolution.
            //If it is not changed, we just move the player to there.
            targetScreenX=screenRect.x();
            targetScreenY=screenRect.y();
            targetScreenWidth=screenRect.width();
            targetScreenHeight=screenRect.height();
            //Check the resolution of the screen.
            //Compare the resolution.
            if(targetScreenWidth==lastScreenWidth &&
                    targetScreenHeight==lastScreenHeight)
            {
                //Check last X and last Y validation.
                //Check last X is out of border.
                if(lastX<targetScreenX)
                {
                    //Update the last X to 0.
                    lastX=targetScreenX;
                }
                else if(lastX>targetScreenX+targetScreenWidth-playerWidth)
                {
                    //Update the last X to right most.
                    lastX=targetScreenX+targetScreenWidth-playerWidth;
                }
                //Check last Y is out of border.
                if(lastY<targetScreenY)
                {
                    //Update the last Y to top.
                    lastY=targetScreenY;
                }
                else if(lastY>targetScreenHeight-playerHeight)
                {
                    //Update the last Y to bottom.
                    lastY=targetScreenHeight-playerHeight;
                }
                //Move the position to the target position.
                move(lastX, lastY);
                return;
            }
        }
    }
    else
    {
        //The screen is not exist anymore, set the target screen to be the
        //default screen.
        QRect screenRect=desktopWidget->screenGeometry();
        //Check default screen rect is empty or not.
        if(screenRect.isEmpty())
        {
            //If the screen is invalid, then igonre the request.
            return;
        }
        //Set the current screen information.
        targetScreenX=screenRect.x();
        targetScreenY=screenRect.y();
        targetScreenWidth=screenRect.width();
        targetScreenHeight=screenRect.height();
    }
    //Update the last X and Y to screen position.
    lastX-=lastScreenX;
    lastY-=lastScreenY;
    //Check special position.
    //Left.
    if(lastX==0)
    {
        //Top.
        if(lastY==0)
        {
            //Move to top left.
            move(targetScreenX, targetScreenY);
            return;
        }
        //Bottom.
        if(lastY==lastScreenHeight-playerHeight)
        {
            //Move to bottom left.
            move(targetScreenX, targetScreenHeight-playerHeight);
            return;
        }
    }
    //Right.
    if(lastX==lastScreenWidth-playerWidth)
    {
        //Top.
        if(lastY==0)
        {
            //Move to top left.
            move(targetScreenX-playerWidth, targetScreenY);
            return;
        }
        //Bottom.
        if(lastY==lastScreenHeight-playerHeight)
        {
            //Move to bottom left.
            move(targetScreenX-playerWidth, targetScreenHeight-playerHeight);
            return;
        }
    }
    //Recalculate the last parameters.
    zoomParameter(lastX, (qreal)targetScreenWidth/(qreal)lastScreenWidth);
    zoomParameter(lastY, (qreal)targetScreenHeight/(qreal)lastScreenHeight);
    //Check the parameter, ensure that one part of the window must be inside
    //screen.
    //If it's not inside the screen, make the top of the window 0.
    if(lastX<m_minimalX)
    {
        lastX=m_minimalX;
    }
    else if(lastX>targetScreenWidth)
    {
        lastX=targetScreenWidth-playerWidth;
    }
    if(lastY<m_minimalY)
    {
        lastY=m_minimalY;
    }
    else if(lastY>targetScreenHeight)
    {
        lastY=targetScreenHeight-playerHeight;
    }
    //Set the geometry.
    move(targetScreenX+lastX, targetScreenY+lastY);
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
    //Check lyrics widget position.
    if(m_lyrics->y()==0)
    {
        //Show up the control panel.
        startAnime(height());
    }
    //Set pressed flag.
    m_pressed=true;
    //Save the pressed point.
    m_pressedPoint=event->pos();
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
        QPoint globalClickPos(event->globalPos()),
               targetPosition(globalClickPos-m_pressedPoint);
        //Get the desktop widget.
        int desktopRight=1, desktopHeight=1, desktopLeft=0;
        {
            //FIXED 8th Apr, 2016
            //On OS X 10.11, desktop size will be fixed 640x480. It should be a
            //bug under OS X 10.11 for Qt 5.6. (Untest for Qt 5.5)
            //Fixed by using screenGeometry() of desktop widget.
            //Get the desktop widget.
            QDesktopWidget *desktop=qApp->desktop();
            //Check desktop is null or not.
            if(!desktop)
            {
                //Failed to read the desktop information.
                return;
            }
            //Check the mouse is in which screen.
            for(int i=0; i<desktop->screenCount(); ++i)
            {
                //Check mouse position.
                QRect currentScreen=desktop->screenGeometry(i);
                //Check whether current screen is include the pointer.
                if(currentScreen.contains(globalClickPos))
                {
                    //Save the desktop left.
                    desktopLeft=currentScreen.left();
                    //Save the width and height.
                    desktopRight=currentScreen.right();
                    desktopHeight=currentScreen.height();
                    break;
                }
            }
        }
        //Check target position.
        if(targetPosition.y()<m_minimalY+DetectMargin)
        {
            targetPosition.setY(m_minimalY);
        }
        else if(targetPosition.y()+height()+DetectMargin>desktopHeight)
        {
            targetPosition.setY(desktopHeight-height());
        }
        if(targetPosition.x()<desktopLeft+m_minimalX+DetectMargin)
        {
            targetPosition.setX(desktopLeft+m_minimalX);
        }
        else if(targetPosition.x()+width()+DetectMargin>desktopRight)
        {
            targetPosition.setX(desktopRight-width()+1);
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
    //Change the background color.
    QPalette pal=palette();
    QColor backColor=pal.color(QPalette::Window);
    backColor.setHsv(backColor.hue(),
                     backColor.saturation(),
                     (qreal)frame/(qreal)height()*70.0);
    pal.setColor(QPalette::Window, backColor);
    setPalette(pal);
}

void KNMusicMiniPlayer::onActionShowLyrics()
{
    //Stop the scroll label animation.
    m_detailLabel->stopAnime();
    //Disconnect the time line finish signal.
    m_animeHandler.disconnectAll();
}

void KNMusicMiniPlayer::onActionHideLyrics()
{
    //Start the scroll label animation.
    m_detailLabel->startAnime();
    //Disconnect the time line finish signal.
    m_animeHandler.disconnectAll();
}

void KNMusicMiniPlayer::onActionClose()
{
#ifndef Q_OS_LINUX
    //Close mini player first.
    close();
#endif
    //Ask to close the main window.
    emit requireCloseMainWindow();
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

#ifdef Q_OS_UNIX
inline void KNMusicMiniPlayer::configureMinimalXAndY()
{
    //Get the shell information.
    int desktopShell=knGlobal->desktopEnvironment();
    //Check the validation of the desktop shell.
    if(desktopShell==KNGlobal::NullShell)
    {
        //We cannot detect the shell, ignore the process.
        return;
    }
    //Check the desktop shell.
    switch(desktopShell)
    {
    case KNGlobal::GnomeShell:
        m_minimalY=20;
        break;
    }
}
#endif

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
    //Disconnect all.
    m_animeHandler.disconnectAll();
    //Stop the time line.
    m_moving->stop();
    //Configure the time line.
    m_moving->setFrameRange(m_lyrics->y(), targetFrame);
    //Link the time line with specific finish slot.
    if(targetFrame==0)
    {
        //When target frame is 0, means the lyrics will be shown.
        m_animeHandler.append(
                    connect(m_moving, &QTimeLine::finished,
                            this, &KNMusicMiniPlayer::onActionShowLyrics));
    }
    else
    {
        //When target frame is not 0, means the lyrics will be hidden.
        m_animeHandler.append(
                    connect(m_moving, &QTimeLine::finished,
                            this, &KNMusicMiniPlayer::onActionHideLyrics));
    }
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
