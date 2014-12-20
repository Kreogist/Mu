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
#include <QFocusEvent>
#include <QLabel>
#include <QTimer>
#include <QTimeLine>

#include "knopacitybutton.h"
#include "knprogressslider.h"
#include "knfilepathlabel.h"
#include "knmusicdetailtooltipartwork.h"
#include "knmusicmodelassist.h"
#include "knmusicmodel.h"
#include "knmusicbackend.h"

#include "knmusicdetailtooltip.h"

#include <QDebug>

KNMusicDetailTooltip::KNMusicDetailTooltip(QWidget *parent) :
    KNMusicDetailTooltipBase(parent)
{
    //Set properties.
    setFixedSize(m_tooltipWidth, m_tooltipHeight);
    setWindowFlags(Qt::ToolTip);
    //Initial the resource.
    m_playIcon=QPixmap(":/plugin/music/preview/play.png");
    m_pauseIcon=QPixmap(":/plugin/music/preview/pause.png");

    //Initial the counter.
    m_disappearCounter=new QTimer(this);
    m_disappearCounter->setSingleShot(true);
    connect(m_disappearCounter, &QTimer::timeout,
            this, &KNMusicDetailTooltip::onActionHide);

    //Initial the layout and widget.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(11,11,11,11);
    mainLayout->setSpacing(6);
    setLayout(mainLayout);
    //Initial the album art.
    m_albumArt=new KNMusicDetailTooltipArtwork(this);
    m_albumArt->setArtworkSize(154, 154);
    mainLayout->addWidget(m_albumArt);

    QWidget *labelContainer=new QWidget(this);
    labelContainer->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(labelContainer, 1);

    QBoxLayout *labelLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                           mainLayout->widget());
    labelLayout->setContentsMargins(0,0,0,0);
    labelLayout->setSizeConstraint(QLayout::SetMaximumSize);
    labelContainer->setLayout(labelLayout);
    //Initial the detail info labels.
    for(int i=0; i<ToolTipItemsCount; i++)
    {
        m_labels[i]=new QLabel(this);
        m_labels[i]->setFixedWidth(m_labelWidth);
        m_labels[i]->setPalette(m_palette);
        labelLayout->addWidget(m_labels[i]);
    }
    //Set the title label font.
    QFont nameFont=m_labels[ItemTitle]->font();
    nameFont.setBold(true);
    nameFont.setPixelSize(18);
    m_labels[ItemTitle]->setFont(nameFont);
    //Initial the file path label.
    m_fileName=new KNFilePathLabel(this);
    m_fileName->setPalette(m_palette);
    connect(m_fileName, &KNFilePathLabel::aboutToShowInGraphicalShell,
            this, &KNMusicDetailTooltip::startDisappearCountWithAnime);
    labelLayout->insertWidget(1, m_fileName);
    labelLayout->addStretch();

    //Initial the preview player.
    m_previewPlayer=new QBoxLayout(QBoxLayout::LeftToRight,
                                   mainLayout->widget());
    m_previewPlayer->setSpacing(0);
    m_previewPlayer->setContentsMargins(0,0,0,0);
    labelLayout->addLayout(m_previewPlayer);
    m_playNPause=new KNOpacityButton(this);
    m_playNPause->setFocusProxy(this);
    m_playNPause->setFixedSize(20, 20);
    m_playNPause->setIcon(m_playIcon);
    connect(m_playNPause, &KNOpacityButton::clicked,
            this, &KNMusicDetailTooltip::onActionPlayNPauseClick);
    m_previewPlayer->addWidget(m_playNPause);
    m_progress=new KNProgressSlider(this);
    m_progress->setFocusProxy(this);
    m_progress->setWheelStep(1000);
    connect(m_progress, &KNProgressSlider::sliderPressed,
            this, &KNMusicDetailTooltip::onActionProgressPressed);
    connect(m_progress, &KNProgressSlider::sliderReleased,
            this, &KNMusicDetailTooltip::onActionProgressReleased);
    m_previewPlayer->addWidget(m_progress, 1);

    //Initial the mouse in/out timeline.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(0x50);
    initialTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(0x28);
    initialTimeLine(m_mouseOut);

    //Initial the color and palette, initial the palette with frame slot.
    m_palette=palette();
    onActionMouseInOut(0x28);
}

