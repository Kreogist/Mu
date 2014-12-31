/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QWidget>
#include <QPainter>
#include <QVariant>

#include <QDebug>

#include "knmusicratingeditor.h"

#include "knmusicratingdelegate.h"

int KNMusicRatingDelegate::m_starSizeHint=20;

KNMusicRatingDelegate::KNMusicRatingDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    //Initial the star size.
    m_star=QPixmap(":/plugin/music/common/star.png");
    //Scale the pixmap
    m_star=m_star.scaled(m_starSizeHint,
                         m_starSizeHint,
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
}

void KNMusicRatingDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    //Enable antialiasing.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform, true);
    //Draw the background part of the widget.
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }
    //Draw the stars according to the number of the user sets.
    int starNum=index.data(Qt::DisplayRole).toInt();
    painter->save();
    painter->translate(option.rect.topLeft());
    for(int i=0; i<starNum; i++)
    {
        painter->drawPixmap(QPoint(m_starSizeHint*i, 0),
                            m_star);
    }
    painter->restore();
}

QSize KNMusicRatingDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    //Size is five stars' size.
    return QSize(m_starSizeHint*5, m_starSizeHint);
}

QWidget *KNMusicRatingDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    //Create the rating editor.
    KNMusicRatingEditor *editor=new KNMusicRatingEditor(parent);
    editor->setStarSizeHint(m_starSizeHint);
    connect(editor, &KNMusicRatingEditor::editingFinished,
            this, &KNMusicRatingDelegate::commitAndCloseEditor);
    return editor;
}

void KNMusicRatingDelegate::setEditorData(QWidget *editor,
                                          const QModelIndex &index) const
{
    //Set the star number to current number.
    KNMusicRatingEditor *starEditor=static_cast<KNMusicRatingEditor *>(editor);
    starEditor->setStarNum(index.data(Qt::DisplayRole).toInt());
}

void KNMusicRatingDelegate::setModelData(QWidget *editor,
                                         QAbstractItemModel *model,
                                         const QModelIndex &index) const
{
    //Set model data to star editor's star number.
    KNMusicRatingEditor *starEditor=static_cast<KNMusicRatingEditor *>(editor);
    model->setData(index, starEditor->starNum(), Qt::DisplayRole);
}

void KNMusicRatingDelegate::commitAndCloseEditor()
{
    QWidget *editor=static_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

int KNMusicRatingDelegate::starSizeHint()
{
    return m_starSizeHint;
}
