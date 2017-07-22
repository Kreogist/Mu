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

#ifndef KNMAINWINDOWHEADER_H
#define KNMAINWINDOWHEADER_H

#include "knmainwindowheaderbase.h"

class QBoxLayout;
class QSpacerItem;
class KNConfigure;
class KNMainWindowStatusBar;
class KNMainWindowIconButton;
/*!
 * \brief The KNMainWindowHeader class is a default realize of the main window
 * header.
 */
class KNMainWindowHeader : public KNMainWindowHeaderBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMainWindowHeader widget.
     * \param parent The parent widget.
     */
    explicit KNMainWindowHeader(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMainWindowHeaderBase::addHeaderWidget().
     */
    void addHeaderWidget(QWidget *widget,
                         int stretch=0,
                         Qt::Alignment alignment=0) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from
     * KNMainWindowHeaderBase::addNotificationWidget().
     */
    void addNotificationWidget(QWidget *widget) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMainWindowHeaderBase::setCategoryPlugin().
     */
    void setCategoryPlugin(KNCategoryPlugin *categoryPlugin) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMainWindowHeaderBase::addStatusWidget().
     */
    void addStatusWidget(QWidget *widget) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from
     * KNMainWindowHeaderBase::setAppearanceConfigure().
     */
    void setAppearanceConfigure(KNConfigure *configure) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMainWindowHeaderBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void updateContainerSize();
    void onConfigureChanged();

private:
    QWidget *m_widgetContainer;
    QBoxLayout *m_mainLayout, *m_notificationLayout;
    KNCategoryPlugin *m_categoryPlugin;
    KNConfigure *m_appearanceConfigure;
    KNMainWindowIconButton *m_iconButton;
    KNMainWindowStatusBar *m_statusBar;
};

#endif // KNMAINWINDOWHEADER_H
