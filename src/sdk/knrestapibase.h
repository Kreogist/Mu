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
#ifndef KNRESTAPIBASE_H
#define KNRESTAPIBASE_H

#include <QNetworkRequest>

#include "knconnectionhandler.h"

#include <QObject>

class QNetworkAccessManager;
/*!
 * \brief The KNRestApiBase class provides the basic functions for all the
 * module which may used REST API or the module need http request. It support
 * post, get and put.
 */
class KNRestApiBase : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNRestApiBase object.
     * \param parent The parent object pointer.
     */
    explicit KNRestApiBase(QObject *parent = 0);
    ~KNRestApiBase();

    /*!
     * \brief Chagne the working thread of the downloader. Do NOT use
     * moveToThread() function, use this function instead.
     * \param thread The working thread of the downloader.
     */
    virtual void setWorkingThread(QThread *thread);

signals:

public slots:

protected:
    /*!
     * \brief Http delete request via given the request. This function works in
     * stucked way.
     * \param request The request of the get request.
     * \return HTTP response code. If we cannot send the post, then it will be
     * -1.
     */
    int deleteResource(const QNetworkRequest &request);

    /*!
     * \brief Http put request via given the request. This function works in
     * stucked way.
     * \param request The request of the get request.
     * \param parameter The parameter which will be used when doing the get
     * request.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     * \return HTTP response code. If we cannot send the post, then it will be
     * -1.
     */
    int put(const QNetworkRequest &request,
            const QByteArray &parameter,
            QByteArray &responseData);

    /*!
     * \brief Http get request via given the request. This function works in
     * stucked way.
     * \param request The requst of the get request.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     * \return HTTP response code. If we cannot send the get, then it will be
     * -1.
     */
    int get(const QNetworkRequest &request,
            QByteArray &responseData);

    /*!
     * \brief This is a override function.\n
     * Http get request via given the url, cookie and referer. This
     * function works in stucked way.\n
     * The parameters provided in this function is used to generate the request.
     * \param url The url of the get target.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     * \param cookie The cookie which will be use on the request header. By
     * default it can leave blank to use the default one provided by Qt.
     * \param referer The referer parameter.
     * \return HTTP response code. If we cannot send the post, then it will be
     * -1.
     */
    inline int get(const QString &url,
                   QByteArray &responseData,
                   const QVariant &cookie=QVariant(),
                   const QString &referer=QString())
    {
        return get(generateRequest(url, cookie, referer), responseData);
    }

    /*!
     * \brief Http post request via given the request. This function works in
     * stucked way.
     * \param request The request of the post request.
     * \param parameter The parameter which will be used when doing the post
     * request.
     * \param responseData The response data byte array which will be used to
     * receive the data from the post request.
     * \return HTTP response code. If we cannot send the post, then it will be
     * -1.
     */
    int post(QNetworkRequest request,
             const QByteArray &parameter,
             QByteArray &responseData);

    /*!
     * \brief This is a override function.\n
     * Http post request via given the request. This function works in stucked
     * way. The url, cookie and referer is used to generate the request header.
     * The 'parameter' is the parameter when doing the post.
     * \param url The url of the post target.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     * \param parameter The parameter which will be used when doing the post
     * request.
     * \param cookie The cookie which will be use on the request header. By
     * default it can leave blank to use the default one provided by Qt.
     * \param referer The referer parameter.
     * \return HTTP response code. If we cannot send the post, then it will be
     * -1.
     */
    inline int post(const QString &url,
                     QByteArray &responseData,
                     const QByteArray &parameter,
                     const QVariant &cookie=QVariant(),
                     const QString &referer=QString())
    {
        return post(generateRequest(url, cookie, referer),
                    parameter,
                    responseData);
    }

    /*!
     * \brief Generate a request object by providing the url, cookie and the
     * referer.
     * \param url The request target url.
     * \param cookie The custom cookie. Default it is null, it will the default
     * cookie provided by Qt.
     * \param referer The custom referer. Default it's null, it will use no
     * referer to this request(Qt default).
     * \return The request object.
     */
    QNetworkRequest generateRequest(const QString &url,
                                    const QVariant &cookie=QVariant(),
                                    const QString &referer=QString());

private:
    QNetworkAccessManager *m_networkManager;
    KNConnectionHandler m_timeoutHandler;
};

#endif // KNRESTAPIBASE_H
