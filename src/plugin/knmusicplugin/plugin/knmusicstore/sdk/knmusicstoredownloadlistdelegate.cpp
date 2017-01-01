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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>

#include "knlocalemanager.h"

#include "knmusicstoreutil.h"

#include "knmusicstoredownloadlistdelegate.h"

#include <QDebug>

using namespace MusicStoreUtil;

#define DelegateWidth   250
#define DelegateHeight  30
#define ProgressHeight  6
#define ProgressWidth   100
#define ProgressY       12
#define ProgressRadius  3
#define LeftSpacing     10
#define IconX           115
#define IconY           7
#define TextX           136

KNMusicStoreDownloadListDelegate::KNMusicStoreDownloadListDelegate(
        QObject *parent) :
    QStyledItemDelegate(parent),
    m_pauseIcon(QPixmap(":/plugin/music/store/download_state_pause.png")),
    m_downloadingIcon(QPixmap(":/plugin/music/store/download_state_down.png"))
{
    //Link translator.
    knI18n->link(this, &KNMusicStoreDownloadListDelegate::retranslate);
    retranslate();
}

int KNMusicStoreDownloadListDelegate::columnWidth()
{
    return DelegateWidth;
}

QSize KNMusicStoreDownloadListDelegate::sizeHint(
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    //The size is fixed.
    return QSize(DelegateWidth, DelegateHeight);
}

void KNMusicStoreDownloadListDelegate::paint(QPainter *painter,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    //Draw other content.
    QStyledItemDelegate::paint(painter, option, index);
    //Backup the painter.
    painter->save();
    //Configure the painter.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform, true);
    //Prepare the percentage.
    qreal percentage=0.0;
    //Get the total bytes from the model.
    quint64 totalBytes=index.data(TotalBytesRole).toLongLong();
    //Check the total bytes, ignore the divided by 0 bug.
    if(totalBytes!=0)
    {
        //Then we could calculate the percentage.
        percentage=(qreal)index.data(DownBytesRole).toLongLong() /
                    (qreal)totalBytes;
    }
    //Update the pen and brush.
    painter->setPen(Qt::NoPen);
    painter->setBrush(option.palette.color(QPalette::Button));
    //Draw the progress bar.
    painter->drawRoundedRect(QRect(option.rect.x()+LeftSpacing,
                                   option.rect.y()+ProgressY,
                                   ProgressWidth, ProgressHeight),
                             ProgressRadius, ProgressRadius);
    //Draw the progress content.
    painter->setBrush(option.palette.color(QPalette::HighlightedText));
    painter->drawRoundedRect(QRect(option.rect.x()+LeftSpacing,
                                   option.rect.y()+ProgressY,
                                   percentage*ProgressWidth,
                                   ProgressHeight),
                             ProgressRadius, ProgressRadius);
    //Get running state.
    int isDownloading=index.data(StateRole).toInt();
    //Set the pen.
    painter->setPen((option.state&QStyle::State_Selected)?
                        option.palette.color(QPalette::HighlightedText):
                        option.palette.color(QPalette::Text));
    //Check state.
    switch(isDownloading)
    {
    case MissionRunning:
    {
        //Draw the state button.
        painter->drawPixmap(option.rect.x()+IconX, option.rect.y()+IconY,
                            m_downloadingIcon);
        //Prepare the state text.
        QString stateText;
        stateText.sprintf("%.1lf MB/%.1lf MB",
                          index.data(DownMegabytesRole).toReal(),
                          index.data(TotalMegabytesRole).toReal());
        //Draw the percentage.
        painter->drawText(option.rect.x()+TextX, option.rect.y(),
                          option.rect.width()-TextX, option.rect.height(),
                          Qt::AlignVCenter | Qt::AlignLeft,
                          stateText);
        //Complete.
        break;
    }
    case MissionPaused:
    {
        //Draw the state button.
        painter->drawPixmap(option.rect.x()+IconX, option.rect.y()+IconY,
                            m_pauseIcon);
        //Draw the paused text.
        painter->drawText(option.rect.x()+TextX, option.rect.y(),
                          option.rect.width(), option.rect.height(),
                          Qt::AlignVCenter | Qt::AlignLeft,
                          m_pauseText);
        //Complete
        break;
    }
    case MissionWaiting:
    {
        //Draw the waiting text only.
        painter->drawText(option.rect.x()+TextX, option.rect.y(),
                          option.rect.width(), option.rect.height(),
                          Qt::AlignVCenter | Qt::AlignLeft,
                          m_waitingText);
        break;
    }
    default:
        break;
    }
    //Restore the painter.
    painter->restore();
}

void KNMusicStoreDownloadListDelegate::retranslate()
{
    //Update pause text.
    m_pauseText=tr("Paused");
    m_waitingText=tr("Waiting");
}
