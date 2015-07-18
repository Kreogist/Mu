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

#ifndef KNDROPPROXYCONTAINER_H
#define KNDROPPROXYCONTAINER_H

#include <QList>
#include <QUrl>

#include <QWidget>

/*!
 * \brief The KNDropProxyContainer class will emit urlsDropped() signals when
 * files and urls dropped on the widget.
 */
class KNDropProxyContainer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNDropProxyContainer class.
     * \param parent The parent widget pointer.
     */
    explicit KNDropProxyContainer(QWidget *parent = 0);

signals:
    /*!
     * \brief When urls dropped on this widget, this signal will be emitted.
     * \param urls The url list.
     */
    void urlsDropped(QList<QUrl> urls);

    /*!
     * \brief When urls draged into this widget, this signal will be emitted.
     */
    void dragEntered();

    /*!
     * \brief When the urls dropped done, this signal will be emitted.
     */
    void dropped();

    /*!
     * \brief Before this widget is going to be shown, this signal will be
     * emitted.
     */
    void aboutToShow();

    /*!
     * \brief Before this widget is going to be hide, this signal will be
     * emitted.
     */
    void aboutToHide();

public slots:

protected:
    /*!
     * \brief Reimpelment from QWidget::dragEnterEvent().
     */
    void dragEnterEvent(QDragEnterEvent *event);

    /*!
     * \brief Reimpelment from QWidget::dropEvent().
     */
    void dropEvent(QDropEvent *event);

    /*!
     * \brief Reimpelment from QWidget::showEvent().
     */
    void showEvent(QShowEvent *event);

    /*!
     * \brief Reimpelment from QWidget::hideEvent().
     */
    void hideEvent(QHideEvent *event);
};

#endif // KNDROPPROXYCONTAINER_H
