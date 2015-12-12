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
#ifndef KNMUSICRATINGEDITOR_H
#define KNMUSICRATINGEDITOR_H

#include <QWidget>

/*!
 * \brief The KNMusicRatingEditor class provide a star rating editor. It will
 * show up 5 stars and provide a 5 level star edit. It provide the starNum()
 * function get the cureent number.
 */
class KNMusicRatingEditor : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicRatingEditor widget.
     * \param parent The parent widget.
     */
    explicit KNMusicRatingEditor(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QWidget::sizeHint.
     */
    QSize sizeHint() const Q_DECL_OVERRIDE
    {
        return QSize(m_editorWidth, m_starSizeHint);
    }

    /*!
     * \brief Get the single star size parameter.
     * \return The star size.
     */
    int starSizeHint() const;

    /*!
     * \brief Set the star size.
     * \param starSizeHint The single star size height or width.
     */
    void setStarSizeHint(int starSizeHint);

    /*!
     * \brief Get the current star number. It's actually the value of the star
     * editor.
     * \return The star editor value.
     */
    int starNum() const;

    /*!
     * \brief Set the star number value of the current editor.
     * \param starNum The star number.
     */
    void setStarNum(int starNum);

signals:
    /*!
     * \brief When user click the widget, this signal will be emitted. Which
     * means the value is changed.
     */
    void editingFinished();

public slots:
    /*!
     * \brief Set whether the widget will detect the mouse moving and change the
     *  value with the mouse move. This property will be true as default.
     * \param detectOnMove Mouse detecting switch.
     */
    void setDetectOnMove(bool detectOnMove);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent.
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemtned from QWidget::mousePressEvent.
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemtned from QWidget::mouseMoveEvent.
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::mouseReleaseEvent.
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    inline int starAtPosition(int x);
    QPixmap m_star, m_unstar, m_scaleStar, m_scaleUnstar;
    int m_starSizeHint, m_editorWidth, m_starNum, m_halfStarSizeHint;
    bool m_detectOnMove;
};

#endif // KNMUSICRATINGEDITOR_H
