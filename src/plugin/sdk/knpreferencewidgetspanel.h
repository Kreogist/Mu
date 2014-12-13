/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEWIDGETSPANEL_H
#define KNPREFERENCEWIDGETSPANEL_H

#include <QHash>
#include <QStringList>

#include "preference/knpreferenceitemglobal.h"

#include <QScrollArea>

class QBoxLayout;
class QLabel;
class KNGlobal;
class KNPreferenceItemBase;
class KNPreferenceWidgetsPanel : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNPreferenceWidgetsPanel(QWidget *parent = 0);
    bool advancedMode() const;
    QString panelName() const;
    void setPanelName(const QString &panelName);
    void insertItemInfoList(const KNPreferenceTitleInfo &listTitle,
                            const QList<KNPreferenceItemInfo> &list);

signals:
    void requireSetAdvancedVisible(const bool &ok);

public slots:
    void savePanelData();
    void setNormalMode(bool normalMode);
    void setAdvancedMode(bool advancedMode);

protected:

private:
    void addTitle(QLabel *title,
                  const bool &isAdvanced=false);
    void addItem(KNPreferenceItemBase *item,
                 const bool &isAdvanced=false);
    void generateItemFromInfoList(const KNPreferenceTitleInfo &listTitle,
                                  const QList<KNPreferenceItemInfo> &list);
    void updateItemFromInfoList(const KNPreferenceTitleInfo &listTitle,
                                const QList<KNPreferenceItemInfo> &list);
    void setAdvancedItem(QWidget *item);
    QString m_panelName;
    QBoxLayout *m_mainLayout;
    QHash<QString, QLabel *> m_titleList;
    QHash<QString, KNPreferenceItemBase *> m_itemList;
    KNPreferenceItemGlobal *m_preferenceItemGlobal;
    KNGlobal *m_global;
    bool m_advancedMode=false;
};

#endif // KNPREFERENCEWIDGETSPANEL_H
