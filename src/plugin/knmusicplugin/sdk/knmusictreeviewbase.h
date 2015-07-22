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

#ifndef KNMUSICTREEVIEWBASE_H
#define KNMUSICTREEVIEWBASE_H

#include <QTreeView>

class QTimeLine;
/*!
 * \brief The KNMusicTreeViewBase class is the basic tree view of all the music
 * tree view. It provides mouse sense alternative row color. It can
 * automatically link to the solo and multi menu to process the music row.\n
 * Remember this is only a base class, it means you have to reimplement it. Set
 * the object name and called onActionThemeUpdate() slot.
 */
class KNMusicTreeViewBase : public QTreeView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTreeViewBase widget.
     * \param parent The parent widget.
     */
    explicit KNMusicTreeViewBase(QWidget *parent = 0);

    /*!
     * \brief Get this tree view will enable animate features or not.
     * \return If the animation features has been enabled, it will be true.
     */
    bool animate() const;

signals:

public slots:
    /*!
     * \brief Set whether this tree view widget should use animation features.
     * \param animate To enabled animation features, set this property to be
     * true.
     */
    void setAnimate(bool animate);

protected:
    /*!
     * \brief Reimplement from QTreeView::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplement from QTreeView::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplement from QTreeView::drawRow().
     */
    void drawRow(QPainter *painter,
                 const QStyleOptionViewItem &options,
                 const QModelIndex &index) const;

protected slots:
    /*!
     * \brief This slot is provide to update the palette when the tree view is
     * being constructed. Or else the UI will be very ugly.
     */
    void onActionThemeUpdate();

private slots:
    void onActionMouseInOut(const int &frame);

private:
    inline QTimeLine *generateTimeLine(const int &endFrame);
    inline void startAnime(QTimeLine *timeLine);
    QTimeLine *m_mouseIn, *m_mouseOut;
    bool m_animate;
};

#endif // KNMUSICTREEVIEWBASE_H
