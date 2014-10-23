/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMGLOBAL_H
#define KNPREFERENCEITEMGLOBAL_H

namespace PreferenceItemGlobal
{
enum KNPreferenceWidgetIndex
{
    Switcher,
    LineEdit,
    PathEdit
};
}

using namespace PreferenceItemGlobal;

#include <QVariant>
#include <QObject>
#include <QFont>
#include <QPalette>

class QLabel;
class KNPreferenceItemBase;
class KNPreferenceItemGlobal : public QObject
{
    Q_OBJECT
public:
    static KNPreferenceItemGlobal *instance();
    QLabel *generateLabel(const QString &caption=QString());
    KNPreferenceItemBase *generateItem(const int &index,
                                       const QString &valueName,
                                       const QVariant &value=QVariant(),
                                       const QVariant &defaultValue=QVariant());
    void updateTitleCaption(QLabel *title,
                            const QString &caption);
    void updateItemCaption(KNPreferenceItemBase *item,
                           const QString &caption);

signals:

public slots:

private:
    static KNPreferenceItemGlobal *m_instance;
    explicit KNPreferenceItemGlobal(QObject *parent = 0);
    QFont m_titleFont;
    QPalette m_titlePalette;
};

#endif // KNPREFERENCEITEMGLOBAL_H
