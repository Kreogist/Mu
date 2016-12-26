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

#ifndef KNMUSICSTOREERRORDIMMER_H
#define KNMUSICSTOREERRORDIMMER_H

#include <QHash>

#include <QWidget>

class QLabel;
class QTimeLine;
/*!
 * \brief The KNMusicStoreErrorDimmer class is designed to be used as two ways.
 * First, it will be used as the shadow widget for the content, second, it will
 * display the error information.\n
 * The show and hide of the widget will be animation in and out. The icon will
 * be only one error icon, and the button will be only retry and okay. The retry
 * button could be hidden.
 */
class KNMusicStoreErrorDimmer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construc a KNMusicStoreErrorDimmer widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreErrorDimmer(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the error and display the error.
     * \param errorType The type of the error ID. 0 for Qt error index. 1 for
     * customized error id.
     * \param errorId The error ID of the error.
     */
    void showDimmer(int errorType, int errorId);

    /*!
     * \brief Hide the dimmer. All the text will be hidden first, and the dimmer
     * will fade out and hide the widget.
     */
    void hideDimmer();

    /*!
     * \brief Clear the error id and code stores in the error dimmer.
     */
    void reset();

private slots:
    void retranslate();
    void onThemeChanged();
    void onTimeLineFinish();
    void onBackgroundAlphaChange(int frame);

private:
    inline void updateLabelText();
    inline void startAnime(int endFrame);
    inline void setLabelVisible(bool visible);
    QString m_connectionError, m_contentError,
            m_checkAndRetrySolution, m_contactSolution;
    QTimeLine *m_background;
    QLabel *m_title, *m_description;
    int m_errorType, m_errorId;
};

#endif // KNMUSICSTOREERRORDIMMER_H
