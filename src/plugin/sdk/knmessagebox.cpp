/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QAction>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QKeySequence>
#include <QLabel>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include "knopacityanimebutton.h"
#include "messagebox/knmessageblock.h"
#include "messagebox/knmessageboxconfigure.h"
#include "messagebox/knmessagecontent.h"

#include "knmessagebox.h"

KNMessageBox::KNMessageBox(QWidget *parent) :
    QDialog(parent)
{
    //Set properties.
    //Reset window flags, SAO style show be no border, ToolTip is the best
    //choice. Why not Qt::FramelessWindowHint? Ask Gnome Shell, I don't know.
    setWindowFlags(Qt::ToolTip);

    //Initial the configure.
    m_boxConfigureInstance=KNMessageBoxConfigure::instance();

    //Set layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial title block.
    m_title=new KNMessageBlock(this);
    m_title->setFont(m_boxConfigureInstance->titleFont());
    m_title->setTopColor(QColor(240,240,240));
    m_title->setButtomColor(QColor(255,255,255));
    m_title->setMaximumHeight(m_headerHeight);
    m_title->setMinimumWidth(0);
    setTitle(m_boxConfigureInstance->defaultTitle());
    mainLayout->addWidget(m_title, 1);

    //Initial the content widget.
    m_content=new KNMessageContent(this);
    m_content->setMinimumHeight(1);
    mainLayout->addWidget(m_content);

    //Initial the bottom block.
    m_buttom=new KNMessageBlock(this);
    m_buttom->setTopColor(QColor(244,244,244));
    m_buttom->setButtomColor(QColor(255,255,255));
    m_buttom->setMaximumHeight(m_panelHeight);
    mainLayout->addWidget(m_buttom, 1);

    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_buttom);
    buttonLayout->setContentsMargins(0,0,0,0);
    buttonLayout->setSpacing(0);
    m_buttom->setLayout(buttonLayout);
    //Initial control buttons: Ok and Cancel.
    m_okButton=new KNOpacityAnimeButton(this);
    m_okButton->setIcon(QPixmap("://public/messagebox/ok.png"));
    initialButton(m_okButton);
    connect(m_okButton, &KNOpacityAnimeButton::clicked,
            this, &KNMessageBox::onActionOkay);
    buttonLayout->addWidget(m_okButton);

    m_cancelButton=new KNOpacityAnimeButton(this);
    initialButton(m_cancelButton);
    m_cancelButton->hide();
    m_cancelButton->setIcon(QPixmap("://public/messagebox/cancel.png"));
    buttonLayout->addWidget(m_cancelButton);
    connect(m_cancelButton, &KNOpacityAnimeButton::clicked,
            this, &KNMessageBox::onActionClose);

    //Initial short cut key.
    initialShortCut();

    //Initial the show animations.
    m_expandAnime=new QSequentialAnimationGroup(this);
    m_showAnime=new QParallelAnimationGroup(this);
    m_hideAnime=new QParallelAnimationGroup(this);

    //Initial animations.
    m_zoomIn=new QPropertyAnimation(this, "geometry", this);
    m_zoomIn->setDuration(100);
    m_showAnime->addAnimation(m_zoomIn);

    m_fadeIn=new QPropertyAnimation(this, "windowOpacity", this);
    m_fadeIn->setDuration(100);
    m_fadeIn->setStartValue(0.0);
    m_fadeIn->setEndValue(1.0);
    m_showAnime->addAnimation(m_fadeIn);
    m_expandAnime->addAnimation(m_showAnime);

    m_expand=new QPropertyAnimation(this, "geometry", this);
    m_expand->setDuration(50);
    m_expandAnime->addAnimation(m_expand);

    m_fold=new QPropertyAnimation(this, "geometry", this);
    m_fold->setDuration(50);
    m_fold->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_fold, &QPropertyAnimation::finished,
            this, &KNMessageBox::close);
    m_hideAnime->addAnimation(m_fold);

    m_fadeOut=new QPropertyAnimation(this, "windowOpacity", this);
    m_fadeOut->setEasingCurve(QEasingCurve::OutCubic);
    m_fadeOut->setDuration(50);
    m_fadeOut->setStartValue(1.0);
    m_fadeOut->setEndValue(0.0);
    m_hideAnime->addAnimation(m_fadeOut);
}

void KNMessageBox::showEvent(QShowEvent *event)
{
    //Initial the dialog.
    initialDialog();
    //Stop anime groups.
    m_expandAnime->stop();
    m_hideAnime->stop();
    //Set geometry.
    int xBase, yBase;
    if(parentWidget()==nullptr)
    {
        xBase=QApplication::desktop()->width()>>1;
        yBase=QApplication::desktop()->height()>>1;
    }
    else
    {
        xBase=parentWidget()->x()+(parentWidget()->frameGeometry().width()>>1);
        yBase=parentWidget()->y()+(parentWidget()->frameGeometry().height()>>1);
    }
    int finalWidth=m_title->preferWidth(),
        finalHeight=m_middleHeight+m_content->preferHeight();
    QRect middleRect=QRect(xBase-(finalWidth>>1),
                           yBase-(m_middleHeight>>1),
                           finalWidth,
                           m_middleHeight);
    m_zoomIn->setStartValue(QRect(xBase-(finalWidth>>2),
                                  yBase-(m_middleHeight>>2),
                                  (finalWidth>>1),
                                  (m_middleHeight>>1)));
    m_zoomIn->setEndValue(middleRect);
    m_expand->setStartValue(middleRect);
    m_expand->setEndValue(QRect(middleRect.x(),
                                yBase-(finalHeight>>1),
                                finalWidth,
                                finalHeight));
    //Start show anime group.
    m_expandAnime->start();
    //Show dialog.
    QDialog::showEvent(event);
    //Set focus.
    setFocus();
}

void KNMessageBox::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    //Draw a rect at final.
    QPainter painter(this);
    painter.setPen(QColor(255,255,255));
    painter.setBrush(QBrush(QColor(0,0,0,0)));
    painter.drawRect(QRect(0,0,width()-1,height()-1));
}

bool KNMessageBox::onActionOkayClose()
{
    return true;
}

void KNMessageBox::onActionOkay()
{
    if(onActionOkayClose())
    {
        onActionClose();
    }
}

void KNMessageBox::onActionClose()
{
    //Stop anime groups.
    m_expandAnime->stop();
    m_hideAnime->stop();
    //Because the width cannot set to 0 when there is text, clear it.
    m_title->clear();
    //Because the width cannot set to 0 when button is shown, hide it.
    m_okButton->hide();
    m_cancelButton->hide();
    //Set close animation properties.
    m_fold->setStartValue(geometry());
    m_fold->setEndValue(QRect(x()+(frameGeometry().width()/2),
                                    y(),
                                    0,
                                    height()));
    //Start anime.
    m_hideAnime->start();
}

void KNMessageBox::initialShortCut()
{
    QAction *escapeClose=new QAction(this);
    escapeClose->setShortcut(QKeySequence(QKeySequence::Close));
    connect(escapeClose, SIGNAL(triggered()),
            this, SLOT(close()));
    addAction(escapeClose);
}

void KNMessageBox::initialDialog()
{
    //Update title.
    m_title->setText(m_titleText);
    //Update button.
    m_okButton->show();
}

void KNMessageBox::initialButton(KNOpacityAnimeButton *button)
{
    button->setMaximumSize(40,40);
}

QString KNMessageBox::title() const
{
    return m_titleText;
}

void KNMessageBox::setTitle(const QString &title)
{
    m_titleText=title;
}
