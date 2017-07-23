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

#ifndef KNMUSICBACKEND_H
#define KNMUSICBACKEND_H

#include <QObject>

/*!
 * \brief The KNMusicBackend class provides the music playing ports for
 * KNMusicDetailTooltipBase and KNMusicHeaderPlayerBase. All the function should
 * be implement and realized. The backend should support play two music
 * concurrency, and control them independently. The volume should be controlled
 * independently as well.\n
 * There is a defination called 'main thread' and another one called 'preview
 * thread'. The main thread will be used to play the file in the header player,
 * main player and any other parts. The preview thread will only be used at
 * these parts: Lyrics, Preview Tooltip, Details. Only for previewing the file.
 */
class KNMusicBackend : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackend class with given parent object.
     * \param parent The parent object.
     */
    KNMusicBackend(QObject *parent = 0) : QObject(parent){}

    /*!
     * \brief Get the duration of the main thread.
     * \return The duration of the main thread music in msecond. If there's no
     * music playing, return -1.
     */
    virtual qint64 duration() const=0;

    /*!
     * \brief Get the current position of the main thread.
     * \return The current playing position of the main thread music in msecond.
     * If there's no music playing, return -1.
     */
    virtual qint64 position() const=0;

    /*!
     * \brief Load a music file from local hard disk. All the file will be treat
     * at local. You can ask the main thread only played a part of the music.
     * This is designed for those huge music file with a cue sheet file to
     * separate them.
     * \param filePath The music file path.
     * \param start The start position of the music. If you want to play from
     * the start of the whole file, leave it there to the default -1.
     * \param duration The end position of the music. If you want to play to the
     * end of the whole file, leave it there to the default -1.
     * \return If we can get the file and send it to the music playing backend,
     * it will be true. Or else it wil be false. To judge whether has been load
     * successful, wait for a loadSuccess() or loadFailed() signal;
     */
    virtual bool loadMusic(const QString &filePath,
                           const qint64 &start=-1,
                           const qint64 &duration=-1)=0;

    /*!
     * \brief Play the main thread.
     */
    virtual void play()=0;

    /*!
     * \brief Pause the main thread.
     */
    virtual void pause()=0;

    /*!
     * \brief Stop the main thread, reset the position to the start position.
     */
    virtual void stop()=0;

    /*!
     * \brief Reset the main thread. The state will be reset to stop, and all
     * memory resource of the main thread will be cleared.
     */
    virtual void reset()=0;

    /*!
     * \brief Get the volume of the main size, range from the minimum volume
     * to maximum volume.
     * \return The volume size.
     */
    virtual int volume() const=0;

    /*!
     * \brief Get the state of the main thread.
     * \return The main thread state.
     */
    virtual int state() const=0;

    /*!
     * \brief Get the duration of the file loaded in preview thread.
     * \return The duration of the preview file. If there's no file in the
     * preview thread, it will be -1.
     */
    virtual qint64 previewDuration() const=0;

    /*!
     * \brief Get the position of the file loaded in the preview thread.
     * \return The current playing position of the file loaded in the preview
     * thread. If there's no file in the preview thread, it will be -1.
     */
    virtual qint64 previewPosition() const=0;

    /*!
     * \brief Load a music file from local to preview thread.
     * \param filePath The music file path.
     * \param start The start position of the music. If you want to play from
     * the start of the whole file, leave it there to the default -1.
     * \param duration The end position of the music. If you want to play to the
     * end of the whole file, leave it there to the default -1.
     * \return If we can get the file and send it to the preview thread, it will
     * be true. Or else it wil be false. To judge whether has been load
     * successful, wait for a previewLoadSuccess() or previewLoadFailed()
     * signal.
     */
    virtual bool previewLoadMusic(const QString &filePath,
                                  const qint64 &start=-1,
                                  const qint64 &duration=-1)=0;

    /*!
     * \brief Play the preview thread.
     */
    virtual void previewPlay()=0;

    /*!
     * \brief Pause the preview thread.
     */
    virtual void previewPause()=0;

    /*!
     * \brief Stop the preview thread and reset the position of the preview
     * thread to the start position.
     */
    virtual void previewStop()=0;

    /*!
     * \brief Reset the preview thread. The state will be reset to stop, and all
     * memory resource of the preview thread will be cleared.
     */
    virtual void previewReset()=0;

    /*!
     * \brief Get the state of the preview thread.
     * \return The state of the preview thread.
     */
    virtual int previewState() const=0;

    /*!
     * \brief Get the minimal volume size of the backend.
     * \return The minimal volume size of the backend.
     */
    virtual int minimalVolume() const=0;

    /*!
     * \brief Get the maximum volume size of the backend.
     * \return The maximum volume size of the backend.
     */
    virtual int maximumVolume() const=0;

    /*!
     * \brief Get whether the backend is in the mute state.
     * \return If the backend has been muted, it will be true.
     */
    virtual bool mute()=0;

