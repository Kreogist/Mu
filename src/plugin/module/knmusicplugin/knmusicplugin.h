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
#ifndef KNMUSICPLUGIN_H
#define KNMUSICPLUGIN_H

#include "knabstractmusicplugin.h"

class QBoxLayout;
class KNCategoryTabWidget;
class KNMusicBackend;
class KNMusicGlobal;
class KNMusicParser;
class KNMusicHeaderPlayerBase;
class KNMusicNowPlayingBase;
class KNMusicPlugin : public KNAbstractMusicPlugin
{
    Q_OBJECT
public:
    explicit KNMusicPlugin(QObject *parent = 0);
    ~KNMusicPlugin();
    QString caption();
    QPixmap icon();
    QWidget *centralWidget();
    QWidget *headerWidget();

    void loadBackend(KNMusicBackend *plugin);
    void loadHeaderPlayer(KNMusicHeaderPlayerBase *plugin);
    void loadNowPlaying(KNMusicNowPlayingBase *plugin);

signals:

public slots:
    void retranslate();

protected slots:
    void addMusicCategory(const QPixmap &icon,
                          const QString &caption,
                          QWidget *widget);
    virtual void addLeftHeaderWidget(QWidget *widget,
                                     int stretch=0,
                                     Qt::Alignment alignment=0);
    virtual void addRightHeaderWidget(QWidget *widget,
                                      int stretch=0,
                                      Qt::Alignment alignment=0);

private:
    void initialInfrastructure();
    void initialParser();
    void startThreads();
    QLinkedList<QObject *> m_pluginList;
    QString m_caption;
    KNCategoryTabWidget *m_centralWidget=nullptr;
    QWidget *m_headerWidget=nullptr;
    QBoxLayout *m_headerLeftLayout, *m_headerRightLayout;
    QThread m_parserThread, m_backendThread;
    KNMusicGlobal *m_musicGlobal;

    KNMusicBackend *m_backend=nullptr;
    KNMusicParser *m_parser=nullptr;
    KNMusicHeaderPlayerBase *m_headerPlayer=nullptr;
};

#endif // KNMUSICPLUGIN_H
