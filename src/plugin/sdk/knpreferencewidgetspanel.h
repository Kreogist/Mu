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

namespace KNPreferenceWidgets
{
enum KNPreferenceWidgetIndex
{
    Switcher
};
}

using namespace KNPreferenceWidgets;

class QBoxLayout;
class QLabel;
class KNPreferenceWidgetsPanel : public QScrollArea
{
    Q_OBJECT
public:
    explicit KNPreferenceWidgetsPanel(QWidget *parent = 0);
    bool advancedMode() const;

signals:
    void requireSaveConfigure();
    void requireSetAdvancedVisible(const bool &ok);

public slots:
    void addTitle(const QString &titleText=QString(""),
                  const bool &isAdvanced=false);
    void addItem(const int &index,
                 const QString &caption,
                 const QVariant &value=QVariant(),
                 const QVariant &defaultValue=QVariant(),
                 const bool &isAdvanced=false);
    void setTitle(const int &index, const QString &title);
    void setNormalMode(bool normalMode);
    void setAdvancedMode(bool advancedMode);

protected:

private:
    void setAdvancedItem(QWidget *item);
    void addPreferenceWidget(KNPreferenceItemBase *widget);
    QList<KNPreferenceItemBase *> m_widgets;
    QList<QLabel *> m_titles;
    QBoxLayout *m_mainLayout;
    QWidget *m_container;
    QFont m_titleFont;
    QPalette m_titlePalette;
    bool m_advancedMode=false;
};

#endif // KNPREFERENCEWIDGETSPANEL_H
