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
#include <QLabel>
#include <QTimer>
#include <QTimeLine>

#include "knthememanager.h"
#include "knopacitybutton.h"
#include "knprogressslider.h"

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
    m_playIcon(QIcon(":/plugin/music/player/play.png")),
    m_pauseIcon(QIcon(":/plugin/music/player/pause.png")),
    m_isPlaying(false),
    m_progressPressed(false),
    m_previewMusicModel(nullptr),
    m_previewIndex(QPersistentModelIndex()),
    m_backend(nullptr)
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
    labelLayout->setContentsMargins(0,0,0,0);
    labelLayout->setSizeConstraint(QLayout::SetMaximumSize);
    labelContainer->setLayout(labelLayout);
    //Initial the detail info labels.
    for(int i=0; i<ToolTipItemsCount; i++)
    {
        //Initial the label.
        m_labels[i]=new QLabel(this);
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
    previewPlayer->addWidget(m_playNPause);
    //Configure the progress slider.
    m_progress->setMaximum(0);
    m_progress->setWheelStep(1000);
    connect(m_progress, &KNProgressSlider::sliderPressed,
            [=]{m_progressPressed=true;});
    connect(m_progress, &KNProgressSlider::sliderReleased,
            [=]{m_progressPressed=false;});
    previewPlayer->addWidget(m_progress, 1);

    //Configure the timer.
    m_fadeOutCounter->setSingleShot(true);
    connect(m_fadeOutCounter, &QTimer::timeout,
            this, &KNMusicDetailTooltip::onActionHide);

    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicDetailTooltip::onActionThemeChanged);
    onActionThemeChanged();
    //Initial the palette.
    onActionMouseInOut(0x20);

    //Check the backend.
    if(knMusicGlobal->backend())
    {
        //Save the backend.
        m_backend=knMusicGlobal->backend();
    }
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
                                           const QPersistentModelIndex &index)
{
    //Generate an analysis item.
    KNMusicAnalysisItem item;
    //Set the detail info to item.
    item.detailInfo=musicModel->rowDetailInfo(index.row());
    //Reanalysis the item.
    if(knMusicGlobal->parser()->reanalysisItem(item))
    {
        //Save the music model and the index.
        m_previewMusicModel=musicModel;
        m_previewIndex=index;
        //Check the cover image.
        if(item.coverImage.isNull())
        {
            m_albumArt->setArtwork(knMusicGlobal->noAlbumArt());
        }
        else
        {
            //Set the image to the album art.
            m_albumArt->setArtwork(QPixmap::fromImage(item.coverImage));
        }
        //Get the detail info of the item.
        const KNMusicDetailInfo &detailInfo=item.detailInfo;
        //Set text data to labels.
        setEliedText(m_labels[ItemTitle],
                     detailInfo.textLists[Name].toString());
        setEliedText(m_labels[ItemTime],
                     detailInfo.textLists[Time].toString());
        setEliedText(m_labels[ItemArtist],
                     detailInfo.textLists[Artist].toString());
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
    //Set focus.
    setFocus();
    //When mouse move in, it means user may want to preview the song, load the
    //music here.
    loadPreview();
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
    ;
    //Hide the tooltip.
    hide();
}

void KNMusicDetailTooltip::onActionMouseInOut(const int &frame)
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

QTimeLine *KNMusicDetailTooltip::generateTimeLine(const int &endFrame)
{
    //Generate time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configure the time line.
    timeLine->setEndFrame(endFrame);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(10);
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
    //Move right of the tooltip for a little to avoid the mouse pointer.
    int preferX=position.x()+14, preferY=position.y()-(TooltipHeight>>1),
        screenHeight=QApplication::desktop()->height(),
        screenWidth=QApplication::desktop()->width();
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

void KNMusicDetailTooltip::setEliedText(QLabel *label, const QString &text)
{
    //Calculate the size of the text using the fontMetrics of the label.
    if(label->fontMetrics().width(text) > LabelWidth)
    {
        //Set the elied text to the label, add the text as tooltip of it.
        label->setText(label->fontMetrics().elidedText(text,
                                                       Qt::ElideRight,
                                                       LabelWidth));
        label->setToolTip(text);
        return;
    }
    //Clear the tooltip and set text.
    label->setText(text);
    label->setToolTip("");
}

void KNMusicDetailTooltip::loadPreview()
{
    //Check if the backend, music model and current index is still valid.
    if(m_backend && m_previewMusicModel && m_previewIndex.isValid())
    {
        //Get the detail info of the model.
        KNMusicDetailInfo detailInfo=
                m_previewMusicModel->rowDetailInfo(m_previewIndex.row());
        //Load the music to preview thread.
        m_backend->previewLoadMusic(detailInfo.filePath,
                                    detailInfo.startPosition,
                                    detailInfo.duration);
    }
}
