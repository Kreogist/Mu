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
#include <QTime>
#include <QDataStream>

#if defined(Q_OS_WIN)
#include <QtCore/QLibrary>
#include <QtCore/qt_windows.h>
typedef BOOL(WINAPI*PProcessIdToSessionId)(DWORD,DWORD*);
static PProcessIdToSessionId pProcessIdToSessionId = 0;
#endif
#if defined(Q_OS_UNIX)
#include <time.h>
#endif

#include "knlockedfile.cpp"
#if defined(Q_OS_WIN)
#include "knlockedfile_win.cpp"
#else
#include "knlockedfile_unix.cpp"
#endif

#include "knlocalpeer.h"

const char* KNLocalPeer::ack = "ack";

KNLocalPeer::KNLocalPeer(QObject *parent, const QString &appId) :
    QObject(parent),
    m_id(appId),
    m_socketName(QString()),
    m_server(nullptr),
    m_lockFileCreated(false)
{
    //Create the prefix according to the application ID.
    QString prefix = m_id;
    prefix.remove(QRegExp("[^a-zA-Z]"));
    prefix.truncate(6);
    //Get the socket name.
    QByteArray idc = m_id.toUtf8();
    quint16 idNum = qChecksum(idc.constData(), idc.size());
    m_socketName = QLatin1String("knsingleapp-") + prefix + QLatin1Char('-') +
            QString::number(idNum, 16);
#if defined(Q_OS_WIN)
    if (!pProcessIdToSessionId)
    {
        QLibrary lib("kernel32");
        pProcessIdToSessionId =
                (PProcessIdToSessionId)lib.resolve("ProcessIdToSessionId");
    }
    if (pProcessIdToSessionId)
    {
        DWORD sessionId = 0;
        pProcessIdToSessionId(GetCurrentProcessId(), &sessionId);
        m_socketName += QLatin1Char('-') + QString::number(sessionId, 16);
    }
#else
    m_socketName += QLatin1Char('-') + QString::number(::getuid(), 16);
#endif
    //Create the server.
    m_server = new QLocalServer(this);
    QString lockName = QDir(QDir::tempPath()).absolutePath() + QLatin1Char('/')
            + m_socketName + QLatin1String("-lockfile");
    //Update the locked file name.
    m_lockFile.setFileName(lockName);
    //Update the file created state.
    m_lockFileCreated = !m_lockFile.exists();
    //Open the file as read write mode.
    m_lockFile.open(QIODevice::ReadWrite);
}

KNLocalPeer::~KNLocalPeer()
{
    //Check the locked file state.
    if (m_lockFileCreated)
    {
        //Remove the locked file.
        m_lockFile.remove();
    }
}

bool KNLocalPeer::isClient()
{
    //Check the file is locked or not.
    if (m_lockFile.isLocked())
    {
        //The file is locked, then it must be a server.
        return false;
    }
    //Try to lock the file at read only mode.
    if (!m_lockFile.lock(KNLockedFile::WriteLock, false))
    {
        //Ok, we are now in a client mode.
        return true;
    }
    //Start to link to server.
    bool res = m_server->listen(m_socketName);
#if defined(Q_OS_UNIX) && (QT_VERSION >= QT_VERSION_CHECK(4,5,0))
    //Workaround
    if (!res && m_server->serverError() == QAbstractSocket::AddressInUseError)
    {
        QFile::remove(QDir::cleanPath(QDir::tempPath())+QLatin1Char('/')+
                      m_socketName);
        res = m_server->listen(m_socketName);
    }
#endif
    if (!res)
    {
        qWarning("KNSingleCoreApplication: listen on local socket failed, %s",
                 qPrintable(m_server->errorString()));
    }
    //Link the server.
    connect(m_server, &QLocalServer::newConnection,
            this, &KNLocalPeer::receiveConnection);
    //Established the connection, it is now in a server mode.
    return false;
}

bool KNLocalPeer::sendMessage(const QStringList &messages, int timeout)
{
    //Check the client.
    if (!isClient())
    {
        //The server side cannot send message.
        return false;
    }
    //Prepare the socket.
    QLocalSocket socket;
    bool connectionOk = false;
    //Start to connect to server.
    for(int i = 0; i < 2; i++)
    {
        // Try twice, in case the other instance is just starting up.
        socket.connectToServer(m_socketName);
        connectionOk = socket.waitForConnected(timeout>>1);
        //Check the connection state.
        if(connectionOk || i)
        {
            //Connected, mission complete.
            break;
        }
        //Delay several times.
        int ms = 250;
#if defined(Q_OS_WIN)
        Sleep(DWORD(ms));
#else
        struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
#endif
    }
    //Check the connection state.
    if (!connectionOk)
    {
        //Failed to send message.
        return false;
    }
    //Generate the message data.
    QByteArray messageData;
    QDataStream dataWriter(&messageData, QIODevice::WriteOnly);
    dataWriter << messages;
    //Create the socket write to write data.
    QDataStream socketWriter(&socket);
    socketWriter.writeBytes(messageData.constData(), messageData.size());
    //Wait for content transfer.
    bool res = socket.waitForBytesWritten(timeout);
    //Check the server reply.
    res &= socket.waitForReadyRead(timeout);   // wait for ack
    res &= (socket.read(qstrlen(ack)) == ack);
    //Return the result.
    return res;
}

QString KNLocalPeer::applicationId() const
{
    return m_id;
}

void KNLocalPeer::receiveConnection()
{
    //Get the pending connection socket.
    QLocalSocket* socket = m_server->nextPendingConnection();
    //Check the pointer.
    if(!socket)
    {
        //Mission complete for null pointer.
        return;
    }
    //Check the available data.
    while(socket->bytesAvailable() < (int)sizeof(quint32))
    {
        //Wait for the data ready.
        socket->waitForReadyRead();
    }
    //Create the data stream for reading data.
    QDataStream socketReader(socket);
    QByteArray messageData;
    quint32 remaining;
    //Read the remaining bytes.
    socketReader >> remaining;
    //Update the data size.
    messageData.resize(remaining);
    //Prepare the parameters.
    int got = 0;
    char* messageBuffer=messageData.data();
    //Start reading data, until the last byte.
    do
    {
        //Read the data.
        got=socketReader.readRawData(messageBuffer, remaining);
        //Decrease the remaining size.
        remaining-=got;
        //Move the pointer.
        messageBuffer+=got;
    }
    while(remaining && got >= 0 && socket->waitForReadyRead(2000));
    //Check the data size we get.
    if (got < 0)
    {
        //If we didn't ready any data, the must be some error.
        qWarning()<<"QtLocalPeer: Message reception failed"
                  <<socket->errorString();
        //Remove the socket.
        delete socket;
        //Complete.
        return;
    }
    //Give back the ACK data.
    socket->write(ack, qstrlen(ack));
    socket->waitForBytesWritten(1000);
    delete socket;
    //Use a data stream to flush data to string list.
    QStringList messages;
    QDataStream dataReader(&messageData, QIODevice::ReadOnly);
    dataReader >> messages;
    //Emit the messages.
    emit messageReceived(messages);
}
