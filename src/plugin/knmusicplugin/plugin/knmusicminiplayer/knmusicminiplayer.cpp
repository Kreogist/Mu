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
#include <QBoxLayout>
#include <QTimeLine>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>

#include "knimagelabel.h"
#include "knopacityanimebutton.h"
#include "knthememanager.h"
#include "knprogressslider.h"
#include "kneditablelabel.h"

#include "knmusicbackend.h"
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
    m_previous(generateButton(":/plugin/music/player/mini_backward.png")),
    m_playNPause(generateButton()),
    m_next(generateButton(":/plugin/music/player/mini_forward.png")),
    m_mute(generateButton()),
    m_restore(generateButton(":/plugin/music/player/maximal_mini.png")),
    m_close(generateButton(":/plugin/music/player/close_mini.png")),
    m_progressSlider(new KNProgressSlider(this)),
    m_lyrics(new KNMusicHScrollLyrics(this)),
    m_moving(new QTimeLine(200, this)),
    m_backend(nullptr),
    m_progressPressed(false),
    m_pressed(false)
{
    setObjectName("MiniPlayer");
    //Set property.
    setAutoFillBackground(true);
#ifdef Q_OS_MACX
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#else
    setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
#endif
    setFixedSize(fontMetrics().height()*28, fontMetrics().height()<<1);
    //Initial the resource.
    // Mute icons.
    m_muteIcon[0]=QIcon(":/plugin/music/player/mute_false.png");
    m_muteIcon[1]=QIcon(":/plugin/music/player/mute_true.png");
    // Play and pause icons.
    m_iconPause=QIcon(":/plugin/music/player/mini_pause.png");
    m_iconPlay=QIcon(":/plugin/music/player/mini_play.png");

    //Configure the container.
    m_container->setFixedSize(size());
    m_container->move(0, -height());
    //Configure the icon label.
    m_icon->setPixmap(QPixmap(":/plugin/preference/about.png"));
    m_icon->setFixedSize(16, 16);
    m_icon->setCursor(Qt::SizeAllCursor);
    //Configure default icons.
    m_playNPause->setIcon(m_iconPlay);
    m_mute->setIcon(m_muteIcon[false]);
    //Configure progress bar.
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
    containerLayout->setSpacing(0);
    m_container->setLayout(containerLayout);
    //Add widgets to container layout.
    containerLayout->addWidget(m_icon);
    containerLayout->addWidget(m_previous);
    containerLayout->addWidget(m_playNPause);
    containerLayout->addWidget(m_next);
    containerLayout->addWidget(m_mute);
    containerLayout->addStretch();
    containerLayout->addWidget(m_position);
    containerLayout->addWidget(m_progressSlider);
    containerLayout->addWidget(m_restore);
    containerLayout->addWidget(m_close);

    //Register the theme.
    knTheme->registerWidget(this);
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
    //Link the play and pause button.
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
    ;
}

void KNMusicMiniPlayer::enterEvent(QEvent *event)
{
    //Do original event.
    KNMusicMiniPlayerBase::enterEvent(event);
    //Move to hide the lyrics widget.
    startAnime(height());
}

void KNMusicMiniPlayer::leaveEvent(QEvent *event)
{
    //Do original event
    KNMusicMiniPlayerBase::leaveEvent(event);
    //Move to show the lyrics widget.
    startAnime(0);
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

inline KNOpacityAnimeButton *KNMusicMiniPlayer::generateButton(
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
