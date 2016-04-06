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
#ifndef KNMUSICSTORETITLEBAR_H
#define KNMUSICSTORETITLEBAR_H

#include "knmousesensewidget.h"

class QBoxLayout;
class QSignalMapper;
class KNAnimeLabelButton;
/*!
 * \brief The KNMusicStoreTitleBar class provides a header of all the title
 * widget.
 */
class KNMusicStoreTitleBar : public KNMouseSenseWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreTitleBar widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreTitleBar(QWidget *parent = 0);

    /*!
     * \brief Append label to the title bar.
     * \param indicator The indicator widget.
     * \param label The label.
     */
    void appendLabel(QWidget *indicator, KNAnimeLabelButton *label);

signals:

public slots:

private slots:
    void retranslate();
    void onActionLabelPress(int index);

private:
    QList<QWidget *> m_indicatorStack;
    QList<KNAnimeLabelButton *> m_labelStack;
    QBoxLayout *m_buttonLayout;
    KNAnimeLabelButton *m_homeButton;
};

#endif // KNMUSICSTORETITLEBAR_H
