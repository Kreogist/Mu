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
#include <QBitmap>
#include <QBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QEvent>

//Dependencies.
#include "kndpimanager.h"
#include "knlocalemanager.h"
#include "knimagelabel.h"
#include "knaccountavatarbutton.h"

//Kreogist Account
#include "account/knaccountpanel.h"
#include "account/knaccountdetails.h"
#include "account/knaccount.h"

#include "knnotificationview.h"
#include "knnotification.h"
#include "knnotificationmodel.h"
#include "knnotificationwidget.h"

#include "knnotificationcenter.h"

#include <QDebug>

KNNotificationCenter::KNNotificationCenter(QWidget *parent) :
    QFrame(parent),
    m_notificationIndicator(new QLabel(this)),
    #ifndef Q_OS_MACX
    m_resizeAnime(new QPropertyAnimation(this, "size", this)),
    #endif
    m_button(new KNAccountAvatarButton(this)),
    m_notificationView(new KNNotificationView(this)),
    m_notificationWidget(new KNNotificationWidget(this)),
    m_accountPanel(new KNAccountPanel(this)),
    m_heightHint(-1)
{
    setObjectName("NotificationCenter");
    //Set properties.
    setAutoFillBackground(true);
    setFixedWidth(knDpi->width(324));
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::Tool |
                   Qt::FramelessWindowHint);

    //Configure the indicator.
    m_notificationIndicator->setAutoFillBackground(true);
    m_notificationIndicator->setAttribute(Qt::WA_TransparentForMouseEvents,
                                          true);
    //Set palette.
    QPalette pal=m_notificationIndicator->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(0,0,0,0));
    pal.setColor(QPalette::AlternateBase, QColor(0,0,0,0));
    pal.setColor(QPalette::ToolTipBase, QColor(255,255,255,0));
    pal.setColor(QPalette::ToolTipText, QColor(255,255,255,0));
    pal.setColor(QPalette::Button, QColor(255,255,255,0));
    pal.setColor(QPalette::ButtonText, QColor(255,255,255,0));
    m_notificationIndicator->setPalette(pal);
    //Set the pixmap.
    QPixmap indicatorPixmap=
            QPixmap("://public/notification_indicator.png").scaled(
                knDpi->size(22, 20),
                Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_notificationIndicator->setPixmap(indicatorPixmap);
    //Configure the indicator.
    m_notificationIndicator->setFixedSize(indicatorPixmap.size());
    m_notificationIndicator->hide();
#ifndef Q_OS_MACX
    //Configure animation.
    m_resizeAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_resizeAnime, &QPropertyAnimation::finished,
            [=]{resize(m_resizeAnime->endValue().toSize());});
#endif
    //Configure the button.
    m_button->setCursor(Qt::PointingHandCursor);
    m_button->setButtonSize(knDpi->width(NotificationButtonSize));
    //Configure the view.
    m_notificationView->setModel(knNotification->model());
    //Get account details.
    KNAccountDetails *accountDetails=knAccount->accountDetails();
    //Link account avatar change signal with button updater.
    connect(accountDetails, &KNAccountDetails::accountAvatarUpdate,
            [=]
            {
                //Update the notification center button.
                m_button->setAccountAvatar(
                            knAccount->accountDetails()->accountAvatar());
            });
    //Link the account panel to notification.
    connect(m_accountPanel, &KNAccountPanel::requireResize,
            this, &KNNotificationCenter::resizeNotificationCenter);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(knDpi->margins(5, 5, 5, 5));
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_accountPanel);
    mainLayout->addWidget(m_notificationView, 1);

    //Link retranslate.
    knI18n->link(this, &KNNotificationCenter::retranslate);
    retranslate();
}

KNAccountAvatarButton *KNNotificationCenter::headerButton()
{
    return m_button;
}

QWidget *KNNotificationCenter::indicator()
{
    return m_notificationIndicator;
}

int KNNotificationCenter::heightHint()
{
    return m_notificationView->heightHint()+ knDpi->height(10) +
            m_accountPanel->height();
}

void KNNotificationCenter::showEvent(QShowEvent *event)
{
    //Resize the notification center.
    resize(width(), qMin(maximumHeight(), heightHint()));
    //Show the indicator.
    m_notificationIndicator->show();
    //Set focus on this widget.
    activateWindow();
    //Do the original show event.
    QFrame::showEvent(event);
}

void KNNotificationCenter::hideEvent(QHideEvent *event)
{
    //Hide the indicator.
    m_notificationIndicator->hide();
    //Do the original hide event.
    QFrame::hideEvent(event);
}

void KNNotificationCenter::resizeEvent(QResizeEvent *event)
{
    //Resize the frame.
    QFrame::resizeEvent(event);
    //Set the rendering.
    QBitmap objBitmap(size());
    QPainter painter(&objBitmap);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    painter.fillRect(objBitmap.rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(objBitmap.rect(),
                            knDpi->width(10), knDpi->height(10));
    setMask(objBitmap);
}

void KNNotificationCenter::retranslate()
{
    //Update the tooltip.
    m_button->setToolTip(tr("Show Notification Center and Kreogist Account"));
}

void KNNotificationCenter::resizeNotificationCenter()
{
#ifdef Q_OS_MACX
    //For Mac OS X, we will use no animation.
    resize(width(), qMin(heightHint(), maximumHeight()));
#else
    //Stop the anime.
    m_resizeAnime->stop();
    //Set the initial size and end size.
    m_resizeAnime->setStartValue(size());
    m_resizeAnime->setEndValue(QSize(width(), qMin(heightHint(),
                                                   maximumHeight())));
    //Start the anime.
    m_resizeAnime->start();
#endif
}

KNNotificationWidget *KNNotificationCenter::notificationWidget()
{
    return m_notificationWidget;
}

bool KNNotificationCenter::event(QEvent *e)
{
    //Check the type of e.
    switch(e->type())
    {
    case QEvent::WindowDeactivate:
        //We need to hide the window.
        hide();
        return true;
    default:
        //Do the default event.
        return QFrame::event(e);
    }

}
