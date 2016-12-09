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

#ifndef KNSCROLLAREA_H
#define KNSCROLLAREA_H

#include <QScrollArea>

class QTimeLine;
class QScrollBar;
/*!
 * \brief The KNScrollArea class is a customized scroll area widget based on the
 * Qt's scroll area widget. This widget change the default scroll bar to the
 * mouse sense scroll bar. The scroll bar will automatically appear when it
 * needs, and automatically disappear when the widget is hidden.
 */
class KNScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNScrollArea widget.
     * \param parent The parent widget.
     */
    explicit KNScrollArea(QWidget *parent = 0);

    /*!
     * \brief Update the object name set up the new palette from theme manager.
     * \param name The new object name.
     */
    void updateObjectName(const QString &name);


    /*!
     * \brief Get the customized horizontal scroll bar.
     * \return The horizontal scroll bar widget.
     */
    QScrollBar *hScrollBar() const;

    /*!
     * \brief Get the vertical scroll bar widget pointer.
     * \return The veritical scroll bar widget.
     */
    QScrollBar *vScrollBar() const;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QScrollArea::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QScrollArea::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QScrollArea::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QScrollArea::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected slots:
    /*!
     * \brief This slot is provide to update the palette when the scroll area is
     * being constructed. Or else the UI will be very ugly.
     */
    void onActionThemeUpdate();

private slots:
    void onActionMouseInOut(int frame);

private:
    inline void updateVerticalScrollBarGeometry();
    inline void startAnime(int endFrame);
    QScrollBar *m_hScrollBar, *m_vScrollBar;
    QTimeLine *m_mouseAnime;
    int m_currentFrame;
};

#endif // KNSCROLLAREA_H
