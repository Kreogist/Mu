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

#ifndef KNCATEGORYTAB_H
#define KNCATEGORYTAB_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNCategoryTab class is the tab of the category switcher in a
 * category plugin. It is design to show the sub category of the application
 * category with fully animated effects.\n
 * The tab is a kind of button.
 */
class KNCategoryTab : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNCategoryTab widget.
     * \param parent The parent widget.
     */
    explicit KNCategoryTab(QWidget *parent = 0);

    /*!
     * \brief Get the category tab height. This is a fixed value for UI tweak.
     * \return The category height.
     */
    static int tabHeight();

    /*!
     * \brief Set the text of category. Reimplemented from
     * QAbstractButton::setText(). It will update the UI elements when settings
     * a new text.
     * \param text The text of the tab.
     */
    void setText(const QString &text);

signals:

public slots:

protected:
    /*!
     * \brief Reimplment from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplment from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplment from QAbstractButton::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplment from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplment from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onMouseInOut(int frame);
    void onActionMouseUpDown(int frame);
    void onActionToggled(bool checked);

private:
    inline QTimeLine *generateTimeLine();
    inline void startAnime(QTimeLine *timeLine, int endFrame);

    QTimeLine *m_mouseInOut, *m_mouseUpDown;

    int m_textWidth, m_textFullWidth, m_animeParameter;
    QLinearGradient m_highLight;
    qreal m_backgroundAlpha=0.0;
};

#endif // KNCATEGORYTAB_H
