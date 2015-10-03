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

#ifndef KNMUSICMAINPLAYERCONTENTSWITCHER_H
#define KNMUSICMAINPLAYERCONTENTSWITCHER_H

#include <QWidget>

class KNOpacityAnimeButton;
class KNMusicMainPlayerContentSwitcher : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicMainPlayerContentSwitcher(QWidget *parent = 0);

signals:
    void requirePrevious();
    void requireNext();

public slots:
    void setCurrentIndex(const int &index);

protected:
    /*!
     * \brief resizeEvent
     * \param event
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief paintEvent
     * \param event
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    inline KNOpacityAnimeButton *generateButton(const QString &imagePath);
    KNOpacityAnimeButton *m_previous, *m_next;
    int m_currentIndex;
};

#endif // KNMUSICMAINPLAYERCONTENTSWITCHER_H
