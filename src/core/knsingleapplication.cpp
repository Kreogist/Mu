/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLocalSocket>
#include <QLocalServer>
#include <QFile>

#include "knsingleapplication.h"

KNSingleApplication::KNSingleApplication(int &argc,
                                         char *argv[],
                                         const QString uniqueKey) :
    QApplication(argc, argv),
    m_uniqueKey(uniqueKey)
{
    //Set the key of the shared memory space.
    m_uniqueKeyMemory.setKey(m_uniqueKey);
    //Check the memory and we can know whether there's another instance.
    if(m_uniqueKeyMemory.attach())
    {
        //Find an another instance, set the running flag to true.
        m_isRunning=true;
    }
    else
    {
        //This is the first instance.
        m_isRunning=false;
        //Create a shared memory.
        if(!m_uniqueKeyMemory.create(1))
        {
            //!FIXME: show up 'Unable to create single instance.'
            return;
        }
        //Create a local server for listening incomming messages from other instances.
        m_messageServer=new QLocalServer(this);
        //When there's a new connection connected, we need to process the message.
        connect(m_messageServer, &QLocalServer::newConnection,
                this, &KNSingleApplication::messageReceive);
        //Using the unique key as socket ID.
        if(!m_messageServer->listen(m_uniqueKey))
        {
            //If not successful, and it caused by address has been taken,
            //that must be an instance crashed and left those data there.
            //Remove it and listen again.
            if(m_messageServer->serverError()==QAbstractSocket::AddressInUseError)
            {
                QLocalServer::removeServer(m_uniqueKey);
                m_messageServer->listen(m_uniqueKey);
            }
        }
    }
}

bool KNSingleApplication::isPatternRunning() const
{
    return m_isRunning;
}

bool KNSingleApplication::sendMessage(const QString &message)
{
    //If I am the only instance, ignore the send request.
    if(!m_isRunning)
    {
        return false;
    }
    //Create sender.
    QLocalSocket poster;
    //Connect to the server.
    poster.connectToServer(m_uniqueKey, QIODevice::WriteOnly);
    //If cannot connect, throw out error.
    if(!poster.waitForConnected(m_timeout))
    {
        //!FIXME: show the error string.
        return false;
    }
    //Write data.
    poster.write(message.toUtf8()); //I love UTF-8, bite me!
    //Check write successful.
    if(!poster.waitForBytesWritten(m_timeout))
    {
        //!FIXME: show the cannot write error.
        poster.disconnectFromServer();
        return false;
    }
    //Disconnect and return true.
    poster.disconnectFromServer();
    return true;
}

void KNSingleApplication::messageReceive()
{
    //Get the socket.
    QLocalSocket *client=m_messageServer->nextPendingConnection();
    //Wait for reading.
    if(!client->waitForReadyRead(m_timeout))
    {
        //!FIXME: show the error string.
        return;
    }
    //Emit the message.
    QString clientMessage=QString::fromUtf8(client->readAll().constData());
    emit messageAvailable(clientMessage);
    //Disconnect socket.
    client->disconnectFromServer();
}
