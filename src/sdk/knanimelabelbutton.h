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
#ifndef KNANIMELABELBUTTON_H
#define KNANIMELABELBUTTON_H

#include "knlabelbutton.h"

class QTimeLine;
/*!
 * \brief The KNAnimeLabelButton class provides a opacity change of the label
 * button. We will change the palette when the mouse move in and out.\n
 * This widget is bundled with the theme manager, it will automatically detect
 * the palette changed signal from the theme manager and update itself.
 */
class KNAnimeLabelButton : public KNLabelButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAnimeLabelButton widget.
     * \param parent The parent widget.
     */
    explicit KNAnimeLabelButton(QWidget *parent = 0);

    /*!
     * \brief Update the object name, and update the palette.
     * \param name The object name.
     */
    void updateObjectName(const QString &name);

    /*!
     * \brief Set the text brightness chaning range, from min to max.
     * \param min The minimum brightness.
     * \param max The maximum brightness.
     */
    void setRange(int min, int max);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNLabelButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNLabelButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onUpdatePalette(int frame);

private:
    inline void startAnime(int endFrame);
    QTimeLine *m_timeLine;
    int m_minBrightness, m_maxBrightness;
};

#endif // KNANIMELABELBUTTON_H
