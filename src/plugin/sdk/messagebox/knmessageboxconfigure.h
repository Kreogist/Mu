/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMESSAGEBOXCONFIGURE_H
#define KNMESSAGEBOXCONFIGURE_H

#include <QFont>

#include <QObject>

class KNMessageBoxConfigure : public QObject
{
    Q_OBJECT
public:
    static KNMessageBoxConfigure *instance();
    QFont titleFont() const;
    void setTitleFont(const QFont &titleFont);
    QString defaultTitle() const;
    void setDefaultTitle(const QString &defaultTitle);

signals:

public slots:

private:
    static KNMessageBoxConfigure *m_instance;
    explicit KNMessageBoxConfigure(QObject *parent = 0);
    QFont m_titleFont;
    QString m_defaultTitle;
};

#endif // KNMESSAGEBOXCONFIGURE_H
