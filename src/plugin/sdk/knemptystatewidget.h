/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNEMPTYSTATEWIDGET_H
#define KNEMPTYSTATEWIDGET_H

#include <QStackedWidget>

class KNEmptyStateWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit KNEmptyStateWidget(QWidget *parent = 0);
    QWidget *emptyWidget() const;
    void setEmptyWidget(QWidget *emptyWidget);
    QWidget *contentWidget() const;
    void setContentWidget(QWidget *contentWidget);

signals:
    void aboutToBeShown();

public slots:
    void showEmptyWidget();
    void showContentWidget();

protected:
    void showEvent(QShowEvent *event);

private:
    QWidget *m_emptyWidget=nullptr;
    QWidget *m_contentWidget=nullptr;
};

#endif // KNEMPTYSTATEWIDGET_H
