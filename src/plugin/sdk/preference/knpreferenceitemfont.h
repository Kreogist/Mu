/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMFONT_H
#define KNPREFERENCEITEMFONT_H

#include "knpreferenceitem.h"

class QPushButton;
class KNMouseDetectLabel;
class KNPreferenceItemFont : public KNPreferenceItem
{
    Q_OBJECT
public:
    explicit KNPreferenceItemFont(QWidget *parent = 0);
    ~KNPreferenceItemFont();

public slots:
    void setDefaultValue(const QVariant &defaultValue);
    void setValue(const QVariant &value);

private slots:
    void retranslate();
    void onActionSelectFont();
    void onActionShowTooltip();
    void onActionHideTooltip();
    void updateOverview();

private:
    KNMouseDetectLabel *m_previewIcon;
    QLabel *m_fontOverview, *m_previewTooltip;
    QPushButton *m_changeFont;
};

#endif // KNPREFERENCEITEMFONT_H
