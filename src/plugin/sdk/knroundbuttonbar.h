/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNROUNDBUTTONBAR_H
#define KNROUNDBUTTONBAR_H

#include <QWidget>

class QBoxLayout;
class QSignalMapper;
class KNRoundAnimeButton;
class KNRoundButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit KNRoundButtonBar(QWidget *parent = 0);
    ~KNRoundButtonBar();
    void addButton(KNRoundAnimeButton *button);
    void addButton(const QPixmap &icon);

signals:
    void currentIndexChanged(int index);

public slots:
    void setCurrentIndex(const int &index);

private:
    QBoxLayout *m_mainLayout;
    QSignalMapper *m_toolbarMappper;
    int m_buttonIndex=0, m_currentIndex=-1;
};

#endif // KNROUNDBUTTONBAR_H
