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
#include <QScopedPointer>

#include "sao/knmessageboxblock.h"
#include "sao/knmessageboxcontent.h"
#include "knopacityanimebutton.h"
#include "knlabellineedit.h"
#include "knglobal.h"

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
    m_showOkayButton(true),
    m_showAnime(new QSequentialAnimationGroup(this)),
    m_hideAnime(new QParallelAnimationGroup(this)),
    m_zoomIn(new QPropertyAnimation(this, "geometry", this)),
    m_fadeIn(new QPropertyAnimation(this, "windowOpacity", this)),
    m_expand(new QPropertyAnimation(this, "geometry", this)),
    m_fold(new QPropertyAnimation(this, "geometry", this)),
    m_fadeOut(new QPropertyAnimation(this, "windowOpacity", this)),
    m_dialogResizing(new QPropertyAnimation(this, "geometry", this)),
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
    pal.setColor(QPalette::WindowText, QColor(100,100,100));
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

    //Raise the widget.
    m_content->raise();
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
        //Get the main window locate screen.
        QDesktopWidget *desktop=qApp->desktop();
        //Get the target screen position.
        QRect targetGeometry=desktop->screenGeometry(knGlobal->mainWindow());
        //Calculate the X and Y base.
        xBase=targetGeometry.left() + (targetGeometry.width()>>1);
        yBase=targetGeometry.top() + (targetGeometry.height()>>1);
    }
    else
    {
        xBase=parentWidget()->x()+(parentWidget()->width()>>1);
        yBase=parentWidget()->y()+(parentWidget()->height()>>1);
    }
    //Calculate the final width, final height and middle rect.
    const int middleHeight=HeaderHeight+BottomHeight;
    const QSize preferSize=m_content->targetSize();
    int finalWidth=qMax(m_topBlock->widthHint(), preferSize.width()),
        finalHeight=middleHeight+preferSize.height();
    QRect middleRect(xBase-(finalWidth>>1),
                     yBase-(middleHeight>>1),
                     finalWidth,
                     middleHeight);
    //Set the parameters of the animations.
    m_zoomIn->setStartValue(QRect(xBase-(finalWidth>>2),
                                  yBase-(middleHeight>>2),
                                  (finalWidth>>1),
                                  (middleHeight>>1)));
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

void KNMessageBox::setButtonVisible(bool isOkayButton, bool isVisible)
{
    //Check is the user want to show the okay button.
    if(isOkayButton)
    {
        //Set the okay button visible.
        m_okayButton->setVisible(isVisible);
        //Mission complete.
        return;
    }
    //Set the cancel button visible.
    m_cancelButton->setVisible(isVisible);
}

void KNMessageBox::onActionZoomFinished()
{
    //Set the top block text.
    m_topBlock->setText(m_titleText.isEmpty()?"Information":m_titleText);
    //Set show the button.
    m_okayButton->setVisible(m_showOkayButton);
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
        setResult(QDialog::Accepted);
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
    setResult(QDialog::Rejected);
    //Launch the close anime.
    startCloseAnime();
}

inline void KNMessageBox::initialBlocks()
{
    //Configure the top block.
    QFont titleFont=m_topBlock->font();
    titleFont.setFamily("SAO UI");
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

    //----Expand Animation----
    m_dialogResizing->setEasingCurve(QEasingCurve::OutCubic);
    m_dialogResizing->setDuration(120);
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
    //Initial the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(m_bottomBlock);
    //Configure the button.
    button->setIcon(QIcon(iconPath));
    button->setFixedSize(ButtonSize, ButtonSize);
    //Give back the button.
    return button;
}

void KNMessageBox::startCloseAnime()
{
    //Hide the content first.
    m_content->hideContent();
    //Release the content according to the auto remove flag.
    m_content->checkAutoDelete();
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

void KNMessageBox::setOkayButtonVisible(bool showOkayButton)
{
    m_showOkayButton = showOkayButton;
}

bool KNMessageBox::showCancelButton() const
{
    return m_showCancelButton;
}

void KNMessageBox::information(const QString &text,
                               const QString &title,
                               const Qt::Alignment &alignment)
{
    //Generate a temporary message box.
    QScopedPointer<KNMessageBox> messageBox(new KNMessageBox);
    //Configure the message box.
    messageBox->setTitleText(title);
    //Generate the text holder label.
    QLabel *textHolder=new QLabel(text, messageBox.data());
    //Configure the text holder.
    textHolder->setContentsMargins(17,12,17,12);
    textHolder->setAlignment(alignment);
    textHolder->setMinimumHeight(78);
    //Set palette, the color of SAO style window cannot be changed.
    QPalette pal=textHolder->palette();
    pal.setColor(QPalette::WindowText, QColor(72,72,72));
    textHolder->setPalette(pal);
    //Set the content widget.
    messageBox->setContentWidget(textHolder);
    //Show up the message box.
    messageBox->exec();
}

QString KNMessageBox::getText(const QString &text,
                              const QString &title,
                              const QString &originalText)
{
    //Generate a temporary message box.
    QScopedPointer<KNMessageBox> messageBox(new KNMessageBox);
    //Configure the message box.
    messageBox->setTitleText(title);
    messageBox->setCancelButtonVisible(true);
    //Generate a temporary widget.
    QScopedPointer<QWidget> container(new QWidget);
    //Initial the layout of the container.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          container.data());
    //Set the main layout to container.
    container->setLayout(mainLayout);
    //Initial a label.
    QLabel *hintText=new QLabel(text, container.data());
    //Configure the hint text.
    hintText->setAlignment(Qt::AlignCenter);
    //Add the hint text to main layout.
    mainLayout->addWidget(hintText);
    //Generate a new line edit.
    KNLabelLineEdit *lineEdit=new KNLabelLineEdit(container.data());
    //Configure the line edit.
    lineEdit->setMinimumLightness(0xC0);
    lineEdit->setMediumLightness(0xE0);
    lineEdit->setText(originalText);
    lineEdit->selectAll();
    //Add line edit to main layout.
    mainLayout->addWidget(lineEdit);
    //Set the content widget to message box.
    messageBox->setContentWidget(container.data(), false);
    //Show up the message box.
    return (messageBox->exec()==QDialog::Accepted)?
                lineEdit->text():QString();
}

void KNMessageBox::setCancelButtonVisible(bool showCancelButton)
{
    m_showCancelButton = showCancelButton;
}

void KNMessageBox::resizeDialog(const QSize &contentSize)
{
    //Stop the resizing animation.
    m_dialogResizing->stop();
    //Set the start position to be the geometry.
    m_dialogResizing->setStartValue(geometry());
    //Set the target position.
    int finalHeight=HeaderHeight + BottomHeight + contentSize.height();
    m_dialogResizing->setEndValue(
                QRect(geometry().center().x()-(contentSize.width()>>1),
                      geometry().center().y()-(finalHeight>>1),
                      contentSize.width(),
                      finalHeight));
    //Start the dialog resizing.
    m_dialogResizing->start();
}

QString KNMessageBox::titleText() const
{
    return m_titleText;
}

void KNMessageBox::setTitleText(const QString &titleText)
{
    m_titleText=titleText;
}

void KNMessageBox::setContentWidget(QWidget *widget, bool autoDelete)
{
    //Set the content widget.
    m_content->setContent(widget, autoDelete);
    //Raise the widget.
    widget->raise();
}

