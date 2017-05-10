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
#ifndef KNACCOUNTBASE_H
#define KNACCOUNTBASE_H

#include "knrestapibase.h"

class KNAccountDetails;
/*!
 * \brief The KNAccountBase class provides the basic operations of the cloud
 * service. The basic operations contain the following:
 *  * Upload a file.
 *  * Remove a file.
 *  * Download file to a byte array.
 *  * Create row in a table.
 *  * Update row in a table.
 *  * Get the row content from a table.
 */
class KNAccountBase : public KNRestApiBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountBase object.
     * \param parent The parent object.
     */
    explicit KNAccountBase(QObject *parent = 0);
    ~KNAccountBase();

signals:

public slots:

protected:
    /*!
     * \brief Create a row on the cloud.
     * \param tableClassName The table name at the cloud.
     * \param parameter The row content.
     * \param responseData The response data from the cloud server.
     * \param errorCode The error code when there is an error occur.
     * \return If the row could created successfully, return true.
     */
    bool createRow(const QString &tableClassName,
                   const QByteArray &parameter,
                   QByteArray &responseData,
                   int &errorCode);

    /*!
     * \brief Update a row on the cloud.
     * \param tableClassName The table name at the cloud.
     * \param parameter The row content.
     * \param responseData The response data from the cloud server.
     * \param errorCode The error code when there is an error occur.
     * \param sessionToken The session token text. If the token is provided, it
     * will be set as a part of the row.
     * \return If the row could updated successfully, return true.
     */
    bool updateRow(const QString &tableClassName,
                   const QByteArray &parameter,
                   QByteArray &responseData,
                   int &errorCode,
                   const QString &sessionToken=QString());

    /*!
     * \brief Fetch the row content from the cloud.
     * \param tableClassName The table name at the cloud.
     * \param responseData The response data from the cloud server.
     * \param errorCode The error code when there is an error occur.
     * \param queryMap If the fetch need some query, fill this area.
     * \return If the row content fetched successfully, return true.
     */
    bool fetchRow(const QString &tableClassName,
                  QByteArray &responseData,
                  int &errorCode,
                  const QMap<QString, QString> &queryMap=
            QMap<QString, QString>());

    /*!
     * \brief Upload a file to the cloud.
     * \param url The file upload path.
     * \param fileContent The content of the file.
     * \param responseData The response data from the cloud.
     * \param errorCode The error code when there is an error occurs.
     * \param fileType The file type.
     * \return If the file could upload to the server, return true.
     */
    bool uploadFile(const QString &url,
                    const QByteArray &fileContent,
                    QByteArray &responseData,
                    int &errorCode,
                    const QString &fileType=QString());

    /*!
     * \brief Download a file from the cloud.
     * \param url The file url on the cloud.
     * \param responseData The response data from the cloud. It should be the
     * file binary content.
     * \param errorCode The error code when there is an error occurs.
     * \return If the file could download to the server, return true.
     */
    bool downloadFile(const QString &url,
                      QByteArray &responseData,
                      int &errorCode);

    /*!
     * \brief Remove a file from the cloud.
     * \param url The file url on the cloud.
     * \param errorCode The error code when there is an error occurs.
     * \return If the file could download to the server, return true.
     */
    bool removeFile(const QString &url,
                    int &errorCode);

private:
    inline QNetworkRequest generateCloudRequest(const QString &url);
    KNAccountDetails *m_accountDetails;
};

#endif // KNACCOUNTBASE_H
