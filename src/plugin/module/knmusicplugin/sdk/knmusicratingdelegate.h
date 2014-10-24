/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICRATINGDELEGATE_H
#define KNMUSICRATINGDELEGATE_H

#include <QStyledItemDelegate>

class KNMusicRatingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit KNMusicRatingDelegate(QWidget *parent = 0);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;
signals:

public slots:

private slots:
    void commitAndCloseEditor();

private:
    QPixmap m_star;
    int m_starSizeHint=20;
};

#endif // KNMUSICRATINGDELEGATE_H
