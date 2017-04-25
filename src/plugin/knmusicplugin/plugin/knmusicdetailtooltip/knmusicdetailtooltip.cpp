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
#include <QApplication>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QTimer>
#include <QTimeLine>

#include "knscrolllabel.h"
#include "knthememanager.h"
#include "knopacitybutton.h"
#include "knprogressslider.h"
#include "knglobal.h"

#include "knmusicbackend.h"
#include "knmusicalbumlabel.h"
#include "knmusicglobal.h"
#include "knmusicparser.h"
#include "knmusicmodel.h"

#include "knmusicdetailtooltip.h"

#include <QDebug>

#define TooltipWidth 448
#define TooltipHeight 176
#define LabelWidth TooltipWidth-TooltipHeight-11
#define NormalFadeOutTime 1500
#define ShortFadeOutTime 300

KNMusicDetailTooltip::KNMusicDetailTooltip(QWidget *parent) :
    KNMusicDetailTooltipBase(parent),
    m_fadeOutCounter(new QTimer(this)),
    m_mouseIn(generateTimeLine(0x50)),
    m_mouseOut(generateTimeLine(0x20)),
    m_albumArt(new KNMusicAlbumLabel(this)),
    m_playNPause(new KNOpacityButton(this)),
    m_progress(new KNProgressSlider(this)),
    m_playIcon(QIcon(":/plugin/music/player/play_light.png")),
    m_pauseIcon(QIcon(":/plugin/music/player/pause_light.png")),
    m_isPlaying(false),
    m_progressPressed(false),
    m_detailInfo(KNMusicDetailInfo())
{
    setObjectName("MusicDetailTooltip");
    //Set properties.
    setAutoFillBackground(true);
    setWindowFlags(Qt::ToolTip);
    setFixedSize(TooltipWidth, TooltipHeight);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(11,11,11,11);
    mainLayout->setSpacing(6);
    setLayout(mainLayout);
    //Initial the album art.
    m_albumArt->setFixedSize(154, 154);
    mainLayout->addWidget(m_albumArt);
    //Generate the label container.
    QWidget *labelContainer=new QWidget(this);
    labelContainer->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(labelContainer, 1);
    //Initial the label layout.
    QBoxLayout *labelLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                           mainLayout->widget());
    labelLayout->setContentsMargins(0,5,0,5);
    labelLayout->setSpacing(0);
    labelLayout->setSizeConstraint(QLayout::SetMaximumSize);
    labelContainer->setLayout(labelLayout);
    //Initial the detail info labels.
    for(int i=0; i<ToolTipItemsCount; i++)
    {
        //Initial the label.
        m_labels[i]=new KNScrollLabel(this);
        //Configure the label.
        m_labels[i]->setContentsMargins(0,0,0,0);
        //Resize the labels.
        m_labels[i]->setFixedWidth(LabelWidth);
        //Add the label to the layout.
        labelLayout->addWidget(m_labels[i]);
    }
    labelLayout->addStretch();
    //Configure the font of title.
    QFont nameFont=m_labels[ItemTitle]->font();
    nameFont.setBold(true);
    nameFont.setPixelSize(18);
    m_labels[ItemTitle]->setFont(nameFont);
    //Initial the preview player.
    QBoxLayout *previewPlayer=new QBoxLayout(QBoxLayout::LeftToRight,
                                             labelLayout->widget());
    previewPlayer->setContentsMargins(0,0,0,0);
    previewPlayer->setSpacing(0);
    labelLayout->addLayout(previewPlayer);
    //Configure preview player control button.
    m_playNPause->setFocusProxy(this);
    m_playNPause->setFixedSize(20, 20);
    m_playNPause->setIcon(m_playIcon);
    //Link clicked request.
    connect(m_playNPause, &KNOpacityButton::clicked,
            this, &KNMusicDetailTooltip::onActionPlayNPauseClicked);
    //Add to player layout.
    previewPlayer->addWidget(m_playNPause);
    //Configure the progress slider.
    m_progress->setMaximum(0);
    m_progress->setWheelStep(1000);
    m_progress->setFocusProxy(this);
    connect(m_progress, &KNProgressSlider::sliderPressed,
            [=]{m_progressPressed=true;});
    connect(m_progress, &KNProgressSlider::sliderReleased,
            [=]{m_progressPressed=false;});
    previewPlayer->addWidget(m_progress, 1);

    //Configure the timer.
    m_fadeOutCounter->setSingleShot(true);
    m_fadeOutCounter->setTimerType(Qt::VeryCoarseTimer);
    connect(m_fadeOutCounter, &QTimer::timeout,
            this, &KNMusicDetailTooltip::onActionHide);

    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicDetailTooltip::onActionThemeChanged);
    //Initial the palette.
    onActionMouseInOut(0x20);
    //Update the theme.
    onActionThemeChanged();
}

