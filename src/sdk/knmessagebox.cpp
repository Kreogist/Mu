/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QApplication>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "sao/knmessageboxblock.h"
#include "sao/knmessageboxcontent.h"
#include "knopacityanimebutton.h"

#include "knmessagebox.h"

#include <QDebug>

#define HeaderHeight 61
#define BottomHeight 66
#define ButtonSize 40

KNMessageBox::KNMessageBox(QWidget *parent) :
    QDialog(parent),
    m_topBlock(new KNMessageBoxBlock(this)),
    m_bottomBlock(new KNMessageBoxBlock(this)),
    m_content(new KNMessageBoxContent(this)),
    m_okayButton(generateButton("://public/ok.png")),
    m_cancelButton(generateButton("://public/cancel.png")),
    m_titleText(QString()),
    m_showCancelButton(false),
    m_showAnime(new QSequentialAnimationGroup(this)),
    m_hideAnime(new QParallelAnimationGroup(this)),
    m_zoomIn(new QPropertyAnimation(this, "geometry", this)),
    m_fadeIn(new QPropertyAnimation(this, "windowOpacity", this)),
    m_expand(new QPropertyAnimation(this, "geometry", this)),
    m_fold(new QPropertyAnimation(this, "geometry", this)),
    m_fadeOut(new QPropertyAnimation(this, "windowOpacity", this)),
    m_pressed(false)
{
    //Set properties.
    setAutoFillBackground(true);
    setWindowOpacity(0.9);
    //Reset window flags, SAO style show be no border, ToolTip is the best
    //choice. Why not Qt::FramelessWindowHint? Ask Gnome Shell, I don't know.
    //Here's a Mac OS X hack. We need to set it to FramelessWindowHint.
#ifdef Q_OS_MACX
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
#else
    setWindowFlags(Qt::ToolTip);
#endif
    //Set the focus policy after set the tooltip window flags.
    setFocusPolicy(Qt::StrongFocus);

    //Set palette, the color of SAO style window cannot be changed.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(240,240,240));
    setPalette(pal);

    //Initial elements.
    initialBlocks();
    initialAnimes();

    //Initial the main layout of the message box.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add blocks and content container to layout.
    mainLayout->addWidget(m_topBlock, 1);
    mainLayout->addWidget(m_content);
    mainLayout->addWidget(m_bottomBlock, 1);
}

void KNMessageBox::showEvent(QShowEvent *event)
{
    //Configure all the elements first.
    configureElements();
    //Stop all the animations.
    m_showAnime->stop();
    m_hideAnime->stop();
    //Configure animation parameters.
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
    //Calculate the final width, final height and middle rect.
    const int m_middleHeight=HeaderHeight+BottomHeight;
    const QSize preferSize=m_content->targetSize();
    int finalWidth=qMax(m_topBlock->widthHint(), preferSize.width()),
            finalHeight=m_middleHeight+preferSize.height();
    QRect middleRect(xBase-(finalWidth>>1),
                     yBase-(m_middleHeight>>1),
                     finalWidth,
                     m_middleHeight);
    //Set the parameters of the animations.
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
    //Start show animation.
    m_showAnime->start();
    //Show dialog.
    QDialog::showEvent(event);
    //Set the focus.
    activateWindow();
}

void KNMessageBox::mousePressEvent(QMouseEvent *event)
{
    //Set the pressed flag.
    m_pressed=true;
    //Save the current mouse position as reference.
    m_referencePoint=event->pos();
    //Do original pressed event.
    QDialog::mousePressEvent(event);
}

void KNMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    //Do the original mouse move event.
    QDialog::mouseMoveEvent(event);
    //Check the mouse pressed flag.
    if(m_pressed)
    {
        //Move the window.
        move(event->globalPos()-m_referencePoint);
    }
}

void KNMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the original mouse release event.
    QDialog::mouseReleaseEvent(event);
    //Reset the mouse pressed flag.
    m_pressed=false;
}

bool KNMessageBox::okayPressed()
{
    //Return true for default.
    return true;
}

void KNMessageBox::cancelPressed()
{
    //For default, it will do nothing.
}

void KNMessageBox::onActionZoomFinished()
{
    //Set the top block text.
    m_topBlock->setText(m_titleText.isEmpty()?"Information":m_titleText);
    //Set show the button.
    m_okayButton->show();
    m_cancelButton->setVisible(m_showCancelButton);
}

void KNMessageBox::onActionShowFinished()
{
    //Show the content widget.
    m_content->showContent();
}

void KNMessageBox::onActionOkayClicked()
{
    //If the okay pressed, check we can close or not.
    if(okayPressed())
    {
        //Accept the process after animation finished.
        connect(m_hideAnime, &QParallelAnimationGroup::finished,
                this, &KNMessageBox::accept);
        //Set the result data.
        setResult(true);
        //Launch the close anime.
        startCloseAnime();
    }
}

