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

#ifndef KNMUSICSTANDARDBACKEND_H
#define KNMUSICSTANDARDBACKEND_H

#include "knmusicbackend.h"

class KNMusicStandardBackendThread;
/*!
 * \brief The KNMusicStandardBackend class provides a standard backend interface
 * for simplifing the backend development. The backend which use this interface
 * should support play music concurrency and the only thing you have to do is to
 * write a thread for standard backend and set the main and preview thread.
 */
class KNMusicStandardBackend : public KNMusicBackend
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStandardBackend class with given object.
     * \param parent The parent object.
     */
    explicit KNMusicStandardBackend(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicBackend::duration().
     */
    qint64 duration() const Q_DECL_OVERRIDE;


    /*!
     * \brief  Reimplemented from KNMusicBackend::position().
     */
    qint64 position() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::loadMusic().
     */
    bool loadMusic(const QString &filePath,
                   const qint64 &start=-1,
                   const qint64 &duration=-1) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::play().
     */
    void play() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::pause().
     */
    void pause() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::stop().
     */
    void stop() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::previewDuration().
     */
    qint64 previewDuration() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::previewPosition().
     */
    qint64 previewPosition() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::previewLoadMusic().
     */
    bool previewLoadMusic(const QString &filePath,
                          const qint64 &start=-1,
                          const qint64 &duration=-1) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::previewPlay().
     */
    void previewPlay() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::previewPause().
     */
    void previewPause() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::previewStop().
     */
    void previewStop() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::previewReset().
     */
    void previewReset() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Set the main backend thread.
     * \param thread The main standard backend thread object.
     */
    void setMainThread(KNMusicStandardBackendThread *thread);

    /*!
     * \brief Set the preview backend thread.
     * \param thread The preview standard backend thread object.
     */
    void setPreviewThread(KNMusicStandardBackendThread *thread);

    /*!
     * \brief Reimplemented from KNMusicBackend::setPosition().
     */
    void setPosition(const qint64 &position) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicBackend::setPreviewPosition().
     */
    void setPreviewPosition(const qint64 &position) Q_DECL_OVERRIDE;

private:
    inline qint64 threadDuration(KNMusicStandardBackendThread *thread) const;
    inline qint64 threadPosition(KNMusicStandardBackendThread *thread) const;
    inline bool threadLoadMusic(KNMusicStandardBackendThread *thread,
                                const QString &filePath,
                                const qint64 &start,
                                const qint64 &duration) const;
    inline void threadPlay(KNMusicStandardBackendThread *thread);
    inline void threadPause(KNMusicStandardBackendThread *thread) ;
    inline void threadStop(KNMusicStandardBackendThread *thread);
    inline void threadReset(KNMusicStandardBackendThread *thread);
    inline void threadSetPosition(KNMusicStandardBackendThread *thread,
                                  const qint64 &position);

    KNMusicStandardBackendThread *m_main, *m_preview;
};

#endif // KNMUSICSTANDARDBACKEND_H
