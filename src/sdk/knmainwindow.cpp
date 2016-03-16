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
#include <QAction>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QTimer>
#include <QSequentialAnimationGroup>

//Dependences
#include "knabstractmusicplugin.h"
#include "knglobal.h"
#include "knconfigure.h"
#include "kncategoryplugin.h"
#include "knpreferenceplugin.h"
#include "knnotification.h"
#include "knthememanager.h"
#include "knmainwindowcontainer.h"
#include "knnotification.h"
#include "knaccountavatarbutton.h"

//Notifications
#include "notification/knnotificationcenter.h"
#include "notification/knnotificationwidget.h"

//Ports
#include "knmainwindowheaderbase.h"

#include "knmainwindow.h"

#include <QDebug>

#define NotificationPatch 22
#define NotificationWidgetPatch 10

KNMainWindow::KNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_musicPlugin(nullptr),
    m_cacheConfigure(knGlobal->cacheConfigure()->getConfigure("MainWindow")),
    m_container(new KNMainWindowContainer(this)),
    m_categoryPlugin(nullptr),
    m_notificationCenter(new KNNotificationCenter(this)),
    m_inAnime(generateAnime()),
    m_outAnime(generateAnime()),
    m_outAndInAnime(new QSequentialAnimationGroup(this)),
    m_notificationWaiter(new QTimer(this)),
    m_originalWindowState(Qt::WindowNoState)
{
    setObjectName("MainWindow");
    //Set properties.
    setAutoFillBackground(true);
    setCentralWidget(m_container);
    setContentsMargins(0,0,0,0);
    setMinimumSize(772, 477);
    setWindowIcon(QIcon("://icon/mu.png"));
    //Mac OS X title hack.
#ifdef Q_OS_MACX
    setWindowTitle(qApp->applicationDisplayName());
#endif

    //Reset the parent relationship of indicator.
    m_notificationCenter->indicator()->setParent(this);
    //Hide the notification center.
    m_notificationCenter->hide();
    //Link the notification center button to show slots.
    connect(m_notificationCenter->headerButton(),
            &KNAccountAvatarButton::requireShowNotificationCenter,
            this, &KNMainWindow::onActionShowNotificationCenter);
    //Configure the notification widget.
    KNNotificationWidget *notificationWidget=
            m_notificationCenter->notificationWidget();
    //Reset the parent relationship of the notification widget.
    notificationWidget->setParent(this);
    notificationWidget->raise();
    //Move the notification center.
    notificationWidget->move(0,
                             -notificationWidget->height() -
                             NotificationWidgetPatch);

    //Configure the in and out animation.
    connect(m_outAnime, &QPropertyAnimation::finished,
            this, &KNMainWindow::onActionHideComplete);
    //Configure the animation queue.
    m_outAndInAnime->addAnimation(m_outAnime);
    m_outAndInAnime->addAnimation(m_inAnime);
    //Configure the waiter.
    m_notificationWaiter->setInterval(4000);
    m_notificationWaiter->setSingleShot(true);
    //Link the start slot with in anime.
    connect(m_inAnime, &QPropertyAnimation::finished,
            m_notificationWaiter,
            static_cast<void (QTimer::*)()>(&QTimer::start));
    connect(m_notificationWaiter, &QTimer::timeout,
            [=]
            {
                //Reset the notification widget.
                m_notificationCenter->notificationWidget()->resetPressedFlag();
                //Start the out anime.
                m_outAnime->start();
            });
    connect(notificationWidget, &KNNotificationWidget::requireHideNotification,
            [=]
            {
                //Stop the timer.
                m_notificationWaiter->stop();
                //Reset the notification widget.
                m_notificationCenter->notificationWidget()->resetPressedFlag();
                //Start the out anime.
                m_outAnime->start();
            });

    //Give the notification widget to backend.
    knNotification->setNotificationWidget(notificationWidget);
    //Link the notification backend.
    connect(knNotification, &KNNotification::requirePushNotification,
            this, &KNMainWindow::onActionPopupNotification);
    connect(this, &KNMainWindow::notificationComplete,
            knNotification, &KNNotification::onActionPushNextNotification);

    //Add main window to theme list.
    knTheme->registerWidget(this);
    //Add full screen short cut actions.
    QAction *fullScreen=new QAction(this);
    fullScreen->setShortcut(QKeySequence(QKeySequence::FullScreen));
    connect(fullScreen, &QAction::triggered,
            this, &KNMainWindow::onActionFullScreen);
    addAction(fullScreen);
    //Recover the geometry.
    recoverGeometry();
    //Update the animation positions.
    updateAnimeStartAndEnd();
}

