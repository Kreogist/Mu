/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMPANEL_H
#define KNPREFERENCEITEMPANEL_H

#include <QHash>
#include <QLinkedList>

#include <QScrollArea>

class QBoxLayout;
class QLabel;
class KNConfigure;
class KNPreferenceItem;
class KNPreferenceItemPanel : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNPreferenceItemPanel(QWidget *parent = 0);
    bool advancedMode() const;
    void addTitle(QLabel *title, bool advanced=false);
    void addItem(KNPreferenceItem *item, bool advanced=false);

signals:
    void requireSetAdvancedVisible(const bool &ok);

public slots:
    void loadItemsValue();
    void saveItemsValue();
    void setAdvancedMode(bool advancedMode);
    void setNormalMode(bool normalMode)
    {
        setAdvancedMode(!normalMode);
    }

private:
    void setAdvancedItem(QWidget *item);
    QBoxLayout *m_mainLayout;
    QLinkedList<KNPreferenceItem *> m_items;
    bool m_advancedMode=false;
};

#endif // KNPREFERENCEITEMPANEL_H
