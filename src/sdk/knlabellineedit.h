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

#ifndef KNLABELLINEEDIT_H
#define KNLABELLINEEDIT_H

#include <QLineEdit>

class QTimeLine;
/*!
 * \brief The KNLabelLineEdit class provide an enhanced line edit widget. It
 * could provide a rounded rectangle border and
 */
class KNLabelLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNLabelLineEdit widget with parent widget.
     * \param parent The parent widget pointer.
     */
    explicit KNLabelLineEdit(QWidget *parent = 0);

    /*!
     * \brief Get the label icon of the line edit.
     * \return The left icon of the line edit. If you never set the icon before,
     * it will stay null QPixmap.
     */
    QPixmap labelIcon() const;

    /*!
     * \brief Get the focus source widget. The focus source widget pointer will
     * point one widget. When you pressed ESC, it will set focus to the widget
     * pointer.
     * \return The focus source widget pointer. If you never set it before, it
     * will be nullptr.
     */
    QWidget *focusSource() const;

public slots:
    /*!
     * \brief Set the source focus widget.
     * \param focusSource The widget pointer.
     */
    void setFocusSource(QWidget *focusSource);

    /*!
     * \brief Set the label icon.
     * \param labelIcon The icon pixmap.
     */
    void setLabelIcon(const QPixmap &labelIcon);

    /*!
     * \brief Use this slot to update the object name. Do NOT use
     * setObjectName().
     * \param itemName The new name of the line edit.
     */
    void updateObjectName(const QString &itemName);

    /*!
     * \brief Set the minimum lightness of the background. When the widget
     * doesn't get focus. The background lightness will be this one.\n
     * Notice: This won't changed any color. Set this before you really show the
     * widget. Never called this function when you have show the line edit.
     * \param minimumLightness The lightness level. It should be 0 to 255.
     */
    void setMinimumLightness(int minimumLightness);

    /*!
     * \brief Set the maximum lightness of the background. When the widget
     * gets focus. The background lightness will be this one.\n
     * Notice: This won't changed any color. Set this before you really show the
     * widget. Never called this function when you have show the line edit.
     * \param mediumLightness The lightness level. It should be 0 to 255.
     */
    void setMediumLightness(int mediumLightness);

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
    void onThemeChanged();
    void onActionMouseInOut(const int &frame);
    void onFocusInOut(const int &frame);

private:
    inline QTimeLine *generateTimeLine();
    inline void startAnime(QTimeLine *timeLine, const int &end);
    QPixmap m_labelIcon;
    QColor m_baseColor;
    QTimeLine *m_mouseInOut, *m_focusInOut;
    QWidget *m_focusSource;
    int m_minimumLightness, m_mediumLightness;
};

#endif // KNLABELLINEEDIT_H