void KNMainWindow::setHeader(KNMainWindowHeaderBase *header)
{
    //Check header pointer.
    if(header==nullptr)
    {
        //If the header pointer is null, then failed to load the header.
        return;
    }
    //Set the header widget.
    m_container->setHeader(header);
    //Add notification center button to header.
    header->addNotificationButton(m_notificationCenter->headerButton());
    //Link the header show preference signal to container.
    connect(header, &KNMainWindowHeaderBase::requireShowPreference,
            m_container, &KNMainWindowContainer::showPreference);
}

void KNMainWindow::setMainWidget(KNCategoryPlugin *mainWidget)
{
    //Save the main widget.
    m_categoryPlugin=mainWidget;
    //Set the new category plugin.
    m_container->setMainWidget(m_categoryPlugin);
}

void KNMainWindow::setPreferencePanel(KNPreferencePlugin *preferencePanel)
{
    //Set the preference widget.
    m_container->setPreferencePanel(preferencePanel);
    //Link the hide preference request.
    connect(preferencePanel, &KNPreferencePlugin::requireClosePreference,
            m_container, &KNMainWindowContainer::hidePreference);
}

void KNMainWindow::setMusicPlugin(KNAbstractMusicPlugin *musicPlugin)
{
    //Save the music player plugin.
    m_musicPlugin=musicPlugin;
    //Set the main player widget.
    m_container->setMainPlayer(m_musicPlugin->mainPlayer());
    //Link the plugin and the main window.
    connect(m_musicPlugin, &KNAbstractMusicPlugin::requireShowMainPlayer,
            this, &KNMainWindow::showMainPlayer);
    connect(m_musicPlugin, &KNAbstractMusicPlugin::requireHideMainPlayer,
            this, &KNMainWindow::hideMainPlayer);
    connect(m_musicPlugin, &KNAbstractMusicPlugin::requireHideMainWindow,
            this, &KNMainWindow::hide);
    connect(m_musicPlugin, &KNAbstractMusicPlugin::requireShowMainWindow,
            this, &KNMainWindow::show);
    connect(m_musicPlugin, &KNAbstractMusicPlugin::requireCloseMainWindow,
            this, &KNMainWindow::close);

    connect(this, &KNMainWindow::mainWindowShown,
            m_musicPlugin, &KNAbstractMusicPlugin::hideMiniPlayer);
}

void KNMainWindow::showMainPlayer()
{
    //Set the container to show the main player.
    m_container->showMainPlayer();
}

void KNMainWindow::hideMainPlayer()
{
    //Hide the main player.
    m_container->hideMainPlayer();
}

void KNMainWindow::showEvent(QShowEvent *event)
{
    //Emit the main window show signal.
    emit mainWindowShown();
    //Show the main window.
    QMainWindow::showEvent(event);
}

void KNMainWindow::resizeEvent(QResizeEvent *event)
{
    //Resize the main window.
    QMainWindow::resizeEvent(event);
    //Update the position.
    updateAnimeStartAndEnd();
    //Check animation and notification widget position.
    if(m_inAnime->state()==QAbstractAnimation::Running ||
            m_outAnime->state()==QAbstractAnimation::Running ||
            m_outAndInAnime->state()==QAbstractAnimation::Running)
    {
        //Mission complete.
        return;
    }
    //Get notification widget.
    KNNotificationWidget *notificationWidget=
            m_notificationCenter->notificationWidget();
    //Move the notification widget to target position.
    notificationWidget->move(width() - notificationWidget->width() -
                             NotificationWidgetPatch,
                             notificationWidget->y());
}