void KNMusicDetailTooltip::showTooltip(const QPoint &position)
{
    //Set the position.
    moveToPosition(position);
    //Show the tooltip box if it didn't display before.
    if(!isVisible())
    {
        show();
    }
    //Reset the fade out counter.
    resetCounter();
    //Start the fade out counter.
    m_fadeOutCounter->start();
}

void KNMusicDetailTooltip::setPreviewIndex(KNMusicModel *musicModel,
                                           const QModelIndex &index)
{
    //Clear the label information.
    //Generate an analysis item.
    KNMusicAnalysisItem item;
    //Set the detail info to item.
    item.detailInfo=musicModel->rowDetailInfo(index.row());
    //Reanalysis the item.
    if(knMusicGlobal->parser()->reanalysisItem(item))
    {
        //Save the detail info.
        m_detailInfo=item.detailInfo;
        //Check the cover image.
        if(item.coverImage.isNull())
        {
            //Draw the no album art.
            m_albumArt->setPixmap(knMusicGlobal->noAlbumArt());
        }
        else
        {
            //Set the image to the album art.
            m_albumArt->setPixmap(QPixmap::fromImage(item.coverImage));
        }
        //Get the detail info of the item.
        const KNMusicDetailInfo &detailInfo=item.detailInfo;
        //Set text data to labels.
        m_labels[ItemTitle]->setText(detailInfo.textLists[Name].toString());
        m_labels[ItemTime]->setText(detailInfo.textLists[Time].toString());
        m_labels[ItemArtist]->setText(detailInfo.textLists[Artist].toString());
    }
    else
    {
        //Reset detail info.
        m_detailInfo=KNMusicDetailInfo();
        //Set album detail info to be no album art.
        m_albumArt->setPixmap(knMusicGlobal->noAlbumArt());
        //Clear all the labels.
        m_labels[ItemTitle]->setText("");
        m_labels[ItemTime]->setText("");
        m_labels[ItemArtist]->setText("");
    }
}

void KNMusicDetailTooltip::enterEvent(QEvent *event)
{
    //Do enter event first.
    QWidget::enterEvent(event);
    //Stop the disappear counter.
    m_fadeOutCounter->stop();
    //Magic Interval:
    /*
     * Reduce the disappear timer interval. This is called magic interval, when
     * user move there mouse in and then move mouse out.
     */
    m_fadeOutCounter->setInterval(ShortFadeOutTime);
    //Start mouse in animation.
    startAnime(m_mouseIn);
    //When mouse move in, it means user may want to preview the song, load the
    //music here.
    loadPreview();
    //Set focus.
    setFocus();
}

void KNMusicDetailTooltip::leaveEvent(QEvent *event)
{
    //Do leave event first.
    QWidget::leaveEvent(event);
    //Start mouse out animation.
    startAnime(m_mouseOut);
    //Start the fade out counter.
    m_fadeOutCounter->start();
}

void KNMusicDetailTooltip::focusOutEvent(QFocusEvent *event)
{
    //Do focus out event first.
    QWidget::focusOutEvent(event);
    //Start mouse out animation.
    startAnime(m_mouseOut);
    //Start the fade out counter.
    m_fadeOutCounter->start();
}

void KNMusicDetailTooltip::onActionHide()
{
    //Reset the player.
    m_playNPause->setIcon(m_playIcon);
    //Reset the progress.
    m_progress->setMaximum(0);
    //Disconnect all the connection handle.
    m_backendHandler.disconnectAll();
    //Reset the backend.
    if(knMusicGlobal->backend())
    {
        //Reset the preview.
        knMusicGlobal->backend()->previewReset();
    }
    //Hide the tooltip.
    hide();
}

void KNMusicDetailTooltip::onActionMouseInOut(int frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Update the background color.
    QColor color=pal.color(QPalette::Window);
    color.setHsv(color.hue(), color.saturation(), frame);
    pal.setColor(QPalette::Window, color);
    //Update the label text color.
    color=pal.color(QPalette::WindowText);
    color.setHsv(color.hue(), color.saturation(), (frame<<1)+0x5F);
    pal.setColor(QPalette::WindowText, color);
    //Set palette for all widget.
    setPalette(pal);
    for(int i=0; i<ToolTipItemsCount; i++)
    {
        m_labels[i]->setPalette(pal);
    }
}

