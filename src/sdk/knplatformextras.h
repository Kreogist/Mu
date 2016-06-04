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

#ifndef KNPLATFORMEXTRAS_H
#define KNPLATFORMEXTRAS_H

#include <QObject>

class QMainWindow;
/*!
 * \brief The KNPlatformExtras class provides an abstract class for platform
 * independent operations ports.\n
 * On different platforms, we may provide different functions for different
 * paltform. All theses functions should be implemented from this class and
 * realize their own codes.
 */
class KNPlatformExtras : public QObject
{
    Q_OBJECT
public:
    enum LoopStateButtonState
    {
        ButtonNoRepeat,
        ButtonRepeat,
        ButtonRepeatAll,
        ButtonShuffle,
        LoopStateButtonStateCount
    };
    /*!
     * \brief Construct a KNPlatformExtras object.
     * \param parent The parent object.
     */
    KNPlatformExtras(QObject *parent = 0): QObject(parent){}

    /*!
     * \brief Get the main window object.
     * \param mainWindow The main window pointer.
     */
    virtual void setMainWindow(QMainWindow *mainWindow)=0;

signals:
    /*!
     * \brief When the object is asking for playing current song, this signal
     * will be emitted.
     */
    void requirePlay();

    /*!
     * \brief When the object is asking for pausing current song, this signal
     * will be emitted.
     */
    void requirePause();

    /*!
     * \brief When the object is asking for playing the next song in the current
     * playing model, this signal will be emitted.
     */
    void requirePlayNext();

    /*!
     * \brief When the object is asking for playing the previous song in the
     * current model, this signal will be emitted.
     */
    void requirePlayPrev();

    /*!
     * \brief When the object is asking for turn the volume up, this signal will
     * be emitted.
     */
    void requireVolumeUp();

    /*!
     * \brief When the object is asking for turn the volume down, this signal
     * will be emitted.
     */
    void requireVolumeDown();

    /*!
     * \brief When the object is asking for change the state of mute, this
     * signal will be emitted.
     */
    void requireChangeMuteState();

    /*!
     * \brief When the object is asking for change the state of loop, this
     * signal will be emitted.
     */
    void requireChangeLoopState();

public slots:
    /*!
     * \brief When the plaing state is changed, this slot will be called to
     * process the action.
     * \param isPlay Current playing state.
     */
    virtual void onActionPlayStateChanged(const bool &isPlay)=0;

    /*!
     * \brief When the volume mute state is changed, this slot will be called to
     * process the action.
     * \param isMute Current volume mute state.
     */
    virtual void onActionMuteStateChanged(const bool &isMute)=0;

    /*!
     * \brief When the playing loop state is changed, this slot will be called
     * to process the action.
     * \param loopState Current loop state. Defined in LoopStateButtonState.
     */
    virtual void onActionLoopStateChanged(const int &loopState)=0;

    /*!
     * \brief Load preference of platform special from the configure files.
     */
    virtual void loadPreference()=0;

    /*!
     * \brief Save preference of platform special to the configure files.
     */
    virtual void savePreference()=0;
};

#endif // KNPLATFORMEXTRAS_H
