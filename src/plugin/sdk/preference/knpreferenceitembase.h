/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMBASE_H
#define KNPREFERENCEITEMBASE_H

#include <QVariant>
#include <QWidget>

class QLabel;
class KNPreferenceItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit KNPreferenceItemBase(QWidget *parent = 0);
    bool isAdvanced() const;
    virtual QVariant defaultValue() const=0;
    virtual QVariant value() const=0;
    QString caption() const;

signals:

public slots:
    void setAdvanced(const bool &isAdvanced);
    void setCaption(const QString &caption);
    virtual void setDefaultValue(const QVariant &defaultValue)=0;
    virtual void setValue(const QVariant &value)=0;

private:
    bool m_isAdvanced=false;
    QLabel *m_caption;
};

#endif // KNPREFERENCEITEMBASE_H
