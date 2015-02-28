/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMLINEEDIT_H
#define KNPREFERENCEITEMLINEEDIT_H

#include "knpreferenceitem.h"

class QLineEdit;
class KNPreferenceItemLineEdit : public KNPreferenceItem
{
    Q_OBJECT
public:
    explicit KNPreferenceItemLineEdit(QWidget *parent = 0);

signals:

public slots:
    void setValue(const QVariant &value);
    void setDefaultValue(const QVariant &defaultValue);

private:
    QLineEdit *m_editor;
};

#endif // KNPREFERENCEITEMLINEEDIT_H
