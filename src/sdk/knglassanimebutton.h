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

#ifndef KNGLASSANIMEBUTTON_H
#define KNGLASSANIMEBUTTON_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNGlassAnimeButton class
 */
class KNGlassAnimeButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNGlassAnimeButton widget.
     * \param parent The parent widget.
     */
    explicit KNGlassAnimeButton(QWidget *parent = 0);

    void setIcon(const QIcon &icon);

    /*!
     * \brief Whether will the button show the left border line of the button.
     * \return If the button line will be shown, return true. It will be false
     * as default.
     */
    bool showLeftLine() const;

    /*!
     * \brief Set whether the left border line will be shown.
     * \param If you want to show the left border line, set this to be true.
     */
    void setShowLeftLine(bool showLeftLine);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

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

    /*!
     * \brief Reimplemented from QAbstractButton::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionMouseAnime(const int &frame);

private:
    inline void startAnimation(const int &endFrame);
    inline void updateScaledIcon();
    QLinearGradient m_lineGradient, m_backGradient;
    QPixmap m_scaledIcon;
    QTimeLine *m_mouseAnime;
    int m_iconSize, m_currentHighlight, m_iconX, m_iconY;
    bool m_leftLine;
};

#endif // KNGLASSANIMEBUTTON_H
