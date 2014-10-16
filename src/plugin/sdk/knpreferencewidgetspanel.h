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

#include <QWidget>

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
class KNPreferenceWidgetsPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceWidgetsPanel(QWidget *parent = 0);

signals:
    void requireSaveConfigure();

public slots:
    void addTitle(const QString &titleText=QString(""));
    void addItem(const int &index,
                 const QString &caption,
                 const bool &isAdvanced=false,
                 const QVariant &value=QVariant(),
                 const QVariant &defaultValue=QVariant());
    void setTitle(const int &index, const QString &title);

private:
    void addPreferenceWidget(KNPreferenceItemBase *widget);
    QList<KNPreferenceItemBase *> m_widgets;
    QList<QLabel *> m_titles;
    QBoxLayout *m_mainLayout;
    QFont m_titleFont;
    QPalette m_titlePalette;
};

#endif // KNPREFERENCEWIDGETSPANEL_H
