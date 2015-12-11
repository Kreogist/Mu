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

#ifndef KNMESSAGEBOXCONTENT_H
#define KNMESSAGEBOXCONTENT_H

#include <QWidget>

class QBoxLayout;
/*!
 * \brief The KNMessageBoxContent class provides a holder for content widget. It
 * will also add two shadow on the top and bottom as the dialog in SAO.
 */
class KNMessageBoxContent : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMessageBoxContent widget with given parent.
     * \param parent The parent widget.
     */
    explicit KNMessageBoxContent(QWidget *parent = 0);

    /*!
     * \brief Get the final target size of the content.
     * \return The target size of the content.
     */
    QSize targetSize() const;

    /*!
     * \brief Get the content widget pointer. If the content widget is not set,
     * it will be nullptr.
     * \return The content widget pointer.
     */
    QWidget *content();

    /*!
     * \brief Set the content widget.
     * \param content The content widget.
     * \param autoDelete The auto delete flag. It will be true as default.
     */
    void setContent(QWidget *content, bool autoDelete=true);


signals:

public slots:
    /*!
     * \brief Show the content widget. It will do nothing if the content isn't
     * set.
     */
    void showContent();

    /*!
     * \brief Hide the content widget. It will do nothing if the content isn't
     * set.
     */
    void hideContent();

    /*!
     * \brief Check the auto delete flag, and reset the parent widget of the
     * content widget.
     */
    void checkAutoDelete();

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

private:
    QLinearGradient m_upShadowGradient, m_downShadowGradient;
    QBoxLayout *m_mainLayout;
    QWidget *m_content;
    bool m_autoDelete;
};

#endif // KNMESSAGEBOXCONTENT_H
