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

#ifndef KNMAINWINDOWSTATUSBAR_H
#define KNMAINWINDOWSTATUSBAR_H

#include <QWidget>

class QBoxLayout;
class QTimeLine;
/*!
 * \brief The KNMainWindowStatusBar class provides the status button bar.
 * It will show the button at the top right corner. All the size of the button
 * will be fixed to 16px.
 */
class KNMainWindowStatusBar : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMainWindowStatusBar widget.
     * \param parent The parent widget.
     */
    explicit KNMainWindowStatusBar(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Add widget to the status bar.
     * \param widget The widget pointer.
     */
    void addWidget(QWidget *widget);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onPaletteChanged();
    void changeBackgroundColor(int frame);

private:
    inline void updateGradient();
    inline QTimeLine *generateTimeline();
    inline void startAnime(int endFrame);
    QColor m_backgroundColor;
    QLinearGradient m_background;
    QBoxLayout *m_mainLayout;
    QTimeLine *m_mouseInOut;
    int m_opacity, m_buttonCount;
};

#endif // KNMAINWINDOWSTATUSBAR_H
