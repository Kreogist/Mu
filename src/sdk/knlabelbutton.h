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

#ifndef KNLABELBUTTON_H
#define KNLABELBUTTON_H

#include <QLabel>

/*!
 * \brief The KNLabelButton class provides a label which could be clicked. It
 * extends the QLabel widget and could operate with the mouse click and double
 * click event.
 */
class KNLabelButton : public QLabel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNLabelButton widget with given label.
     * \param parent The parent widget.
     */
    explicit KNLabelButton(QWidget *parent = 0);

    /*!
     * \brief Get whether the cursor will be changed when mouse is in.
     * \return If the cursor will change to hand, this will return true.
     */
    bool changeCursor() const;

signals:
    /*!
     * \brief When the widget is clicked, this signal will be emitted.
     */
    void clicked();

    /*!
     * \brief When the mouse is pressed on this widget, this signal will be
     * emitted.
     */
    void pressed();

    /*!
     * \brief When the pressed mouse released on this widget, this signal will
     * be emitted.
     */
    void released();

    /*!
     * \brief When user double clicked mouse on this widget, this signal will be
     * emitted.
     */
    void doubleClicked();

public slots:
    /*!
     * \brief Set whether the mouse will be changed to hand shape.
     * \param changeCursor To change the moving cursor, set to true.
     */
    void setChangeCursor(bool changeCursor);

protected:
    /*!
     * \brief Reimplemented from QLabel::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLabel::mouseDoubleClickEvent().
     */
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLabel::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNLabelButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNLabelButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_pressed, m_changeCursor;
};

#endif // KNLABELBUTTON_H
