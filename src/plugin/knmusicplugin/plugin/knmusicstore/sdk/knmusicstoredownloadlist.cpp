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
#include <QStandardPaths>

#include "knopacityanimebutton.h"
#include "knthememanager.h"

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
    m_downloadView->setModel(m_downloadModel);

    //Initial the container layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, m_container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    m_container->setLayout(mainLayout);
    //Initial the button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    mainLayout->addLayout(buttonLayout);
    //Add buttons to layout.
    mainLayout->addWidget(m_downloadView, 1);

    //Configure the animation.
    m_containerAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_containerAnime, &QPropertyAnimation::valueChanged,
            this, &KNMusicStoreDownloadList::onContainerMove);
    connect(m_containerAnime, &QPropertyAnimation::finished,
            this, &KNMusicStoreDownloadList::onContainerMoveFinish);
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
