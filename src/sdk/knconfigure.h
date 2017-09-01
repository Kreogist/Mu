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

#ifndef KNCONFIGURE_H
#define KNCONFIGURE_H

#include <QJsonObject>
#include <QHash>
#include <QVariant>

#include <QObject>

/*!
 * \brief The KNConfigure class is a Json based configure read and write class.
 * It will hold a Json object to load, save and modify settings.\n
 * The Json object can be set mannually, or from a value from another
 * KNConfigure class.
 */
class KNConfigure : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNConfigure with a parent object.
     * \param parent The parent object of the KNConfigure.
     */
    explicit KNConfigure(QObject *parent = 0);

    /*!
     * \brief Construct a KNConfigure with a given json object. When you get a
     * Json object from configure file, you can construct a KNConfigure in this
     * way to hold the Json object.
     * \param data The Json object.
     * \param parent The parent object of the KNConfigure.
     */
    explicit KNConfigure(const QJsonObject &data,
                         QObject *parent = 0);

    /*!
     * \brief Get the data Json object. This value will be updated by recursive
     * calling the sub configure of the current configure when this function is
     * called.
     * \return The data Json object.
     */
    QJsonObject dataObject();

    /*!
     * \brief Get a configure class from the current configure.
     * \param key The key of the class.
     * \return The configure class.
     */
    KNConfigure *getConfigure(const QString &key);

    /*!
     * \brief Get a data from the configure via the key. If the configure cannot
     * find the key, it will return the default value. The default value of the
     * default value is QVariant().
     * \param key The data key.
     * \param defaultValue The default value of the specific data.
     * \return The data in QVariant.
     */
    QVariant data(const QString &key,
                  const QVariant &defaultValue=QVariant());

    /*!
     * \brief Check whether the current configure contains one data.
     * \param key The data key.
     * \return If the configure has the data key, return true.
     */
    bool contains(const QString &key) const;

    /*!
     * \brief Get a data which could be quite deep level in the configure. If it
     * is not in this level, it will call the deeper level in the configure. If
     * it is right in the level, it will find and return the value the same as
     * calling the data() function.
     * \param path Configure setting path.
     * \param defaultValue The default value of the specific data.
     * \return The data in QVariant.
     */
    QVariant getPathData(const QStringList &path,
                         const QVariant &defaultValue=QVariant());

signals:
    /*!
     * \brief When any value of the configure changed, this signal will be
     * emitted.
     */
    void valueChanged();

public slots:
    /*!
     * \brief Save a data into a specific key.
     * \param key The key of the data.
     * \param value The data.
     */
    void setData(const QString &key, const QVariant &value);

    /*!
     * \brief Remove a data set from the configure.
     * \param key The key of the data.
     */
    void remove(const QString &key);

    /*!
     * \brief Set the data in a specific path.
     * \param path The path of the data.
     * \param value The data.
     */
    void setPathData(const QStringList &path, const QVariant &value);

    /*!
     * \brief Set the data object from a configure file.
     * \param dataObject The data object.
     */
    void setDataObject(const QJsonObject &dataObject);

private:
    enum ObjectValueType
    {
        Font,
        Shortcut,
        CustomObject
    };
    inline void buildTypeList();

    static QHash<QString, int> m_typeList;

    QJsonObject m_dataObject;
    QString m_caption;
    QHash<QString, KNConfigure *> m_subObjectTable;
};

#endif // KNCONFIGURE_H