void KNMusicDetailTooltip::setBackend(KNMusicBackend *backend)
{
    m_backend=backend;
    //Connect to backend.
    connect(m_backend, &KNMusicBackend::previewPositionChanged,
            this, &KNMusicDetailTooltip::onActionPreviewPositionChanged);
    connect(m_backend, &KNMusicBackend::previewPlayingStateChanged,
            this, &KNMusicDetailTooltip::onActionPreviewStatusChange);
    connect(m_progress, &KNProgressSlider::sliderMoved,
            m_backend, &KNMusicBackend::setPreviewPosition);
}

void KNMusicDetailTooltip::showTooltip()
{
    //Show the tooltip box if it didn't display before.
    if(!isVisible())
    {
        show();
    }
    //Reset the counter.
    resetDisappearCounter();
    m_disappearCounter->start();
}

void KNMusicDetailTooltip::setPreviewIndex(KNMusicModel *musicModel,
                                           const QPersistentModelIndex &index,
                                           const QPoint &position)
{
    //Check the row.
    if(m_currentMusicModel==musicModel &&
            m_currentIndex.row()==index.row())
    {
        return;
    }
    //Set the current row and model.
    m_currentIndex=index;
    m_currentMusicModel=musicModel;
    //Get the detail from data.
    KNMusicDetailInfo detailInfo;
    if(KNMusicModelAssist::reanalysisRow(musicModel,
                                         index,
                                         detailInfo))
    {
        //Set data to details.
        m_albumArt->setArtwork(detailInfo.coverImage.isNull()?
                                   KNMusicGlobal::instance()->noAlbumArt():
                                   QPixmap::fromImage(detailInfo.coverImage));
        setEliedText(m_labels[ItemTitle], detailInfo.textLists[Name]);
        setEliedText(m_fileName, tr("In file: %1").arg(detailInfo.fileName));
        m_fileName->setFilePath(detailInfo.filePath);
        setEliedText(m_labels[ItemTime], detailInfo.textLists[Time]);
        setEliedText(m_labels[ItemArtist], detailInfo.textLists[Artist]);
    }
    //Set the position.
    moveToPosition(position);
}

inline void KNMusicDetailTooltip::moveToPosition(const QPoint &position)
{
    //Move right of the tooltip for a little to avoid the mouse pointer.
    int preferX=position.x()+14, preferY=position.y()-(m_tooltipHeight>>1),
        screenHeight=QApplication::desktop()->height(),
        screenWidth=QApplication::desktop()->width();
    //Move to the right position.
    move(preferX+m_tooltipWidth>screenWidth?
             screenWidth-m_tooltipWidth-5:preferX,
         preferY+m_tooltipHeight>screenHeight?
             screenHeight-m_tooltipHeight:preferY);
}

void KNMusicDetailTooltip::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    //Stop the disappear counter.
    m_disappearCounter->stop();
    //Magic Interval:
    /*
     * Reduce the disappear timer interval. This is called magic interval, when
     * user move there mouse in and then move mouse out.
     */
    m_disappearCounter->setInterval(300);
    //Stop the time line.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Launch mouse in/out timer.
    m_mouseIn->setStartFrame(m_backgroundColor.value());
    m_mouseIn->start();
    //Set focus.
    setFocus();
    //When mouse move in, it means user may want to preview the song, load the
    //music here.
    loadCurrentToPreview();
}

void KNMusicDetailTooltip::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    //Start disappear.
    startDisappearCountWithAnime();
}

void KNMusicDetailTooltip::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    //Start disappear.
    startDisappearCountWithAnime();
}