void KNMusicDetailTooltip::onActionThemeChanged()
{
    //Backup the current brightness.
    int brightness=palette().color(QPalette::Window).value();
    //Set the latest palette.
    setPalette(knTheme->getPalette(objectName()));
    //Update the brightness.
    onActionMouseInOut(brightness);
}

void KNMusicDetailTooltip::onActionPlayNPauseClicked()
{
    //Check the backend.
    if(knMusicGlobal->backend())
    {
        //Get the backend.
        KNMusicBackend *backend=knMusicGlobal->backend();
        //Check the state.
        if(backend->previewState()==Playing)
        {
            //Pause when playing.
            backend->previewPause();
        }
        else
        {
            //Play when pausing or stopped.
            backend->previewPlay();
        }
    }
}

void KNMusicDetailTooltip::onActionPreviewPositionChanged(
        const qint64 &position)
{
    //Check and ensure the progress is not being pressed.
    if(!m_progressPressed)
    {
        //Change the value of the progress.
        m_progress->setValue(position);
    }
}

void KNMusicDetailTooltip::onActionPreviewDurationChanged(
        const qint64 &position)
{
    //Update the range of the progress.
    m_progress->setMaximum(position);
}

void KNMusicDetailTooltip::onActionPreviewStateChange(const int &state)
{
    //Update the icon of the button.
    m_playNPause->setIcon(state==Playing?m_pauseIcon:m_playIcon);
}

inline QTimeLine *KNMusicDetailTooltip::generateTimeLine(const int &endFrame)
{
    //Generate time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configure the time line.
    timeLine->setEndFrame(endFrame);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(16);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNMusicDetailTooltip::onActionMouseInOut);
    //Give back the time line.
    return timeLine;
}

inline void KNMusicDetailTooltip::startAnime(QTimeLine *timeLine)
{
    //Stop all the time line.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Configure the time line.
    timeLine->setStartFrame(palette().color(QPalette::Window).value());
    //Start time line.
    timeLine->start();
}

inline void KNMusicDetailTooltip::moveToPosition(const QPoint &position)
{
    //Get the desktop widget.
    QDesktopWidget *desktopWidget=qApp->desktop();
    //Get the screen.
    QRect screenRect=desktopWidget->screenGeometry(knGlobal->mainWindow());
    //Move right of the tooltip for a little to avoid the mouse pointer.
    int preferX=position.x()+14, preferY=position.y()-(TooltipHeight>>1),
        screenHeight=screenRect.bottom(),
        screenWidth=screenRect.right();
    //Move to the right position.
    move(preferX+TooltipWidth>screenWidth?
             screenWidth-TooltipWidth-5:preferX,
         preferY+TooltipHeight>screenHeight?
             screenHeight-TooltipHeight:preferY);
}

inline void KNMusicDetailTooltip::resetCounter()
{
    //Stop the counter and set the interval.
    m_fadeOutCounter->stop();
    m_fadeOutCounter->setInterval(NormalFadeOutTime);
}

void KNMusicDetailTooltip::loadPreview()
{
    //Check if the backend, music model and current index is still valid.
    if(knMusicGlobal->backend() && QFileInfo::exists(m_detailInfo.filePath))
    {
        //Get the backend.
        KNMusicBackend *backend=knMusicGlobal->backend();
        //Link the backend with the preview widget.
        m_backendHandler.append(
          connect(backend, &KNMusicBackend::previewPositionChanged,
                  this, &KNMusicDetailTooltip::onActionPreviewPositionChanged));
        m_backendHandler.append(
          connect(backend, &KNMusicBackend::previewDurationChanged,
                  this, &KNMusicDetailTooltip::onActionPreviewDurationChanged));
        m_backendHandler.append(
          connect(backend, &KNMusicBackend::previewPlayingStateChanged,
                  this, &KNMusicDetailTooltip::onActionPreviewStateChange));
        m_backendHandler.append(
          connect(m_progress, &KNProgressSlider::sliderMoved,
                  backend, &KNMusicBackend::setPreviewPosition));
        //Load the music to preview thread.
        backend->previewLoadMusic(m_detailInfo.filePath,
                                  m_detailInfo.startPosition,
                                  m_detailInfo.duration);
    }
}
