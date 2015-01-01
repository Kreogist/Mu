/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMGLOBAL_H
#define KNPREFERENCEITEMGLOBAL_H

#include <QVariant>
#include <QObject>
#include <QHash>
#include <QFont>
#include <QPalette>

namespace KNPreferenceItemList
{
enum KNPreferenceItemType
{
    Switcher,
    Number,
    LineEdit,
    PathEdit,
    Font
};
struct KNPreferenceTitleInfo
{
    bool advanced=false;
    QString title;
    QString titleIdentifier;
};

struct KNPreferenceItemInfo
{
    bool advanced=false;
    int type=0;
    QString title;
    QString valueName;
    QVariant value;
    QVariant defaultValue;
    QHash<QString, QVariant> property;
};
}

using namespace KNPreferenceItemList;

class QLabel;
class KNPreferenceItemBase;
class KNPreferenceItemGlobal : public QObject
{
    Q_OBJECT
public:
    static KNPreferenceItemGlobal *instance();
    QLabel *generateLabel(const QString &caption=QString());
    KNPreferenceItemBase *generateItem(const KNPreferenceItemInfo &info);
    static KNPreferenceItemInfo generateInfo(int type,
                                             const QString &title,
                                             const QString &valueName,
                                             const QVariant &value=QVariant(),
                                             const QVariant &defaultValue=QVariant(),
                                             bool advanced=false);
    void updateTitleCaption(QLabel *title,
                            const QString &caption);
    void updateItemCaption(KNPreferenceItemBase *item,
                           const QString &caption);
    QVariant itemData(KNPreferenceItemBase *item);

signals:
    void requireApplyPreference();

public slots:

private:
    static KNPreferenceItemGlobal *m_instance;
    explicit KNPreferenceItemGlobal(QObject *parent = 0);
    QFont m_titleFont;
    QPalette m_titlePalette;
};

#endif // KNPREFERENCEITEMGLOBAL_H
