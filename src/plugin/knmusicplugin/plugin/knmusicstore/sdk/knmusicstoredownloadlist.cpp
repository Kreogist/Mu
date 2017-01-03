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
#include <QBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardPaths>

#include "knopacityanimebutton.h"
#include "knopacityanimetextbutton.h"
#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusicstoredownloadlistdelegate.h"
#include "knmusicstoreutil.h"
#include "knmusicstoredownloadmanager.h"

#include "knmusicstoredownloadlist.h"

#include <QDebug>

KNMusicStoreDownloadList::KNMusicStoreDownloadList(QWidget *parent) :
    QWidget(parent),
    m_backgroundColor(QColor(0, 0, 0, 0)),
    m_downloadPath(QStandardPaths::writableLocation(
                       QStandardPaths::MusicLocation)),
    m_downloadModel(new KNMusicStoreDownloadManager(this)),
    m_stateButton(new KNOpacityAnimeButton(this)),
    m_closeList(new KNOpacityAnimeButton(this)),
    m_missionStart(generateButton(":/plugin/music/store/download_start.png")),
    m_missionPause(generateButton(":/plugin/music/store/download_pause.png")),
    m_missionRemove(generateButton(":/plugin/music/store/download_close.png")),
    m_missionClear(generateButton(":/plugin/music/store/download_remove.png")),
    m_container(new QWidget(this)),
    m_containerAnime(new QPropertyAnimation(m_container, "pos", this)),
    m_downloadView(new QTreeView(this))
{
    //Configure the state button.
    m_stateButton->setFixedSize(16, 16);
    m_stateButton->setIcon(QIcon(":/plugin/music/store/download_list.png"));
    connect(m_stateButton, &KNOpacityAnimeButton::clicked,
            this, &KNMusicStoreDownloadList::showDownloadList);
    //Configure the container.
    m_container->setAutoFillBackground(true);
    m_container->setObjectName("MusicStoreDownloadList");
    knTheme->registerWidget(m_container);
    //Configure the view
    m_downloadView->setFrameStyle(QFrame::NoFrame);
    m_downloadView->setItemDelegateForColumn(
                1,
                new KNMusicStoreDownloadListDelegate(this));
    m_downloadView->setIndentation(0);
    m_downloadView->setAllColumnsShowFocus(true);
    m_downloadView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_downloadView->setObjectName("MusicStoreDownloadView");
    knTheme->registerWidget(m_downloadView);
    connect(m_downloadView, &QTreeView::doubleClicked,
            this, &KNMusicStoreDownloadList::onDoubleClickIndex);
    //Configure the model.
    m_downloadView->setModel(m_downloadModel);
    m_downloadView->setColumnWidth(0, 50);
    m_downloadView->setColumnWidth(
                1,
                KNMusicStoreDownloadListDelegate::columnWidth());
    connect(m_downloadView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &KNMusicStoreDownloadList::onSelectionChanged);
    // Configure the header and column.
    QHeaderView *headerView=m_downloadView->header();
    headerView->hide();
    headerView->setSectionResizeMode(0, QHeaderView::Fixed);
    headerView->setSectionResizeMode(1, QHeaderView::Fixed);
    headerView->setSectionResizeMode(2, QHeaderView::Stretch);
    //Configure close list button.
    m_closeList->setIcon(QIcon(":/plugin/music/player/hide_mainplayer.png"));
    m_closeList->setFixedSize(25, 25);
    connect(m_closeList, &KNOpacityAnimeButton::clicked,
            this, &KNMusicStoreDownloadList::hideDownloadList);
    //Configure the model signal.
    connect(m_downloadModel,
            &KNMusicStoreDownloadManager::modelEmptyStateChange,
            this, &KNMusicStoreDownloadList::onEmptyStateChanged);
    //Configure the operation button.
    connect(m_missionStart, &KNOpacityAnimeButton::clicked,
            [=]
            {
                //Download all the mission.
                m_downloadModel->startAll();
            });
    connect(m_missionPause, &KNOpacityAnimeButton::clicked,
            [=]
            {
                //Emit the signal for pause mission.
                m_downloadModel->pauseAll();
            });
    connect(m_missionRemove, &KNOpacityAnimeTextButton::clicked,
            [=]
            {
                //Create the mission list.
                QList<int> selectedMission;
                //Get the current selected index.
                QModelIndexList selectedList=
                        m_downloadView->selectionModel()->selectedRows();
                //Translate the list to mission list.
                for(auto i : selectedList)
                {
                    //Add i to the list.
                    selectedMission.append(i.row());
                }
                //Sort index row list.
                std::sort(selectedMission.begin(), selectedMission.end());
                //Remove the selected index.
                m_downloadModel->removeMissions(selectedMission);
            });

    //Initial the container layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, m_container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    m_container->setLayout(mainLayout);
    //Initial the button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    buttonLayout->setContentsMargins(15, 11, 15, 11);
    buttonLayout->setSpacing(10);
    mainLayout->addLayout(buttonLayout);
    //Add buttons to layout.
    buttonLayout->addWidget(m_missionStart);
    buttonLayout->addWidget(m_missionPause);
    buttonLayout->addWidget(m_missionRemove);
    buttonLayout->addWidget(m_missionClear);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeList);
    //Add download view.
    mainLayout->addWidget(m_downloadView, 1);

    //Configure the animation.
    m_containerAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_containerAnime, &QPropertyAnimation::valueChanged,
            this, &KNMusicStoreDownloadList::onContainerMove);
    connect(m_containerAnime, &QPropertyAnimation::finished,
            this, &KNMusicStoreDownloadList::onContainerMoveFinish);

    //Link locale.
    knI18n->link(this, &KNMusicStoreDownloadList::retranslate);
    retranslate();
}

