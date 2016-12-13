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

#ifndef KNMUSICSTOREGLOBAL_H
#define KNMUSICSTOREGLOBAL_H

#include <QMutex>

#include "knmusicstoreutil.h"

#include <QObject>

/*!
 * \def knMusicStoreGlobal
 * A global pointer referring to the unique music store global object.
 */
#define knMusicStoreGlobal (KNMusicStoreGlobal::instance())

using namespace MusicStoreUtil;

class KNDarkWaitingWheel;
class KNMusicStoreAlbumModel;
/*!
 * \brief The KNMusicStoreGlobal class provides some public resources for the
 * music store framework:\n
 *  1. The error message id and translated text.
 */
class KNMusicStoreGlobal : public QObject
{
    Q_OBJECT
public:
    ~KNMusicStoreGlobal();

    /*!
     * \brief Get the global public instance.
     * \return The instance object pointer.
     */
    static KNMusicStoreGlobal *instance();

    /*!
     * \brief Construct the global instance.
     * \param parent The parent object of the instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the connect state wheel widget pointer.
     * \return The widget pointer.
     */
    QWidget *connectStateWheel();

    /*!
     * \brief Add several connection counter to the state counter.
     * \param counter The pending request number.
     */
    void addConnectionCounter(int counter);

    /*!
     * \brief Reduce several connection counter to the state counter.
     * \param counter The finished request number.
     */
    void reduceConnectionCounter(int counter);

    /*!
     * \brief Get the music store global album model. This is used for album
     * songs displaying.
     * \return The album model pointer.
     */
    KNMusicStoreAlbumModel *albumModel() const;

signals:

public slots:

private slots:
    void retranslate();

private:
    static KNMusicStoreGlobal *m_instance;
    explicit KNMusicStoreGlobal(QObject *parent = 0);
    KNMusicStoreGlobal(const KNMusicStoreGlobal &);
    KNMusicStoreGlobal(KNMusicStoreGlobal &&);

//    QString m_errorText[MusicStoreErrorCount];
    QMutex m_connectLock;
    int m_connectSemaphore;
    KNDarkWaitingWheel *m_connectStateWheel;
    KNMusicStoreAlbumModel *m_albumModel;
};

#endif // KNMUSICSTOREGLOBAL_H
