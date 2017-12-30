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
#include <QFileOpenEvent>

#include "knlocalpeer.h"
#include "knsingletonapplication.h"

#define TimeoutLimit 1000

KNSingletonApplication::KNSingletonApplication(int &argc,
                                                  char **argv,
                                                  const QString uniqueKey) :
    QApplication(argc, argv),
    m_peer(new KNLocalPeer(this, uniqueKey))
{
    //Check peer running state.
    if(m_peer->isClient())
    {
        //Get the arguments.
        QStringList &&pendingMessages=arguments();
        //Check out the pending message. If there's valid file path, then it
        //will be send to the main instance.
        //Send the messages.
        if(pendingMessages.size()>1 && m_peer->sendMessage(pendingMessages,
                                                           5000))
        {
            //Complete the mission.
            return;
        }
    }
    //Link the peer to the message received signal.
    connect(m_peer, &KNLocalPeer::messageReceived,
            this, &KNSingletonApplication::messageAvailable);
}

bool KNSingletonApplication::isInstanceRunning() const
{
    //At the original version, there is no '!'. However, according the logic it
    //is not correct here.
    //When the peer works at client mode, this should return false.
    return !m_peer->isClient();
}

//All these events are only used under Mac OS X.
//Windows and Linux doesn't use these fucking events.
#ifdef Q_OS_MACX
bool KNSingletonApplication::event(QEvent *e)
{
    //Under Mac OS X, we have to handle open file event as a new argument.
    if(e->type()==QEvent::FileOpen)
    {
        //Cast event as file open event.
        QFileOpenEvent *fileOpenEvent=static_cast<QFileOpenEvent *>(e);
        //Prepare the message stringlist.
        QStringList parameters;
        //Insert an empty string for the first part, follow the file path.
        parameters << "" << fileOpenEvent->file();
        //Emit the file open information.
        emit messageAvailable(parameters);
        //Mission complete.
        return true;
    }
    //When application is quit.
    if(e->type()==QEvent::Close)
    {
        //Emit the application quit signal.
        emit applicationQuit();
    }
    //Do original event.
    return QApplication::event(e);
}
#endif
