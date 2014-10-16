/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEWIDGETSPANEL_H
#define KNPREFERENCEWIDGETSPANEL_H

#include <QList>

#include "preference/knpreferencewidgetbase.h"

#include <QWidget>

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
    void addTitle(const QString &title=QString(""));
    void setTitle(const int &index, const QString &title);

private:
    QList<KNPreferenceWidgetBase *> m_widgets;
    QList<QLabel *> m_titles;
    QBoxLayout *m_mainLayout;
};

#endif // KNPREFERENCEWIDGETSPANEL_H
