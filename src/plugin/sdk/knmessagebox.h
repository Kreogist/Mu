/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMESSAGEBOX_H
#define KNMESSAGEBOX_H

#include <QDialog>

class QSequentialAnimationGroup;
class QParallelAnimationGroup;
class QBoxLayout;
class QLabel;
class QPropertyAnimation;
class KNOpacityAnimeButton;
class KNMessageBlock;
class KNMessageContent;
class KNMessageBoxConfigure;
class KNMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit KNMessageBox(QWidget *parent = 0);
    QString title() const;

signals:

public slots:
    void setTitle(const QString &title);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual bool onActionOkayClose();

private slots:
    void onActionOkay();
    void onActionClose();

private:
    void initialShortCut();
    void initialDialog();
    void initialButton(KNOpacityAnimeButton *button);
    QPropertyAnimation *m_zoomIn, *m_fadeIn, *m_expand, *m_fold, *m_fadeOut;
    QParallelAnimationGroup *m_showAnime, *m_hideAnime;
    QSequentialAnimationGroup *m_expandAnime;
    KNMessageBoxConfigure *m_boxConfigureInstance;
    KNOpacityAnimeButton *m_okButton, *m_cancelButton;
    QFont m_fontTitle;
    KNMessageBlock *m_title, *m_buttom;
    KNMessageContent *m_content;
    QString m_titleText;
    int m_headerHeight=61,
    m_panelHeight=66,
    m_middleHeight=m_headerHeight+m_panelHeight;
};

#endif // KNMESSAGEBOX_H
