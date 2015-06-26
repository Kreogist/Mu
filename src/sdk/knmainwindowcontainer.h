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
     * save the frist widget you set.
     */
    void setPreferencePanel(QWidget *preferencePanel);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private:
    inline void updateTheStackRelationship();
    inline void setWidget(const int &index, QWidget *widget);
    enum ContainerWidgetElement
    {
        Header,
        MainWidget,
        PreferencePanel,
        ContainerWidgetCount
    };
    QWidget *m_elementWidget[ContainerWidgetCount];
};

#endif // KNMAINWINDOWCONTAINER_H
