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

#ifndef KNMUSICSEARCH_H
#define KNMUSICSEARCH_H

#include "knmusicsearchbase.h"

class QAction;
class KNSearchBox;
class KNMusicSearchSyntaxEngine;
/*!
 * \brief The KNMusicSearch class is a default realize of the music search
 * module. This is a simple example of the search module.
 */
class KNMusicSearch : public KNMusicSearchBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicSearch class.
     * \param parent The parent widget.
     */
    explicit KNMusicSearch(QObject *parent = 0);

    ~KNMusicSearch();

    /*!
     * \brief Reimplement from KNMusicSearchBase::widget().
     */
    QWidget *widget() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from KNMusicSearchBase::rules().
     */
    QList<KNMusicSearchBlock> rules() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from KNMusicSearchBase::activateAction().
     */
    QAction *activateAction() Q_DECL_OVERRIDE;

signals:

public slots:

private slots:
    void retranslate();
    void onActionSearch(const QString &text);

private:
    KNSearchBox *m_searchBox;
    KNMusicSearchSyntaxEngine *m_engine;
    QAction *m_activateAction;

    QList<KNMusicSearchBlock> m_searchBlockList;
};

#endif // KNMUSICSEARCH_H
