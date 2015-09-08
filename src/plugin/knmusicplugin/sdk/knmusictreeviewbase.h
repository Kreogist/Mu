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
class KNMusicModel;
class KNMusicProxyModel;
class KNMusicTab;
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
    explicit KNMusicTreeViewBase(QWidget *parent = 0,
                                 KNMusicTab *tab = 0);
    ~KNMusicTreeViewBase();

    /*!
     * \brief Get this tree view will enable animate features or not.
     * \return If the animation features has been enabled, it will be true.
     */
    bool animate() const;

    /*!
     * \brief Get the proxy of the model.
     * \return The proxy music model pointer.
     */
    KNMusicProxyModel *proxyModel();

    /*!
     * \brief Get the music model of the model. It will simply return the
     * managing music model of the proxy model.
     * \return The music model pointer.
     */
    KNMusicModel *musicModel();

    /*!
     * \brief Set the music model. DO NOT use setModel() function. Use this
     * instead.
     * \param musicModel The music model pointer.
     */
    void setMusicModel(KNMusicModel *musicModel);

    /*!
     * \brief Select a song by providing the song row of the source model.
     * \param sourceRow The source row of the song. If the row is invalid or
     * there's no valid model, it will do nothing.
     */
    void selectSourceSong(const int &sourceRow);

    /*!
     * \brief This is a override function.\n
     * Select a song by providing one index of the the song row of the source
     * model.
     * \param index One of the index of the source row.
     */
    inline void selectSourceSong(const QModelIndex &index)
    {
        selectSourceSong(index.row());
    }

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
     * \brief Reimplemented from QTreeView::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QTreeView::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QTreeView::drawRow().
     */
    void drawRow(QPainter *painter,
                 const QStyleOptionViewItem &options,
                 const QModelIndex &index) const;

    /*!
     * \brief Reimplemented from QTreeView::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented from QTreeView::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented from QTreeView::keyReleaseEvent().
     */
    void keyReleaseEvent(QKeyEvent *event);

    /*!
     * \brief Reset the header column state to a default state. Including the
     * width and visible.
     */
    virtual void resetHeaderState();

    /*!
     * \brief Move one column to the first column.
     * \param logicalIndex The logical index of the header column.
     */
    void moveToFirst(const int &logicalIndex);

    /*!
     * \brief Reimplemented from QTreeView::event().
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QTreeView::wheelEvent().
     */
    void wheelEvent(QWheelEvent *event);

protected slots:
    /*!
     * \brief This slot is provide to update the palette when the tree view is
     * being constructed. Or else the UI will be very ugly.
     */
    void onActionThemeUpdate();

private slots:
    void onActionMouseInOut(const int &frame);
    void onActionActivate(const QModelIndex &index);
    void playCurrent();
    void removeCurrent();
    void renameCurrent();

private:
    inline void startAnime(const int &endFrame);
    inline void playIndex(const QModelIndex &index);
    void showSoloMenu(const QPoint &position);
    void showMultiMenu(const QPoint &position);
    bool showDetailTooltip(const QPoint &indexPosition);

    KNMusicTab *m_musicTab;
    QTimeLine *m_mouseAnime;
    bool m_animate;
    KNMusicProxyModel *m_proxyModel;

    bool m_initialLoad, m_pressed;
};

#endif // KNMUSICTREEVIEWBASE_H
