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
#ifndef KNSINGLETONAPPLICATION_H
#define KNSINGLETONAPPLICATION_H

#include <QApplication>

class QLocalServer;
class QSharedMemory;
/*!
 * \brief The KNSingletonApplication class manages a singleton GUI application's
 * control flow and main settings.\n
 * KNSingletonApplication must be constructed with a unique key. This is the
 * only identity information for KNSingletonApplication to check whether there's
 * another instance is being created. You can't change this unique key after
 * KNSingletonApplication is constrcuted.\n
 * It will send the arguments from operating system to the existed instance if
 * there is already a KNSingletonApplication existed. KNSingletonApplication
 * will emit messageAvailble() signal when it gets any new arguments from the
 * other instance.
 */
class KNSingletonApplication : public QApplication
{
    Q_OBJECT
public:
    /*!
     * \brief Initializes the QApplication class and save the unique key for
     * singleton detecting. Pass the argc and argv from system to the
     * QApplication.
     * \param argc Argument count from operating system.
     * \param argv Argument values from operating system.
     * \param uniqueKey The unique key of the application.
     */
    explicit KNSingletonApplication(int &argc,
                                    char **argv,
                                    const QString uniqueKey);

    /*!
     * \brief Return true if the current instance is running.
     * \return Instance running states.
     */
    bool isInstanceRunning() const;

signals:
    /*!
     * \brief This signal is emitted when new messages received.
     * \param messages The message received.
     */
    void messageAvailable(QStringList messages);

public slots:

private slots:
    void onMessageReceive();

private:
    inline bool sendMessages(const QString &uniqueKey,
                             const QStringList &messages);

    QLocalServer *m_messageServer;
    QSharedMemory *m_uniqueKeyMemeory;
    bool m_isInstanceRunning;
};

#endif // KNSINGLETONAPPLICATION_H
