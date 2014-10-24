/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMPATHBROWSER_H
#define KNPREFERENCEITEMPATHBROWSER_H

#include "knpreferenceitembase.h"

class QPushButton;
class KNPathLineEdit;
class KNPreferenceItemPathBrowser : public KNPreferenceItemBase
{
    Q_OBJECT
public:
    explicit KNPreferenceItemPathBrowser(QWidget *parent = 0);
    QVariant defaultValue() const;
    QVariant value() const;
    bool ensureExist() const;
    void setEnsureExist(bool ensureExist);

signals:

public slots:
    void retranslate();
    void setDefaultValue(const QVariant &defaultValue);
    void setValue(const QVariant &value);

private slots:
    void onActionBrowseFolder();
    void onActionPathExist();
    void onActionPathNotExist();

private:
    QString m_defaultValue;
    QPalette m_existEditPalette,
             m_notExistEditPalette;
    QPushButton *m_browse;
    KNPathLineEdit *m_pathEditor;
    bool m_ensureExist=false;
};

#endif // KNPREFERENCEITEMPATHBROWSER_H
