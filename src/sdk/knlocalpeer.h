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

#ifndef KNLOCALPEER_H
#define KNLOCALPEER_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QDir>

#include "knlockedfile.h"

#include <QObject>

/*!
 * \brief The KNLocalPeer class is a class ported from QtLocalPeer. However, the
 * original codes is quite messy and not fit our requirements, this version is
 * created base on the original codes.
 */
class KNLocalPeer : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNLocalPeer object.
     * \param parent The parent object.
     */
    explicit KNLocalPeer(QObject *parent = 0, const QString &appId = QString());
    ~KNLocalPeer();

    /*!
     * \brief Check whether the instance is client.
     * \return If the peer is the at client mode, return true.
     */
    bool isClient();

    /*!
     * \brief Send the message in string list mode.
     * \param message The message in string list mode.
     * \param timeout Waiting response time.
     * \return Whether the message is sent.
     */
    bool sendMessage(const QStringList &messages, int timeout);

    /*!
     * \brief Get the application identifier.
     * \return The application ID.
     */
    QString applicationId() const;

signals:
    /*!
     * \brief When new messages received, this signal will be emitted.
     * \param message The message data of the new parameters.
     */
    void messageReceived(const QStringList &message);

public slots:

private slots:
    void receiveConnection();

private:
    static const char* ack;
    KNLockedFile m_lockFile;
    QString m_id;
    QString m_socketName;
    QLocalServer* m_server;
    bool m_lockFileCreated;
};

#endif // KNLOCALPEER_H
