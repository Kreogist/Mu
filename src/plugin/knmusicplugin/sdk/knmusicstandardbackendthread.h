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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTANDARDBACKENDTHREAD_H
#define KNMUSICSTANDARDBACKENDTHREAD_H

#include <QObject>

/*!
 * \brief The KNMusicStandardBackendThread class provides a interface for
 * standard music backend thread. All the thread should play their own music
 * without borthering other threads.\n
 * This thread abstract interface is used for standard backend.
 */
class KNMusicStandardBackendThread : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStandardBackendThread object with given parent.
     * \param parent The parent standard backend object.
     */
    KNMusicStandardBackendThread(QObject *parent = 0):QObject(parent){}

    /*!
     * \brief Load a music file.
     * \param filePath The file path.
     * \return If the thread load the music successful, then return true.
     */
    virtual bool loadFile(const QString &filePath)=0;

    /*!
     * \brief Clear the playing state and the playing file information. The
     * thread after reset will be free.
     */
    virtual void reset()=0;

    /*!
     * \brief Stop playing. This will reset the playing state.
     */
    virtual void stop()=0;

    /*!
     * \brief Start to play the thread.
     */
    virtual void play()=0;

    /*!
     * \brief Pause a playing thread. If the thread is stopped this function
     * should do nothing.
     */
    virtual void pause()=0;

    /*!
     * \brief Get the volume of the current thread.
     * \return The volume size of the thread.
     */
    virtual int volume()=0;

    /*!
     * \brief Get the duration of the playing file. If there's no file playing
     * or the file loaded failed, this should return -1.
     * \return The duration of the playing file in msecond.
     */
    virtual qint64 duration()=0;

    /*!
     * \brief Get the current position of a playing file. If there's no file
     * playing or the file laoded failed, this should return -1. If the current
     * thread is loaded and it's stopped, this should be 0.
     * \return The position of the playing file in msecond.
     */
    virtual qint64 position()=0;

    /*!
     * \brief Get the playing state of the thread.
     * \return The playing state.
     */
    virtual int state() const=0;

    /*!
     * \brief Set the specific section of the music, treat the section as a
     * whole music file. The start position should be the whether -1 or a number
     * greater than 0 and lesser than duration(). The start position add the
     * duration should not greater than the duration().\n
     * If the duration is not valid, we will play to the end of the file.
     * \param start The start position in msecond.
     * \param duration The duration of the section in msecond.
     */
    virtual void setPlaySection(const qint64 &start=-1,
                                const qint64 &duration=-1)=0;

    /*!
     * \brief Get whether the current proxy is using proxy to playing URL.
     * \return If the backend is using proxy to playing the online data, this
     * function will return true.
     */
    virtual bool isUsingProxy()=0;

signals:
    /*!
     * \brief When url is fully loaded, this signal will emitted.
     */
    void urlLoadComplete();

    /*!
     * \brief When load the file failed, this signal will emitted.
     */
    void loadFailed();

    /*!
     * \brief When load the file successfully, this signal will emitted.
     */
    void loadSuccess();

    /*!
     * \brief When the duration changed, this signal will emitted.
     * \param duration The new duration of the file.
     */
    void durationChanged(qint64 duration);

    /*!
     * \brief When the file is playing, and the position is changing, this
     * signal will emitted.
     * \param position The new position while playing.
     */
    void positionChanged(qint64 position);

    /*!
     * \brief When the state of the thread is changed, this signal will emitted.
     * \param state The new state of the thread.
     */
    void stateChanged(int state);

    /*!
     * \brief When the thread finishes its playing, this signal will emitted.
     */
    void finished();

    /*!
     * \brief When the thread stopped playing, this signal will emitted.
     */
    void stopped();

public slots:
    /*!
     * \brief Save the state of the current thread. The definition of the save
     * and restore state here is the same as KNMusicBackend.
     */
    virtual void save()=0;

    /*!
     * \brief Restore the state of the current thread.
     */
    virtual void restore(const QString &updatedFilePath=QString())=0;

    /*!
     * \brief Set the volume size of this thread.
     * \param volume The volume size of the thread.
     */
    virtual void setVolume(const int &volume)=0;

    /*!
     * \brief Change the position of the current thread.
     * \param position The position of the specifc song.
     */
    virtual void setPosition(const qint64 &position)=0;

    /*!
     * \brief Enabled or disable the proxy server when playing URL.
     * \param enabled To enable the proxy server, set true to enabled.
     */
    virtual void setProxyEnabled(bool enabled)=0;
};

#endif // KNMUSICSTANDARDBACKENDTHREAD_H
