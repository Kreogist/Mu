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

#ifndef KNMAINWINDOWCONTAINER_H
#define KNMAINWINDOWCONTAINER_H

#include <QWidget>

class QPropertyAnimation;
class QParallelAnimationGroup;
class QGraphicsOpacityEffect;
/*!
 * \brief The KNMainWindowContainer class is the container of all the widget.
 * This class will be contructed by KNMainWindow. All the widget of the main
 * window will finally add on this container.
 */
class KNMainWindowContainer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNMainWindowContainer class.
     * \param parent The parent main window.
     */
    explicit KNMainWindowContainer(QWidget *parent = 0);

    /*!
     * \brief Get the current header widget.
     * \return The header widget.
     */
    QWidget *header() const;

    /*!
     * \brief Get the main widget.
     * \return The main widget.
     */
    QWidget *mainWidget() const;

    /*!
     * \brief Get the preference panel widget.
     * \return The preference widget.
     */
    QWidget *preferencePanel() const;

    /*!
     * \brief Get the main player widget.
     * \return The main player widget.
     */
    QWidget *mainPlayer() const;

signals:

public slots:
    /*!
     * \brief Set the header widget.
     * \param header The header widget pointer. It will only save the first
     * widget you set.
     */
    void setHeader(QWidget *header);

    /*!
     * \brief Set the main widget.
     * \param mainWidget The main widget pointer. It will only save the first
     * widget you set.
     */
    void setMainWidget(QWidget *mainWidget);

    /*!
     * \brief Set the preference panel widget.
     * \param preferencePanel The perference panel widget pointer. It will only
     * save the first widget you set.
     */
    void setPreferencePanel(QWidget *preferencePanel);

    /*!
     * \brief Set the main player widget.
     * \param mainPlayer The main player widget pointer. It will only save the
     * first widget you set.
     */
    void setMainPlayer(QWidget *mainPlayer);

    /*!
     * \brief Show the preference widget via animation.
     */
    void showPreference();

    /*!
     * \brief Hide the preference widget via animation.
     */
    void hidePreference();

    /*!
     * \brief Show the main player widget via animation.
     */
    void showMainPlayer();

    /*!
     * \brief Hide the main player widget via animation.
     */
    void hideMainPlayer();

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private:
    inline void updateTheStackRelationship();
    inline void setWidget(const int &index, QWidget *widget);
    inline void updateShowPreference();
    inline void updateHidePreference();
    inline void updateShowMainPlayer();
    inline void updateHideMainPlayer();

    inline QPropertyAnimation *generateAnime(const QByteArray &propertyName);
    enum ContainerWidgetElement
    {
        Header,
        MainWidget,
        MainPlayer,
        PreferencePanel,
        ContainerWidgetCount
    };
    enum ContainerWidgetAnimations
    {
        AnimeHeader,
        AnimeMainWidget,
        AnimeMainPlayer,
        AnimeMainPlayerHeader,
        AnimePreferencePanel,
        ContainerAnimeCount
    };
    QPropertyAnimation *m_elementAnime[ContainerAnimeCount];
    QWidget *m_elementWidget[ContainerWidgetCount];
    QParallelAnimationGroup *m_preferenceAnimeGroup, *m_mainPlayerAnimeGroup;
    QPropertyAnimation *m_preferenceOpacity;
    QGraphicsOpacityEffect *m_preferenceOpacityEffect;
    int m_headerHeight;
};

#endif // KNMAINWINDOWCONTAINER_H
