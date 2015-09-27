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
/*
 * Code name:
 * 2.1 - Trueno.
 * Previous version code name:
 * 2.0 - Cheater.
 * 1.0 - Beast.
 */
#ifndef KNJSONDATABASE_H
#define KNJSONDATABASE_H

#include <QFileInfo>
#include <QScopedPointer>
#include <QFile>
#include <QJsonArray>

#include <QObject>

class QFile;
/*!
 * \brief The KNJsonDatabase class provides a simple, stable and swift Json
 * based database. It's nearly the same as the XML database used by iTunes. This
 * database using a QJsonValue as a row. But it won't check the columns and any
 * other things. So you can design your own format.\n
 * It only have one table, so you have to construct more KNJsonDatabase if you
 * need more. This can be a kind of NoSQL.\n
 * This database will not write your data at once, it will operate for
 * MaxOperateCount operates in memory. There is a counter to count the following
 * operation: append, replace, insert, removeAt. When the counter reaches
 * MaxOperateCount, then it will write the database to file immediately, and
 * reset to counter to 0.
 */
class KNJsonDatabase : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNJsonDatabase object.
     * \param parent The parent object.
     */
    explicit KNJsonDatabase(QObject *parent = 0);

    /*!
     * \brief Link with a json database file. If there's already a link
     * established, it will write the previous data to the database first, and
     * then link to another one.\n
     * It will reset the counter to 0. And clear up the previous data field.\n
     * If the json database file doesn't exist, it will make the path and tried
     * create a new one.
     * \param jsonFile The json database file path.
     * \return If we can write the old data to the previous database when it's
     * not null, it will be true. If there's no previous database, it should
     * always be true.
     */
    bool link(const QString &jsonFile);

    /*!
     * \brief Try to read the data in the json database file. Before you called
     * this function, you should called the link() function first.\n
     * It won't read the higher version database.
     * \return If it can parse the database and find out that it's a 2.x
     * database, it will return true.
     */
    bool read();

    /*!
     * \brief Write the data field data to the database file. It will reset the
     * counter to 0 if the write operation is successful.
     * \return If the counter is 0, it will be true. Or else, return true when
     * write the data to database file is successful.
     */
    bool write();

    /*!
     * \brief The begin iterator of the data field. This is a quick access
     * function.
     * \return The begin() of the json array data field.
     */
    QJsonArray::iterator begin()
    {
        return m_dataField.begin();
    }

    /*!
     * \brief The end iterator of the data field. This is a quick access
     * function.
     * \return The end() of the json array data field.
     */
    QJsonArray::iterator end()
    {
        return m_dataField.end();
    }

    /*!
     * \brief Append a json value to the end of the data field. It will increase
     * the operation counter.
     * \param value The json value.
     */
    void append(const QJsonValue &value);

    /*!
     * \brief Replace the value at index i with the new json value. It will
     * increase the operation counter.
     * \param i The index of the value.
     * \param value The json value.
     */
    void replace(int i, const QJsonValue &value);

    /*!
     * \brief Insert value at index position i in the data field array. It i is
     * 0, the value is prepended to the data field. If i is size(), the value is
     * appended to the end of the data field. It will increase the operation
     * counter.
     * \param i The position.
     * \param value The json value.
     */
    void insert(int i, const QJsonValue &value);

    /*!
     * \brief Remove the json value at the position i. It will increase the
     * operation counter.
     * \param i The position.
     */
    void removeAt(int i);

    /*!
     * \brief Get the specific value at the position i.
     * \param i The position of the value.
     * \return The json value at i of the data field.
     */
    QJsonValue at(int i);

    /*!
     * \brief Get the size of the items which was data field.
     * \return The number of the item which json array holds.
     */
    int size() const;

signals:

public slots:
private:
    inline void checkDatabaseDir();
    inline bool writeToFile();
    inline void count();
    QFileInfo m_fileInfo;
    QScopedPointer<QFile> m_file;
    QJsonArray m_dataField;
    quint64 m_operateCount;
};

#endif // KNJSONDATABASE_H