signals:
    /*!
     * \brief When the volume size changed, this signal will be emitted.
     * \param volumeSize The size of the volume.
     */
    void volumeChanged(int volumeSize);

    /*!
     * \brief If the main thread is fail to load the current file, this signal
     * will be emitted.
     */
    void loadFailed();

    /*!
     * \brief If the main thread load the current file successfully, this signal
     * will be emitted.
     */
    void loadSuccess();

    /*!
     * \brief When the position of the main thread is changed, this signal will
     * be emitted.
     * \param position The new position of the main thread music.
     */
    void positionChanged(qint64 position);

    /*!
     * \brief When the duration of the main thread is changed, this signal will
     * be emitted. This signal is usually emitted when a new file is loaded.
     * \param duration The duration of the new file.
     */
    void durationChanged(qint64 duration);

    /*!
     * \brief When the position reach the end of the file, this signal will be
     * emitted.
     */
    void finished();

    /*!
     * \brief When you stop the main thread, this signal will be emitted.
     */
    void stopped();

    /*!
     * \brief When the playing of a file is changed, e.g.: from pausing or
     * stopped to playing, this signal will be emitted.
     * \param state The new state of the main thread.
     */
    void playingStateChanged(int state);

    /*!
     * \brief If the preview thread is fail to load the current file, this
     * signal will be emitted.
     */
    void previewLoadFailed();

    /*!
     * \brief If the preview thread load the current file successfully, this
     * signal will be emitted.
     */
    void previewLoadSuccess();

    /*!
     * \brief When the position of the preview thread is changed, this signal
     * will be emitted.
     * \param position The new position of the preview thread music.
     */
    void previewPositionChanged(qint64 position);

    /*!
     * \brief When the duration of the preview thread is changed, this signal
     * will be emitted.
     * \param duration The duration of the new file in preview thread.
     */
    void previewDurationChanged(qint64 duration);

    /*!
     * \brief When the position of preivew thread reach the end of the file,
     * this signal will be emitted.
     */
    void previewFinished();

    /*!
     * \brief When you stop the preview thread, this signal will be emitted.
     */
    void previewStopped();

    /*!
     * \brief When the playing of the preview file is changed, this signal will
     * be emitted.
     * \param state The new state of the preview thread.
     */
    void previewPlayingStateChanged(int state);

    /*!
     * \brief When mute state of the backend is changed, this signal will be
     * emitted.
     * \param mute The mute state of the backend.
     */
    void muteStateChanged(bool mute);

public slots:
    /*!
     * \brief Save the current state of the backend, it must be used with a
     * restore. It will only saved for the runtime of the application.\n
     * This function is used for renaming files or write to the file which is
     * currently playing. Call save() first and then the state will be reset,
     * but current state will save. Then you can do the rename or tag editing to
     * the file. After doing all the things, call the restore() function.
     */
    virtual void save()=0;

    /*!
     * \brief Restore the saving state. Function save() must be called first.\n
     * If the playing file is renamed, then the updatedFilePath parameter should
     * be passed the new file path. Or else just stay empty, it will
     * automatically reload the file.
     * \param updatedFilePath The renamed file path. If the file is not renamed,
     * stay empty.
     */
    virtual void restore(const QString &updatedFilePath=QString())=0;

    /*!
     * \brief Change the mute state to the different side.
     */
    virtual void changeMuteState()=0;

    /*!
     * \brief Set the mute state to specific state.
     * \param mute To mute the backend, set true.
     */
    virtual void setMute(bool mute)=0;

    /*!
     * \brief Set the volume to a specific size. The size should be between
     * minimalVolume() and maximumVolume(). Or else it won't change anything.
     * \param volumeSize The prefer volume size.
     */
    virtual void setVolume(int volumeSize)=0;

    /*!
     * \brief Raise volume up a level.
     */
    virtual void volumeUp()=0;

    /*!
     * \brief Turn down the volume a level.
     */
    virtual void volumeDown()=0;

    /*!
     * \brief This is slot is designed for the button which combines the play
     * and pause button together. When a music file is played, it will be set to
     * execute pause. When a music is paused, this slot will be set to execute
     * play.
     */
    virtual void playNPause()=0;

    /*!
     * \brief Set the current position of the main thread. If there's no file
     * loaded in the main thread or the position is not positive and less than
     * the duration, nothing will happened.
     * \param position The new position of the main thread.
     */
    virtual void setPosition(const qint64 &position)=0;

    /*!
     * \brief Set the current position of the preview thread. It works very
     * familiar to the setPosition() function.
     * \param position The new position of the preview thread.
     */
    virtual void setPreviewPosition(const qint64 &position)=0;
};

#endif // KNMUSICBACKEND_H