void KNMainWindow::closeEvent(QCloseEvent *event)
{
    //Check music plugin working state.
    if(m_musicPlugin &&
            m_musicPlugin->isWorking())
    {
        //Ignore the close request.
        event->ignore();
        //Push notification.
        knNotification->pushOnly(
                    tr("Cannot quit now"),
                    tr("Music library is still adding music to your music "
                       "library, please wait until it complete."));
        //Finished.
        return;
    }
    //Save the configure of the category plugin, if the category is valid.
    if(m_categoryPlugin)
    {
        //Save the configure.
        m_categoryPlugin->saveConfigure();
    }
    //Save the geometry.
    backupGeometry();
    //Do the mainwindow close event.
    QMainWindow::closeEvent(event);
}

void KNMainWindow::onActionFullScreen()
{
    //Check out the full screen state.
    if(isFullScreen())
    {
        //Check the original window state.
        if(m_originalWindowState==Qt::WindowFullScreen)
        {
            //Set it to be no state.
            m_originalWindowState=Qt::WindowNoState;
        }
        //Set the window to normal state.
        setWindowState(m_originalWindowState);
    }
    else
    {
        //Save the original window state.
        m_originalWindowState=windowState();
        //Full screen the window.
        setWindowState(Qt::WindowFullScreen);
    }
}

void KNMainWindow::onActionShowNotificationCenter()
{
    //Reset the geometry.
    //Get header button.
    QWidget *headerButton=m_notificationCenter->headerButton();
    //Resize the notification center.
    m_notificationCenter->setMaximumHeight(height()-150);
    //Move the notification center.
    m_notificationCenter->move(
                QPoint(geometry().x() + width() - m_notificationCenter->width(),
                       geometry().y() + headerButton->geometry().bottom() +
                       NotificationPatch));
    //Move tyhe notification indicator.
    m_notificationCenter->indicator()->move(
                headerButton->mapTo(this, QPoint(0,0)) +
                QPoint((headerButton->width()>>1) -
                       (m_notificationCenter->indicator()->width()>>1),
                       headerButton->height()));
    //Show notification center.
    m_notificationCenter->show();
}

void KNMainWindow::onActionHideComplete()
{
    //Move the notification widget to the top of the window.
    m_notificationCenter->notificationWidget()->move(
                width()-m_notificationCenter->notificationWidget()->width()
                - NotificationWidgetPatch,
                - NotificationWidgetPatch - m_notificationCenter->height());
    //Emit the notification finished signal to notice the notification backend
    //to push the next notification.
    emit notificationComplete();
}

void KNMainWindow::onActionPopupNotification()
{
    //Check whether the timer is running.
    if(m_notificationWaiter->isActive())
    {
        //Stop the waiting.
        m_notificationWaiter->stop();
        //Start the queue animation.
        m_outAndInAnime->start();
        //Complete.
        return;
    }
    //Check whether the animation is running.
    if(m_inAnime->state()==QAbstractAnimation::Running ||
            m_outAnime->state()==QAbstractAnimation::Running ||
            m_outAndInAnime->state()==QAbstractAnimation::Running)
    {
        //Ignore the wrong calling.
        return;
    }
    //Them we need to start the in anime.
    m_inAnime->start();
}

