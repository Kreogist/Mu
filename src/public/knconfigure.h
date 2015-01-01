/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNCONFIGURE_H
#define KNCONFIGURE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QHash>

#include <QObject>

namespace KNConfigureTypes
{
enum Types
{
    Font
};
}

using namespace KNConfigureTypes;

class KNConfigure : public QObject
{
    Q_OBJECT
public:
    static KNConfigure *instance();
    void loadConfigure();
    void saveConfigure();
    void setSystemData(const QString &key,
                       const QJsonValue &value);
    void setCustomData(const QString &module,
                       const QString &key,
                       const QJsonValue &value);
    QVariant systemData(const QString &key);
    QVariant customData(const QString &module,
                        const QString &key);
    QString configurePath() const;
    void setConfigurePath(const QString &configurePath);

signals:

public slots:

private:
    static KNConfigure *m_instance;
    explicit KNConfigure(QObject *parent = 0);

    inline QVariant parseJsonValue(const QJsonValue &value);

    inline void generateConfigureFolder();
    inline void loadConfigureFromFile(const QString &filePath,
                                      QJsonObject &configureObject);
    inline void saveConfigureToFile(const QString &filePath,
                                    const QJsonObject &configureObject);

    QString m_configurePath,
            m_systemConfigurePath,
            m_userConfigurePath;
    QJsonObject m_systemConfigure, m_customConfigure;
    QHash<QString, int> m_objectType;
};

#endif // KNCONFIGURE_H
