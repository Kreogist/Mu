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
#include <QScopedPointer>

#include "knopacityanimebutton.h"
#include "messagebox/knmessageblock.h"
#include "messagebox/knmessageboxconfigure.h"
#include "messagebox/knmessagecontent.h"

#include "knmessagebox.h"

#include <QDebug>

KNMessageBox::KNMessageBox(QWidget *parent) :
    QDialog(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setWindowOpacity(0.9);
    //Reset window flags, SAO style show be no border, ToolTip is the best
    //choice. Why not Qt::FramelessWindowHint? Ask Gnome Shell, I don't know.
    setWindowFlags(Qt::ToolTip);
    //Set the focus policy after set the tooltip window flags.
    setFocusPolicy(Qt::StrongFocus);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(240,240,240));
    setPalette(pal);

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
    connect(m_title, &KNMessageBlock::requireMoveTo,
            this, &KNMessageBox::onActionMove);
    m_title->setFocusProxy(this);
    m_title->setFont(m_boxConfigureInstance->titleFont());
    m_title->setTopColor(QColor(240,240,240));
    m_title->setButtomColor(QColor(255,255,255));
    m_title->setMaximumHeight(m_headerHeight);
    m_title->setMinimumWidth(0);
    setTitle(m_boxConfigureInstance->defaultTitle());
    mainLayout->addWidget(m_title, 1);

    //Initial the content widget.
    m_content=new KNMessageContent(this);
    m_content->setFocusProxy(this);
    m_content->setMinimumHeight(1);
    mainLayout->addWidget(m_content);

    //Initial the bottom block.
    m_buttom=new KNMessageBlock(this);
    m_buttom->setFocusProxy(this);
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
            this, &KNMessageBox::onActionCancel);

    //Initial short cut key.
    initialShortCut();

    //Initial the show animations.
    m_expandAnime=new QSequentialAnimationGroup(this);
    m_showAnime=new QParallelAnimationGroup(this);
    m_hideAnime=new QParallelAnimationGroup(this);

    //Initial animations.
    m_zoomIn=new QPropertyAnimation(this, "geometry", this);
    m_zoomIn->setEasingCurve(QEasingCurve::OutCubic);
    m_zoomIn->setDuration(120);
    m_showAnime->addAnimation(m_zoomIn);

    m_fadeIn=new QPropertyAnimation(this, "windowOpacity", this);
    m_fadeIn->setDuration(120);
    m_fadeIn->setStartValue(0.0);
    m_fadeIn->setEndValue(1.0);
    m_showAnime->addAnimation(m_fadeIn);
    m_expandAnime->addAnimation(m_showAnime);

    m_expand=new QPropertyAnimation(this, "geometry", this);
    m_expand->setDuration(100);
    m_expandAnime->addAnimation(m_expand);
    connect(m_expandAnime, &QSequentialAnimationGroup::finished,
            this, &KNMessageBox::onActionExpandFinished);

    m_fold=new QPropertyAnimation(this, "geometry", this);
    m_fold->setDuration(120);
    m_fold->setEasingCurve(QEasingCurve::OutCubic);
    m_hideAnime->addAnimation(m_fold);

    m_fadeOut=new QPropertyAnimation(this, "windowOpacity", this);
    m_fadeOut->setEasingCurve(QEasingCurve::OutCubic);
    m_fadeOut->setDuration(120);
    m_fadeOut->setStartValue(1.0);
    m_fadeOut->setEndValue(0.0);
    m_hideAnime->addAnimation(m_fadeOut);
}

bool KNMessageBox::question(const QString &title,
                            const QString &text)
{
    //Generate a message box first.
    QScopedPointer<KNMessageBox> questionBox(new KNMessageBox);
    //Set properties.
    questionBox->setTitle(title);
    questionBox->enableCancel();
    //Initial the question widget.
    QLabel *questionHint=new QLabel(text, questionBox.data());
    questionHint->setContentsMargins(10,10,10,10);
    questionHint->setAlignment(Qt::AlignCenter);
    questionHint->setMinimumHeight(78);
    //Set the content widget.
    questionBox->setContent(questionHint);
    //Launch the message box, judge the dialog data.
    return (questionBox->exec()==QDialog::Accepted);
}

void KNMessageBox::information(const QString &title, const QString &text)
{
    //Generate a message box first.
    QScopedPointer<KNMessageBox> informationBox(new KNMessageBox);
    //Set properties.
    informationBox->setTitle(title);
    //Initial the question widget.
    QLabel *informationHint=new QLabel(text, informationBox.data());
    informationHint->setContentsMargins(10,10,10,10);
    informationHint->setAlignment(Qt::AlignCenter);
    informationHint->setMinimumHeight(78);
    //Set the content widget.
    informationBox->setContent(informationHint);
    //Launch the message box.
    informationBox->exec();
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
    int finalWidth=qMax(m_title->preferWidth(), m_content->preferWidth()),
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
    //Set the focus.
    activateWindow();
}

void KNMessageBox::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    //Draw a rect at final.
    QPainter painter(this);
    painter.setPen(QColor(255,255,255));
    painter.setBrush(QColor(0,0,0,0));
    painter.drawRect(QRect(0,0,width()-1,height()-1));
}

bool KNMessageBox::onActionOkayClose()
{
    return true;
}

void KNMessageBox::onActionExpandFinished()
{
    //Show the content widget.
    m_content->showContent();
    //Set the focus.
    m_okButton->setFocus();
}

void KNMessageBox::onActionOkay()
{
    if(onActionOkayClose())
    {
        //Linked the finished animation to accept.
        connect(m_hideAnime, &QParallelAnimationGroup::finished,
                this, &KNMessageBox::accept);
        //Set the result data.
        setResult(true);
        //Close the dialog.
        onActionClose();
        return;
    }
}

void KNMessageBox::onActionCancel()
{
    //Linked the finished animation to reject.
    connect(m_hideAnime, &QParallelAnimationGroup::finished,
            this, &KNMessageBox::reject);
    //Set the result data.
    setResult(false);
    //Close the dialog.
    onActionClose();
}

void KNMessageBox::onActionClose()
{
    //Hide content first.
    m_content->hideContent();
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

void KNMessageBox::onActionMove(const QPoint &point)
{
    move(point);
}

void KNMessageBox::initialShortCut()
{
    QAction *escapeClose=new QAction(this);
    escapeClose->setShortcut(QKeySequence(Qt::Key_Escape));
    escapeClose->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(escapeClose, SIGNAL(triggered()),
            this, SLOT(onActionCancel()));
    addAction(escapeClose);

    QAction *acceptClose=new QAction(this);
    acceptClose->setShortcut(QKeySequence(Qt::Key_Return));
    acceptClose->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(acceptClose, SIGNAL(triggered()),
            this, SLOT(onActionOkay()));
    addAction(acceptClose);
}

void KNMessageBox::initialDialog()
{
    //Update title.
    m_title->setText(m_titleText);
    //Update button.
    m_okButton->show();
    //Hide the content.
    m_content->hideContent();
}

void KNMessageBox::initialButton(KNOpacityAnimeButton *button)
{
    button->setMaximumSize(40,40);
}

QString KNMessageBox::title() const
{
    return m_titleText;
}

QWidget *KNMessageBox::content() const
{
    return m_content->content();
}

void KNMessageBox::enableCancel()
{
    m_cancelButton->show();
}

void KNMessageBox::setContent(QWidget *content)
{
    m_content->setContent(content);
}

void KNMessageBox::setTitle(const QString &title)
{
    m_titleText=title;
}
