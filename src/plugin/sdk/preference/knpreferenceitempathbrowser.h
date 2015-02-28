/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMPATHBROWSER_H
#define KNPREFERENCEITEMPATHBROWSER_H

#include "knpreferenceitem.h"

class QPushButton;
class KNPathLineEdit;
class KNLabelButton;
class KNPreferenceItemPathBrowser : public KNPreferenceItem
{
    Q_OBJECT
public:
    explicit KNPreferenceItemPathBrowser(QWidget *parent = 0);
    bool ensureExist() const;
    void setEnsureExist(bool ensureExist);

signals:
    void dirMoved();

public slots:
    void retranslate();
    void setDefaultValue(const QVariant &defaultValue);
    void setValue(const QVariant &value);

private slots:
    void onActionBrowseFolder();
    void onActionMoveFolder();
    void onActionGotoFolder();
    void onActionPathExist();
    void onActionPathNotExist();

private:
    inline void initialButtons();
    inline void initialLineEdit();
    QPalette m_existEditPalette,
             m_notExistEditPalette;
    QPushButton *m_browse, *m_moveTo;
    KNLabelButton *m_goto;
    KNPathLineEdit *m_pathEditor;
    bool m_ensureExist=false;
};

#endif // KNPREFERENCEITEMPATHBROWSER_H
