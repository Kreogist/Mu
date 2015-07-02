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
#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDataStream>

#include "knsingletonapplication.h"

KNSingletonApplication::KNSingletonApplication(int &argc,
                                               char **argv,
                                               const QString uniqueKey) :
    QApplication(argc, argv),
    m_messageServer(nullptr),
    m_uniqueKeyMemeory(new QSharedMemory(uniqueKey, this)),
    m_isInstanceRunning(false)
{
    //We used the QSharedMemory for singleton detecting. Give the QSharedMemory
    //the unique key, check the shared memory and we will know whether this is
    //first instance.
    if(m_uniqueKeyMemeory->attach())
    {
        //We detect another instance running.
        m_isInstanceRunning=false;
        //Get the arguments.
        QStringList pendingMessages=arguments();
        //According to the document in QCoreApplication::arguments():
        //   Usually arguments().at(0) is the program name, arguments().at(1) is
        //   the first argument, and arguments().last() is the last argument.
        //We will only send the arguments and ignore the program name.
        //If the message list has no messages or even it's empty, we won't send
        //any thing.
        if(pendingMessages.size()>1)
        {
            //Remove the program name(application path).
            pendingMessages.removeFirst();
            //Send the messages.
            sendMessages(uniqueKey, pendingMessages);
        }
        return;
    }
    //Set running instance
    m_isInstanceRunning=true;
    //Create a small part of shared memory for instance flag.
    if(!m_uniqueKeyMemeory->create(1))
    {
        //Unable to create single instance.
        qDebug("Unable to create the single instance.");
        return;
    }
    //Create a local server for listening incoming message.
    m_messageServer=new QLocalServer(this);
    //When there's another instance which is being created, it will send the
    //arguments to message server from the other instance.
    connect(m_messageServer, &QLocalServer::newConnection,
            this, &KNSingletonApplication::onMessageReceive);
    //The unique key will be used as the connections name as well.
    if(!m_messageServer->listen(uniqueKey))
    {
        //If we are failed to start listening, and it was caused by address has
        //been taken, it must be a previous instance crashed and left those data
        //there. Remove it and listen again.
        if(QAbstractSocket::AddressInUseError==m_messageServer->serverError())
        {
            //Remove the server and start listen again.
            QLocalServer::removeServer(uniqueKey);
            //Start listen once more.
            m_messageServer->listen(uniqueKey);
        }
    }
}

bool KNSingletonApplication::isInstanceRunning() const
{
    return m_isInstanceRunning;
}

void KNSingletonApplication::onMessageReceive()
{
    const int timeoutLimit=1000;
    //Get the socket from the sender.
    QLocalSocket *client=m_messageServer->nextPendingConnection();
    //Waiting for reading client data.
    if(!client->waitForReadyRead(timeoutLimit))
    {
        qDebug("Cannot read the client data.");
        return;
    }
    //Get the data, and parse it as string list.
    QByteArray messageData=client->readAll();
    //Disconnect the socket.
    client->disconnectFromServer();
    //Use a data stream to flush data to string list.
    QStringList messages;
    QDataStream dataReader(&messageData, QIODevice::ReadOnly);
    dataReader >> messages;
    //Emit the messages.
    emit messageAvailable(messages);
}

bool KNSingletonApplication::sendMessages(const QString &uniqueKey,
                                          const QStringList &messages)
{
    const int timeoutLimit=1000;
    //Create sender client.
    QLocalSocket client;
    //Link to the server which is listening to the unique key.
    client.connectToServer(uniqueKey, QIODevice::WriteOnly);
    //If connecting failed, return false.
    if(!client.waitForConnected(timeoutLimit))
    {
        qDebug("Cannot connect to the local server.");
        //Disconnect from the server.
        client.disconnectFromServer();
        return false;
    }
    //Generate the message data.
    QByteArray messageData;
    QDataStream dataWriter(&messageData, QIODevice::WriteOnly);
    dataWriter << messages;
    //Send the data to local server.
    client.write(messageData);
    //Check sending status.
    if(!client.waitForBytesWritten(timeoutLimit))
    {
        qDebug("Send arguments failed.");
        client.disconnectFromServer();
        return false;
    }
    //Send the arguments success.
    client.disconnectFromServer();
    return true;
}

