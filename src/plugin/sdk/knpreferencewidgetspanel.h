/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEWIDGETSPANEL_H
#define KNPREFERENCEWIDGETSPANEL_H

#include <QList>

#include "preference/knpreferenceitembase.h"

#include <QScrollArea>

class QBoxLayout;
class QLabel;
class KNPreferenceItemBase;
class KNPreferenceWidgetsPanel : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNPreferenceWidgetsPanel(QWidget *parent = 0);
    bool advancedMode() const;
    KNPreferenceItemBase *generateItem(const int &index,
                                              const QString &valueName,
                                              const QVariant &value=QVariant(),
                                              const QVariant &defaultValue=QVariant());
    QLabel *generateLabel(const QString &caption=QString());
    QString panelCaption() const;
    void setPanelCaption(const QString &panelCaption);
    void addTitle(QLabel *title,
                  const bool &isAdvanced=false);
    void addItem(KNPreferenceItemBase *item,
                 const bool &isAdvanced=false);

signals:
    void requireSaveConfigure();
    void requireSetAdvancedVisible(const bool &ok);

public slots:
    void setNormalMode(bool normalMode);
    void setAdvancedMode(bool advancedMode);

protected:

private:
    void setAdvancedItem(QWidget *item);
    QString m_panelCaption;
    QBoxLayout *m_mainLayout;
    QWidget *m_container;
    QFont m_titleFont;
    QPalette m_titlePalette;
    bool m_advancedMode=false;
};

#endif // KNPREFERENCEWIDGETSPANEL_H
