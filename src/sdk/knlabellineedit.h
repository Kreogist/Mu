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
class KNLabelLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit KNLabelLineEdit(QWidget *parent = 0);

    QPixmap labelIcon() const;

    void setLabelIcon(const QPixmap &labelIcon);

    QWidget *focusSource() const;

    void setFocusSource(QWidget *focusSource);


signals:

public slots:
    /*!
     * \brief Use this slot to update the object name. Do NOT use
     * setObjectName().
     * \param itemName The new name of the line edit.
     */
    void updateObjectName(const QString &itemName);

    void setMinimumLightness(int minimumLightness);

    /*!
     * \brief setMediumLightness.\n
     * Notice: This won't changed any color. Set this before you really show the
     * widget. Never called this function when you have show the button.
     * \param mediumLightness
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
    void onActionThemeChanged();
    void onActionMouseInOut(const int &frame);
    void onActionFocusInOut(const int &frame);

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
