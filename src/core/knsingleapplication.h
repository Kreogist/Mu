/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNSINGLEAPPLICATION_H
#define KNSINGLEAPPLICATION_H

#include <QApplication>
#include <QStringList>
#include <QSharedMemory>

class QLocalServer;
class KNSingleApplication : public QApplication
{
    Q_OBJECT
public:
    explicit KNSingleApplication(int &argc,
                                 char *argv[],
                                 const QString uniqueKey);
    bool isPatternRunning() const;
    bool sendMessage(const QStringList &message);

signals:
    void messageAvailable(QStringList message);

public slots:
    void messageReceive();

private slots:

private:
    QString m_uniqueKey;
    QSharedMemory m_uniqueKeyMemory;
    bool m_isRunning=false;
    QLocalServer *m_messageServer;
    const int m_timeout=1000;
};

#endif // KNSINGLEAPPLICATION_H
