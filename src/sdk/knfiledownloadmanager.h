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

#include "knconnectionhandler.h"

#include <QObject>

/*!
 * \brief The KNFileDownloadManager class provides a file download object which
 * could download a file using standard http GET operation.\n
 * This is a highly QNetworkAccessManager packaging. Supports for error handling
 * via signal.
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
    /*!
     * \brief This signal is emitted to indicate the progress of the download
     * part of this network request, if there's any. If there's no download
     * associated with this request, this signal will be emitted once with 0 as
     * the value of both bytesReceived and bytesTotal.
     * \param bytesReceived The count of the bytes downloaded.
     * \param bytesTotal The total bytes of the content.
     */
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    /*!
     * \brief When the file downloaded complete, this signal will be emitted.
     */
    void finished();

public slots:
    /*!
     * \brief Download one file from its target url.
     * \param url The file url.
     * \param targetFolder The file saving directory path.
     * \param rename The new name of the file, if you don't want to change the
     * file name, set it to QString().
     */
    void downloadFile(const QString &url, const QString &targetFolder,
                      const QString &rename);

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
    KNConnectionHandler m_replyHandler;
    QString m_savePath, m_rename;
    QNetworkReply *m_fileReply;
};

#endif // KNFILEDOWNLOADMANAGER_H
