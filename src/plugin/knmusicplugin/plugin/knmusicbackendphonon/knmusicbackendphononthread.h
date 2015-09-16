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

#ifndef KNMUSICBACKENDPHONONTHREAD_H
#define KNMUSICBACKENDPHONONTHREAD_H

#include <QEasingCurve>

#include <phonon4qt5/phonon/MediaObject>
#include <phonon4qt5/phonon/AudioOutput>
#include <phonon4qt5/phonon/MediaSource>

#include "knmusicutil.h"

#include "knmusicstandardbackendthread.h"

using namespace MusicUtil;

class KNMusicBackendPhononThread : public KNMusicStandardBackendThread
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendPhononThread object.
     * \param parent The parent object. This backend thread should only be
     * constructed by the KNMusicBackendPhonon.
     */
    explicit KNMusicBackendPhononThread(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::loadFile().
     */
    bool loadFile(const QString &filePath) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::stop().
     */
    void stop() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::play().
     */
    void play() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::pause().
     */
    void pause() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::volume().
     */
    int volume() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::duration().
     * \return
     */
    qint64 duration() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::position().
     */
    qint64 position() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::state().
     */
    int state() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPlaySection().
     */
    void setPlaySection(const qint64 &start=-1,
                        const qint64 &duration=-1) Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setVolume().
     */
    void setVolume(const int &volume) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPosition().
     */
    void setPosition(const qint64 &position) Q_DECL_OVERRIDE;

private slots:
    void onActionDurationChanged(const qint64 &totalTime);
    void onActionPositionChanged(const qint64 &time);
    void onActionStateChanged(const Phonon::State &state,
                              const Phonon::State &oldState);

private:
    inline void resetMediaInformation();
    inline void checkStartAndEndPosition();

    //Set volume tweak easing curve.
    QEasingCurve m_volumeCurve;
    //Player elements.
    Phonon::MediaSource m_musicSource;
    Phonon::MediaObject *m_mediaObject;
    Phonon::AudioOutput *m_audioOutput;

    //Volume size.
    qreal m_volumeSize, m_volumeValue;
    //Player information.
    qint64 m_totalDuration,
           m_duration,
           m_startPosition,
           m_endPosition;
    int m_state;
    bool m_ticking;
};

#endif // KNMUSICBACKENDPHONONTHREAD_H