inline void KNMainWindow::recoverGeometry()
{
    //Check is the last record is complete. If there's no windowWidth property
    //in the configure, means we don't save the last geometry. Ignore the
    //recover request.
    if(m_cacheConfigure->data("windowWidth").isNull())
    {
        return;
    }

    //Recover the window state.
    int windowState=m_cacheConfigure->data("windowState").toInt();
    //For maximum and full screen, we only need to set the window state.
    switch (windowState)
    {
    case Qt::WindowMaximized:
        setWindowState(Qt::WindowMaximized);
        return;
    case Qt::WindowFullScreen:
        setWindowState(Qt::WindowFullScreen);
        return;
    default:
        setWindowState(Qt::WindowNoState);
    }
    //Read the resolution data of the last time closed.
    int lastScreenWidth=getCacheValue("desktopWidth"),
        lastScreenHeight=getCacheValue("desktopHeight"),
        currentScreenWidth=qApp->desktop()->width(),
        currentScreenHeight=qApp->desktop()->height(),
            lastX=getCacheValue("windowX"),
            lastY=getCacheValue("windowY"),
            lastWidth=getCacheValue("windowWidth"),
            lastHeight=getCacheValue("windowHeight");
    //Check is the resolution is the same as the last closed time.
    if(!(lastScreenWidth==currentScreenWidth &&
         lastScreenHeight==currentScreenHeight))
    {
        //The resolution has been changed, recalculate the size parameters.
        //Get the width ratio and the height ratio.
        qreal widthRatio=(qreal)currentScreenWidth/(qreal)lastScreenWidth,
              heightRatio=(qreal)currentScreenHeight/(qreal)lastScreenHeight;
        //Recalculate the last parameters.
        zoomParameter(lastX, widthRatio);
        zoomParameter(lastY, heightRatio);
        zoomParameter(lastWidth, widthRatio);
        zoomParameter(lastHeight, heightRatio);
    }
    //Check the parameter, ensure that one part of the window must be inside
    //screen.
    //If it's not inside the screen, make the top of the window 0.
    if(lastY<0 || lastY>currentScreenHeight)
    {
        lastY=0;
    }
    //Set the geometry.
    setGeometry(lastX, lastY, lastWidth, lastHeight);
}

inline void KNMainWindow::backupGeometry()
{
    //Set the window state.
    setCacheValue("windowState", static_cast<int>(windowState()));
    //Set the window position.
    setCacheValue("windowX", geometry().x());
    setCacheValue("windowY", geometry().y());
    setCacheValue("windowWidth", geometry().width());
    setCacheValue("windowHeight", geometry().height());
    //Set the current desktop size.
    setCacheValue("desktopWidth", qApp->desktop()->width());
    setCacheValue("desktopHeight", qApp->desktop()->height());
}

inline QPropertyAnimation *KNMainWindow::generateAnime()
{
    //Generate the anime.
    QPropertyAnimation *anime=new QPropertyAnimation(
                m_notificationCenter->notificationWidget(),
                "pos",
                this);
    //Configure the animation.
    anime->setEasingCurve(QEasingCurve::OutCubic);
    //Give back anime pointer.
    return anime;
}

inline void KNMainWindow::updateAnimeStartAndEnd()
{
    //Calculate the target position.
    QPoint targetPosition(
                width() -
                m_notificationCenter->notificationWidget()->width() -
                NotificationWidgetPatch,
                NotificationWidgetPatch);
    //Reset the end start position.
    m_inAnime->setStartValue(
                QPoint(
                    targetPosition.x(),
                    -m_notificationCenter->notificationWidget()->height() -
                    NotificationWidgetPatch));
    m_inAnime->setEndValue(targetPosition);
    m_outAnime->setStartValue(targetPosition);
    m_outAnime->setEndValue(QPoint(width()+NotificationWidgetPatch,
                                   NotificationWidgetPatch));
}

inline int KNMainWindow::getCacheValue(const QString &valueName)
{
    return m_cacheConfigure->data(valueName).toInt();
}

inline void KNMainWindow::setCacheValue(const QString &valueName,
                                        const int &value)
{
    m_cacheConfigure->setData(valueName, value);
}

inline void KNMainWindow::zoomParameter(int &parameter, const qreal &ratio)
{
    parameter=(qreal)parameter*ratio;
}