void KNMusicDetailTooltip::onActionHide()
{
    //Reset the player.
    resetPreviewPlayer();
    //Hide the hint.
    hide();
}

void KNMusicDetailTooltip::onActionMouseInOut(const int &frame)
{
    //Change the color and palette.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             frame);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    m_textColor.setHsv(m_textColor.hue(),
                       m_textColor.saturation(),
                       0x5f+(frame<<1));
    m_palette.setColor(QPalette::WindowText, m_textColor);
    //Set the palette.
    setPalette(m_palette);
    for(int i=0; i<ToolTipItemsCount; i++)
    {
        m_labels[i]->setPalette(m_palette);
    }
}

void KNMusicDetailTooltip::onActionPlayNPauseClick()
{
    //According to the button icon to do the action.
    if(m_isButtonPlay)
    {
        m_backend->playPreview();
    }
    else
    {
        m_backend->pausePreview();
    }
}

void KNMusicDetailTooltip::onActionPreviewStatusChange(const int &state)
{
    //Check the state.
    m_isButtonPlay=(state!=PlayingState);
    //Set the icon.
    m_playNPause->setIcon(m_isButtonPlay?m_playIcon:m_pauseIcon);
}

void KNMusicDetailTooltip::onActionProgressPressed()
{
    m_progressPressed=true;
}

void KNMusicDetailTooltip::onActionProgressReleased()
{
    m_progressPressed=false;
}

void KNMusicDetailTooltip::onActionPreviewPositionChanged(const qint64 &position)
{
    if(!m_progressPressed)
    {
        m_progress->setValue(position);
    }
}

inline void KNMusicDetailTooltip::loadCurrentToPreview()
{
    //Check is the current index still available.
    if(m_currentIndex.isValid())
    {
        //Load the file to preview thread.
        m_backend->loadPreview(m_currentMusicModel->rowProperty(m_currentIndex.row(),
                                                                FilePathRole).toString());
        //Check if it need to set a section.
        if(m_currentMusicModel->rowProperty(m_currentIndex.row(),
                                            StartPositionRole).toLongLong()!=-1)
        {
            m_backend->setPreviewSection(m_currentMusicModel->rowProperty(m_currentIndex.row(),
                                                                          StartPositionRole).toLongLong(),
                                         m_currentMusicModel->songDuration(m_currentIndex.row()));
        }
        //Update the progress bar.
        m_progress->setMaximum(m_backend->previewDuration());
    }
}

inline void KNMusicDetailTooltip::resetDisappearCounter()
{
    //Stop the counter and set the interval.
    m_disappearCounter->stop();
    m_disappearCounter->setInterval(1500);
}

inline void KNMusicDetailTooltip::resetPreviewPlayer()
{
    //Reset player button state.
    m_isButtonPlay=true;
    m_playNPause->setIcon(m_playIcon);
    //Reset the backend.
    m_backend->resetPreviewPlayer();
    //Reset the progress bar.
    m_progress->setValue(0);
}

inline void KNMusicDetailTooltip::initialTimeLine(QTimeLine *timeline)
{
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(5);
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNMusicDetailTooltip::onActionMouseInOut);
}

inline void KNMusicDetailTooltip::setEliedText(QLabel *label, const QString &text)
{
    //Calculate the size of the text using the fontMetrics of the label.
    if(label->fontMetrics().width(text)>m_labelWidth)
    {
        //Set the elied text to the label, add the text as tooltip of it.
        label->setText(label->fontMetrics().elidedText(text,
                                                       Qt::ElideRight,
                                                       m_labelWidth));
        label->setToolTip(text);
    }
    else
    {
        //Clear the tooltip and set text.
        label->setText(text);
        label->setToolTip("");
    }
}

void KNMusicDetailTooltip::startDisappearCountWithAnime()
{
    //Stop the time line.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Launch mouse in/out timer.
    m_mouseOut->setStartFrame(m_backgroundColor.value());
    m_mouseOut->start();
    //Start disappear count.
    m_disappearCounter->start();
}
