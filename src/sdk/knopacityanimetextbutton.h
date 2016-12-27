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

#ifndef KNOPACITYANIMETEXTBUTTON_H
#define KNOPACITYANIMETEXTBUTTON_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNAnimeTextButton class provides the a button which display the
 * text of the button, and the background, text opacity is changed mouse
 * sensitive.\n
 * This button should be use only when it is necessary to display text.\n
 * The button is familiar with the opacity anime button, however, this button
 * will display the text, and the anime button will display pixmap.
 */
class KNOpacityAnimeTextButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAnimeTextButton widget.
     * \param parent The parent widget.
     */
    explicit KNOpacityAnimeTextButton(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractButton::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the current image opacity for painting.
     * \return The opacity, range in 0.0 to 1.0.
     */
    qreal imageOpacity();

    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::focusInEvent().
     */
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionOpacityChanged(const int &opacity);

private:
    inline void startAnime(const int &endFrame);
    QTimeLine *m_mouseAnime;
    qreal m_textOpacity;
};

#endif // KNOPACITYANIMETEXTBUTTON_H
