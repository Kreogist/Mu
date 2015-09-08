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

#ifndef KNPREFERENCEITEM_H
#define KNPREFERENCEITEM_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNPreferenceItem class is a specific button for preference plugin.
 * It will display a title and a icon of a preference tab.
 */
class KNPreferenceItem : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNPreferenceItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceItem(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);
    void onActionToggled(bool checked);

private:
    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void startAnime(QTimeLine *timeLine);
    QLinearGradient m_shadowGradient;
    QTimeLine *m_mouseIn, *m_mouseOut;
    int m_progress, m_textX;
    qreal m_backgroundOpacity;
};

#endif // KNPREFERENCEITEM_H
