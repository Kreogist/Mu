/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEWIDGETSPANEL_H
#define KNPREFERENCEWIDGETSPANEL_H

#include <QList>

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
    void addTitle(QLabel *title,
                  const bool &isAdvanced=false);
    void addItem(KNPreferenceItemBase *item,
                 const bool &isAdvanced=false);

signals:
    void requireSetAdvancedVisible(const bool &ok);

public slots:
    void savePanelData();
    void setNormalMode(bool normalMode);
    void setAdvancedMode(bool advancedMode);

protected:

private:
    void setAdvancedItem(QWidget *item);
    QString m_panelName;
    QBoxLayout *m_mainLayout;
    QList<KNPreferenceItemBase *> m_itemList;
    KNGlobal *m_global;
    bool m_advancedMode=false;
};

#endif // KNPREFERENCEWIDGETSPANEL_H
