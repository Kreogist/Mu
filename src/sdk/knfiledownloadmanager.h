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

#ifndef KNFILEDOWNLOADMANAGER_H
#define KNFILEDOWNLOADMANAGER_H

#include <QUrl>
#include <QNetworkAccessManager>
#include <QScopedPointer>

#include <QObject>

/*!
 * \brief The KNFileDownloadManager class provides a pipeline file download
 * object which could download a file in an async way.
 */
class KNFileDownloadManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNFileDownloadManager object.
     * \param parent The parent object.
     */
    explicit KNFileDownloadManager(QObject *parent = 0);

signals:

public slots:
    /*!
     * \brief Download one file from its target url.
     * \param url The file url.
     * \param targetFolder The file saving directory path.
     */
    void downloadFile(const QString &url, const QString &targetFolder);

    /*!
     * \brief Reset the downloader.
     */
    void reset();

private slots:
    void onDownloaderFinished(QNetworkReply *reply);
    void replyDownloadProgress(const qint64 &bytesReceived,
                               const qint64 &bytesTotal);

private:
    QScopedPointer<QNetworkAccessManager> m_downloader;
    QList<QNetworkReply *> m_fileList;
    QStringList m_savePathList;
};

#endif // KNFILEDOWNLOADMANAGER_H
