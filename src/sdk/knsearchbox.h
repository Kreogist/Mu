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

#ifndef KNSEARCHBOX_H
#define KNSEARCHBOX_H

#include <QLineEdit>

class QTimeLine;
/*!
 * \brief The KNSearchBox class provides you a mouse sense search box. The icon
 * of the search box can be tweaked by setSearchIcon() function.\n
 * You can give a focus source widget as well if you want the search box
 * finished input and automatically jumpped to.
 */
class KNSearchBox : public QLineEdit
{
    Q_OBJECT
public:
    /*!
     * \brief Constuct a KNSearchBox widget with given parent widget.
     * \param parent The parent widget.
     */
    explicit KNSearchBox(QWidget *parent = 0);

    /*!
     * \brief Get the search icon.
     * \return The search icon pixmap. If you haven't set it before, it will be
     * a null pixmap.
     */
    QPixmap searchIcon() const;

    /*!
     * \brief Set the search icon.
     * \param searchIcon The new search icon.
     */
    void setSearchIcon(const QPixmap &searchIcon);

    /*!
     * \brief Get the focus source widget pointer.
     * \return Give back the source widget pointer.
     */
    QWidget *focusSource() const;

    /*!
     * \brief Set the focus source widget.
     * \param focusSource The widget pointer.
     */
    void setFocusSource(QWidget *focusSource);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QLineEdit::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QLineEdit::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QLineEdit::focusInEvent().
     */
    void focusInEvent(QFocusEvent *event);

    /*!
     * \brief Reimplemented from QLineEdit::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event);

    /*!
     * \brief Reimplemented from QLineEdit::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Reimplemented from QLineEdit::keyPressEvent().
     */
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onActionThemeChanged();
    void onActionMouseInOut(const int &frame);
    void onActionFocusInOut(const int &frame);

private:
    inline QTimeLine *generateTimeLine();
    inline void startAnime(QTimeLine *timeLine, const int &end);
    QPixmap m_searchIcon;
    QColor m_baseColor;
    QTimeLine *m_mouseInOut, *m_focusInOut;
    QWidget *m_focusSource;
};

#endif // KNSEARCHBOX_H
