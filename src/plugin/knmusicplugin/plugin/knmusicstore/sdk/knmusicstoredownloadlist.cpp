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
#include <QLabel>
#include <QBoxLayout>
#include <QPropertyAnimation>

#include "knopacityanimebutton.h"
#include "knlocalemanager.h"
#include "knthememanager.h"
#include "knmouseunclickablewidget.h"

#include "knmusicstoreglobal.h"

#include "knmusicstoredownloadlist.h"

#include <QDebug>

KNMusicStoreDownloadList::KNMusicStoreDownloadList(QWidget *parent) :
    QWidget(parent),
    m_button(new KNOpacityAnimeButton(this)),
    m_container(new KNMouseUnclickableWidget(this)),
    m_title(new QLabel(this)),
    m_showHideAnime(new QPropertyAnimation(m_container, "pos", this))
{
    //Set properties.
    setAutoFillBackground(true);
    //Configure the button.
    m_button->setIcon(QPixmap(":/plugin/music/store/download_list.png"));
    m_button->setCursor(Qt::PointingHandCursor);
    m_button->setFixedSize(13, 13);
    connect(m_button, &KNOpacityAnimeButton::clicked,
            this, &KNMusicStoreDownloadList::onActionShowHideDownloadList);
    //Configure the container.
    m_container->setObjectName("MusicStoreDownloadList");
    m_container->setAutoFillBackground(true);
    m_container->setMaximumWidth((knMusicStoreGlobal->storeContentWidth()>>2)
                                 *3);
    knTheme->registerWidget(m_container);
    //Congifure the animation.
    m_showHideAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_showHideAnime, &QPropertyAnimation::valueChanged,
            this, &KNMusicStoreDownloadList::onActionChangeOpacity);
    //Configure the title label button.
    QFont labelFont=m_title->font();
    labelFont.setBold(true);
    labelFont.setPixelSize(17);
    m_title->setFont(labelFont);
    m_title->setAlignment(Qt::AlignVCenter);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    m_container->setLayout(mainLayout);
    //Initial title layout.
    QBoxLayout *titleLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           mainLayout->widget());
    titleLayout->setContentsMargins(31,17,31,17);
    titleLayout->addWidget(m_title);
    titleLayout->addStretch();
    //Add to main layout.
    mainLayout->addLayout(titleLayout);
    mainLayout->addStretch();

    //Link the local manager.
    knI18n->link(this, &KNMusicStoreDownloadList::retranslate);
    retranslate();
}

KNOpacityAnimeButton *KNMusicStoreDownloadList::iconButton()
{
    return m_button;
}

void KNMusicStoreDownloadList::resizeEvent(QResizeEvent *event)
{
    //Update the widget size.
    QWidget::resizeEvent(event);
    //Update the container.
    m_container->resize(knMusicStoreGlobal->storeContentWidth(),
                        height());
    //Check whether the animation is launching.
    if(m_showHideAnime->state()==QPropertyAnimation::Running)
    {
        //Update the parameter.
        updaterAnimeParameter(m_showHideAnime->startValue().toPoint().y()<0);
    }
    else
    {
        //Move the container.
        m_container->move((width()-m_container->width())>>1, 0);
    }
}

void KNMusicStoreDownloadList::mousePressEvent(QMouseEvent *event)
{
    //Do the origianl things.
    QWidget::mousePressEvent(event);
    //If it's clicked, hide the current widget.
    onActionShowHideDownloadList();
}

void KNMusicStoreDownloadList::retranslate()
{
    m_title->setText(tr("Download"));
}

void KNMusicStoreDownloadList::onActionShowHideDownloadList()
{
    //Check animation state.
    if(m_showHideAnime->state()==QAbstractAnimation::Running)
    {
        //Ignore the request.
        return;
    }
    //Check the visiblity of the list.
    if(isVisible())
    {
        //Hide the widget.
        connect(m_showHideAnime, &QPropertyAnimation::finished,
                [=]
                {
                    //Reset animation.
                    disconnect(m_showHideAnime, &QPropertyAnimation::finished,
                               0, 0);
                    //Hide the widget.
                    hide();
                });
        //Launch hide the content widget anime.
        updaterAnimeParameter(false);
        m_showHideAnime->start();
        //Finished.
        return;
    }
    //Show the widget.
    show();
    //Launch show the content widget anime.
    updaterAnimeParameter(true);
    m_showHideAnime->start();
}

void KNMusicStoreDownloadList::onActionChangeOpacity(const QVariant &position)
{
    //Calculate the position.
    QPalette pal=palette();
    //Update the window color.
    pal.setColor(QPalette::Window,
                 QColor(0, 0, 0,
                        (1.0 - (qreal)qAbs(position.toPoint().y()) /
                               (qreal)height())
                        * 200));
    //Set the palette back.
    setPalette(pal);
}

inline void KNMusicStoreDownloadList::updaterAnimeParameter(bool isShown)
{
    //Initial the top point and final point.
    int xPos=(width()-m_container->width())>>1;
    QPoint upPos(xPos, -height()), downPos(xPos, 0);
    //Check the shown.
    if(isShown)
    {
        //Set the start and end for hide.
        m_showHideAnime->setStartValue(upPos);
        m_showHideAnime->setEndValue(downPos);
    }
    else
    {
        //Set the start and end for show.
        m_showHideAnime->setStartValue(downPos);
        m_showHideAnime->setEndValue(upPos);
    }
}
