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

#ifndef KNEMPTYSTATEWIDGET_H
#define KNEMPTYSTATEWIDGET_H

#include <QList>
#include <QUrl>
#include <QWidget>

/*!
 * \brief The KNEmptyStateWidget class provides a two-state stacked widget. It
 * can hold two widget pointers: empty widget and content widget. If you called
 * showEmptyWidget() function, the empty widget will be shown. If you called
 * showContentWidget(), the content widget will be shown.
 */
class KNEmptyStateWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNEmptyStateWidget class.
     * \param parent The parent widget.
     */
    explicit KNEmptyStateWidget(QWidget *parent = 0);

    /*!
     * \brief Get the content widget pointer.
     * \return The content widget. If you haven't set it before, it will be
     * nullptr.
     */
    QWidget *contentWidget() const;

    /*!
     * \brief Set the content widget.
     * \param contentWidget The content widget pointer.
     */
    void setContentWidget(QWidget *contentWidget);

    /*!
     * \brief Get the empty widget pointer.
     * \return The empty widget pointer. If you haven't set it before, it will
     * be nullptr.
     */
    QWidget *emptyWidget() const;

    /*!
     * \brief Set the empty widget.
     * \param emptyWidget The empty widget pointer.
     */
    void setEmptyWidget(QWidget *emptyWidget);

signals:
    /*!
     * \brief When the user pressed the 'New Playlist', this signal will be
     * emitted to ask to create a new playlist.
     */
    void requireCreatePlaylist();

    /*!
     * \brief When the user is required to import such playlists, this signal
     * will be emitted to ask to import some files. Those files are stored in
     * the url list.
     */
    void requireImportPlaylist();

public slots:
    /*!
     * \brief Swithch the current widget to the empty widget.
     */
    void showEmptyWidget();

    /*!
     * \brief Switch the current widget to the content widget.
     */
    void showContentWidget();

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget *m_emptyWidget, *m_contentWidget;
    bool m_isEmptyWidget;
};

#endif // KNEMPTYSTATEWIDGET_H
