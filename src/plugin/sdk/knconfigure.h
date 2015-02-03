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
#ifndef KNCONFIGURE_H
#define KNCONFIGURE_H

#include <QVariant>
#include <QJsonObject>
#include <QLinkedList>

#include <QObject>

namespace KNConfigureGlobal
{
enum Types
{
    Font
};
}

using namespace KNConfigureGlobal;

class KNConfigure : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath)
    Q_PROPERTY(QString caption READ caption WRITE setCaption)
public:
    explicit KNConfigure(QObject *parent = 0);
    ~KNConfigure();
    QJsonObject configureObject();
    void setConfigureObject(const QJsonObject &configureObject);
    void addSubConfigure(KNConfigure *configure=nullptr);

    //Properties.
    QString filePath() const;
    void setFilePath(const QString &filePath);
    QString caption() const;
    void setCaption(const QString &caption);

    //Actions.
    void loadConfigure();
    void saveConfigure();
    QVariant getData(const QString &key);
    void setData(const QString &key, const QVariant &value);

signals:

public slots:

private:
    inline void updateSubConfigure();
    static void buildObjectTypeList();
    static QHash<QString, int> m_objectType;
    QJsonObject m_dataObject;
    QLinkedList<KNConfigure *> m_subConfigures;
    QString m_filePath;
    QString m_caption;
};

#endif // KNCONFIGURE_H
