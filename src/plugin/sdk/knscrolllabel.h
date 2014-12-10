/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNSCROLLLABEL_H
#define KNSCROLLLABEL_H

#include <QWidget>

class KNGraphicsGlowEffect;
class KNScrollLabel : public QWidget
{
    Q_OBJECT
public:
    explicit KNScrollLabel(QWidget *parent = 0);
    QSize sizeHint() const;
    QString text() const;
    void setText(const QString &text);
    qreal opacity() const;
    void setFont(const QFont &font);

signals:

public slots:
    void setOpacity(const qreal &opacity);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionMoving();
    void onActionWatingFinished();

private:
    void stopAllTimer();
    void updateAnimeParameters();
    KNGraphicsGlowEffect *m_glowEffect;
    QString m_text;
    QTimer *m_movingAnime, *m_waiting;
    int m_leftMostX=0, m_textX=0;
    qreal m_glowRadius=9.0, m_opacity=1.0;
    bool m_movingLeft=true;
};

#endif // KNSCROLLLABEL_H