QWidget *KNMusicStoreDownloadList::stateButton()
{
    return m_stateButton;
}

void KNMusicStoreDownloadList::showDownloadList()
{
    //Stop the anime.
    m_containerAnime->stop();
    //Start the animation.
    m_containerAnime->setStartValue(QPoint(m_container->x(), -height()));
    m_containerAnime->setEndValue(QPoint(m_container->x(), 0));
    //Start the anime.
    m_containerAnime->start();
    //Show the widget first.
    show();
}

void KNMusicStoreDownloadList::downloadSong(const QString &url,
                                            const QString &fileName,
                                            const QString &songTitle)
{
    //Start the download mission.
    m_downloadModel->appendItem(url,
                                m_downloadPath,
                                fileName,
                                songTitle);
}

void KNMusicStoreDownloadList::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Paint the background.
    painter.fillRect(rect(), m_backgroundColor);
}

void KNMusicStoreDownloadList::resizeEvent(QResizeEvent *event)
{
    //Do original resize.
    QWidget::resizeEvent(event);
    //Calcualte container width.
    int contentWidth=width();
    contentWidth=(contentWidth>KNMusicStoreUtil::maximumContentWidth())?
                        KNMusicStoreUtil::maximumContentWidth():
                        contentWidth;
    int contentX=(width()-contentWidth)>>1;
    //Resize the widget.
    m_container->resize(contentWidth, height());
    //Check animation running state.
    if(QPropertyAnimation::Running==m_containerAnime->state())
    {
        //Calculate the hide pos.
        QPoint hidePos(contentX, -height()), showPos(contentX, 0);
        //Check the running position.
        if(0==m_containerAnime->startValue().toPoint().y())
        {
            //From center to top, we need to change the end position.
            m_containerAnime->setEndValue(hidePos);
            m_containerAnime->setStartValue(showPos);
        }
        else
        {
            //From top to center, we need to change the start position.
            m_containerAnime->setStartValue(hidePos);
            m_containerAnime->setEndValue(showPos);
        }
    }
    else
    {
        //Update the position of the container.
        m_container->move((width()-contentWidth)>>1,
                          (m_container->y()==0)?0:(-height()));
    }
}

void KNMusicStoreDownloadList::retranslate()
{
    //Update the button tooltips.
    m_missionStart->setText(tr("Start All"));
    m_missionPause->setText(tr("Pause All"));
    m_missionRemove->setText(tr("Remove"));
    m_missionClear->setText(tr("Clear"));
}

void KNMusicStoreDownloadList::hideDownloadList()
{
    //Stop the anime.
    m_containerAnime->stop();
    //Start the animation.
    m_containerAnime->setStartValue(QPoint(m_container->x(), 0));
    m_containerAnime->setEndValue(QPoint(m_container->x(), -height()));
    //Start the anime.
    m_containerAnime->start();
}

void KNMusicStoreDownloadList::onDoubleClickIndex(const QModelIndex &index)
{
    //Check the index state.
    if(m_downloadModel->isPaused(index.row()))
    {
        //Start the mission.
        m_downloadModel->startMission(index.row());
        return;
    }
    //Paused the downloading mission.
    m_downloadModel->pauseMission(index.row());
}

void KNMusicStoreDownloadList::onContainerMove(const QVariant &value)
{
    //Update the background color, calculate the alpha.
    m_backgroundColor.setAlphaF((1.0-
                                (qreal)(-value.toPoint().y())/(qreal)height())
                                /2.0);
    //Update the widget.
    update();
}

void KNMusicStoreDownloadList::onContainerMoveFinish()
{
    //Check the animation end position.
    //Check the running position.
    if(0==m_containerAnime->startValue().toPoint().y())
    {
        //From center to top, we need to hide the widget.
        hide();
    }
}

void KNMusicStoreDownloadList::onBackgroundClicked()
{
    //Check the animation running state.
    if(QPropertyAnimation::Running==m_containerAnime->state())
    {
        //Ignore the running request.
        return;
    }
    //Or else hide the widget.
    hideDownloadList();
}

void KNMusicStoreDownloadList::onEmptyStateChanged(bool isEmpty)
{
    //Change the data to the inverse one, because that is the real button state.
    isEmpty=!isEmpty;
    //When the model is empty, no button should be available.
    m_missionStart->setEnabled(isEmpty);
    m_missionPause->setEnabled(isEmpty);
    m_missionClear->setEnabled(isEmpty);
}

void KNMusicStoreDownloadList::onSelectionChanged(
        const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)
    //Check the current selection list.
    m_missionRemove->setEnabled(!m_downloadView->selectionModel(
                                    )->selection().isEmpty());
}

inline KNOpacityAnimeTextButton *KNMusicStoreDownloadList::generateButton(
        const QString &iconPath)
{
    //Construct the item.
    KNOpacityAnimeTextButton *button=new KNOpacityAnimeTextButton(this);
    //Configure the button.
    button->setObjectName("MusicStoreDownloadList");
    button->setEnabled(false);
    button->setIcon(QIcon(iconPath));
    knTheme->registerWidget(button);
    //Give back the button.
    return button;
}
