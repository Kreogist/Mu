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

#ifndef KNPREFERENCEPANELCONTAINER_H
#define KNPREFERENCEPANELCONTAINER_H

#include "knpreferenceutil.h"

#include <QWidget>

using namespace PreferenceUtil;

class QScrollBar;
class KNSideShadowWidget;
class KNPreferencePanel;
/*!
 * \brief The KNPreferencePanelContainer class provides a container which is
 * the container for all the panel loading widget. It will create the shadow and
 * resize itself to fit all the panel widgets.
 */
class KNPreferencePanelContainer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelContainer widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelContainer(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the panel blocks to the panel list. The container will
     * generate a panel list widget for the items.
     * \param blocks The block list.
     */
    void setPanelBlocks(const QList<PreferencePanelBlock> &blocks);

    /*!
     * \brief When the advanced option is toggled, this slot should be called to
     * show to hide the advanced options.
     * \param showAdvanced To show the advanced items, set to true.
     */
    void setAdvancedShown(bool showAdvanced);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    inline void updateContainerSize();
    inline void updatePanelSize(KNPreferencePanel *panel,
                                KNSideShadowWidget *shadow,
                                QScrollBar *scrollBar);
    QList<KNPreferencePanel *> m_panelList;
    QList<QScrollBar *> m_scrollList;
    QList<KNSideShadowWidget *> m_shadowList;
    QWidget *m_panelContainer;
};

#endif // KNPREFERENCEPANELCONTAINER_H