void KNMessageBox::onActionCancelClicked()
{
    //Do the cancel action.
    cancelPressed();
    //Reject the process after animation finished.
    connect(m_hideAnime, &QParallelAnimationGroup::finished,
            this, &KNMessageBox::reject);
    //Set the result data.
    setResult(false);
    //Launch the close anime.
    startCloseAnime();
}

inline void KNMessageBox::initialBlocks()
{
    //Configure the top block.
    QFont titleFont=m_topBlock->font();
    titleFont.setFamily("sao");
    titleFont.setPixelSize(15);
    m_topBlock->setFont(titleFont);
    m_topBlock->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_topBlock->setGradientColor(QColor(240,240,240), QColor(255,255,255));
    m_topBlock->setMinimumHeight(0);
    m_topBlock->setMaximumHeight(HeaderHeight);

    //Configure the bottom block.
    m_bottomBlock->setGradientColor(QColor(244,244,244), QColor(255,255,255));
    m_bottomBlock->setMinimumHeight(0);
    m_bottomBlock->setMaximumHeight(HeaderHeight);

    //Link ok and cancel button.
    connect(m_okayButton, SIGNAL(clicked(bool)),
            this, SLOT(onActionOkayClicked()));
    connect(m_cancelButton, SIGNAL(clicked(bool)),
            this, SLOT(onActionCancelClicked()));
    //Add ok and cancel button to bottom block.
    QBoxLayout *bottomLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_bottomBlock);
    bottomLayout->setContentsMargins(0,0,0,0);
    bottomLayout->setSpacing(0);
    bottomLayout->addWidget(m_okayButton);
    bottomLayout->addWidget(m_cancelButton);
    m_bottomBlock->setLayout(bottomLayout);
}

void KNMessageBox::initialAnimes()
{
    //----Show animation----
    //Step 1: expand the dialog to a minimal size, from 0 width, 0 height to
    //target width and minimal height. It's will be run in a parallel animation
    //group.
    QParallelAnimationGroup *expandGroup=new QParallelAnimationGroup(this);
    //Step 1 zoom in part.
    m_zoomIn->setEasingCurve(QEasingCurve::OutCubic);
    m_zoomIn->setDuration(120);
    connect(m_zoomIn, &QPropertyAnimation::finished,
            this, &KNMessageBox::onActionZoomFinished);
    expandGroup->addAnimation(m_zoomIn);
    //Step 1 opacity part.
    m_fadeIn->setDuration(120);
    m_fadeIn->setStartValue(0.0);
    m_fadeIn->setEndValue(1.0);
    expandGroup->addAnimation(m_fadeIn);
    //Add step 1 to show anime queue.
    m_showAnime->addAnimation(expandGroup);
    //Step 2: expand the dialog to full target size.
    m_expand->setDuration(100);
    //Add step 2 to show anime queue.
    m_showAnime->addAnimation(m_expand);
    connect(m_showAnime, &QSequentialAnimationGroup::finished,
            this, &KNMessageBox::onActionShowFinished);

    //----Hide Animation----
    //Fold the dialog to 0 width, target height. And reduce the opacity.
    //Hide fold width part.
    m_fold->setDuration(120);
    m_fold->setEasingCurve(QEasingCurve::OutCubic);
    //Add fold part to hide anime.
    m_hideAnime->addAnimation(m_fold);
    //Opacity reduce part.
    m_fadeOut->setEasingCurve(QEasingCurve::OutCubic);
    m_fadeOut->setDuration(120);
    m_fadeOut->setStartValue(1.0);
    m_fadeOut->setEndValue(0.0);
    //Add opacity reduce part to hide anime.
    m_hideAnime->addAnimation(m_fadeOut);
}

inline void KNMessageBox::configureElements()
{
    //Hide the content.
    m_content->hideContent();
    //Hide the button.
    m_okayButton->hide();
    m_cancelButton->hide();
}

inline KNOpacityAnimeButton *KNMessageBox::generateButton(
        const QString &iconPath)
{
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(m_bottomBlock);
    button->setIcon(QIcon(iconPath));
    button->setFixedSize(ButtonSize, ButtonSize);
    return button;
}

void KNMessageBox::startCloseAnime()
{
    //Hide the content first.
    m_content->hide();
    //Stop all the animations.
    m_showAnime->stop();
    m_hideAnime->stop();
    //Clear the title text, and hide the button.
    //Because the width cannot be 0 if there's any sub element display.
    m_topBlock->clear();
    m_okayButton->hide();
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

bool KNMessageBox::showCancelButton() const
{
    return m_showCancelButton;
}

void KNMessageBox::setShowCancelButton(bool showCancelButton)
{
    m_showCancelButton = showCancelButton;
}

QString KNMessageBox::titleText() const
{
    return m_titleText;
}

void KNMessageBox::setTitleText(const QString &titleText)
{
    m_titleText=titleText;
}

void KNMessageBox::setContentWidget(QWidget *widget)
{
    //Set the content widget.
    m_content->setContent(widget);
}

