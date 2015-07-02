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

#ifndef KNPREFERENCEHEADERBUTTON_H
#define KNPREFERENCEHEADERBUTTON_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNPreferenceHeaderButton class provides a great header button of
 * the preference sidebar. It can be used to emit close request.
 */
class KNPreferenceHeaderButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferenceHeaderButton widget.
     * \param parent The parent widget of header button.
     */
    explicit KNPreferenceHeaderButton(QWidget *parent = 0);

    /*!
     * \brief Current icon of the button.
     * \return The icon in QPixmap.
     */
    QPixmap icon() const;

    /*!
     * \brief Set the icon of the button.
     * \param icon Current icon pixmap.
     */
    void setIcon(const QPixmap &icon);

signals:

public slots:

protected:
    bool hitButton(const QPoint &pos) const;
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionMouseInOut(const int &frame);

private:
    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void startAnime(QTimeLine *timeLine);
    QPainterPath m_border;
    QPixmap m_icon, m_closeIcon;
    qreal m_closeIconOpacity;
    QPointF m_iconPosition;
    QTimeLine *m_mouseIn, *m_mouseOut;
};

#endif // KNPREFERENCEHEADERBUTTON_H
