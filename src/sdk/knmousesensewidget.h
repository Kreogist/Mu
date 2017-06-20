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

#ifndef KNMOUSESENSEWIDGET_H
#define KNMOUSESENSEWIDGET_H

#include <QWidget>

class QTimeLine;
/*!
 * \brief The KNMouseSenseWidget class\n
 * IMPORTANT: You have to set a object name for every mouse sense widget's
 * implement widget using updateObjectName(QString) function. DO NOT use
 * setObjectName() function.
 */
class KNMouseSenseWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMouseSenseWidget widget.
     * \param parent The parent widget.
     */
    explicit KNMouseSenseWidget(QWidget *parent = 0);

    /*!
     * \brief Update the object name of the widget. And update the palette as
     * well.
     * \param name The object name of the widget.
     */
    void updateObjectName(const QString &name);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMainWindowHeaderBase::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplemented from KNMainWindowHeaderBase::leaveEvent().
     */
    void leaveEvent(QEvent *event);

protected slots:
    /*!
     * \brief When the palette changed, this slot will be called.
     */
    virtual void onPaletteChanged();

    /*!
     * \brief When the background color need to be changed according to the.
     * \param frame
     */
    virtual void changeBackgroundColor(int frame);

private:
    inline QTimeLine *generateTimeline();
    inline void startAnime(int endFrame);
    QTimeLine *m_mouseInOut;
    int m_rangeStart, m_rangeEnd;
};

#endif // KNMOUSESENSEWIDGET_H
