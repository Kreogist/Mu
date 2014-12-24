#include <QPropertyAnimation>
#include <QApplication>
#include <QBitmap>
#include <QDesktopWidget>
#include <QCursor>
#include <QShowEvent>
#include <QLabel>
#include <QDebug>

#include "knsaostyle.h"

#include "knsaosubmenu.h"

KNSAOSubMenu::KNSAOSubMenu(QWidget *parent) :
    QMenu(parent)
{
    //Initial the object name.
    setObjectName("SAOSubMenu");
    //Initial the style.
    setStyle(KNSAOStyle::instance());

    //Initial the indicator.
    m_indicator=new QWidget;
    //Set properties.
    m_indicator->setWindowFlags(Qt::ToolTip |
                                Qt::FramelessWindowHint);
    m_indicator->setAutoFillBackground(true);
    m_indicator->setAttribute(Qt::WA_NoBackground, true);
    m_indicator->setAttribute(Qt::WA_TranslucentBackground, true);
    m_indicator->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    //Set palette.
    QPalette pal=m_indicator->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(0,0,0,0));
    pal.setColor(QPalette::AlternateBase, QColor(0,0,0,0));
    pal.setColor(QPalette::ToolTipBase, QColor(255,255,255,0));
    pal.setColor(QPalette::ToolTipText, QColor(255,255,255,0));
    pal.setColor(QPalette::Button, QColor(255,255,255,0));
    pal.setColor(QPalette::ButtonText, QColor(255,255,255,0));
    m_indicator->setPalette(pal);
    //Generate the image label.
    QLabel *indicatorLabel=new QLabel(m_indicator);
    indicatorLabel->setPalette(pal);
    QPixmap indicatorPixmap=QPixmap("://public/indicator.png");
    indicatorLabel->setPixmap(indicatorPixmap);
    m_indicator->setMask(indicatorPixmap.mask());
    m_indicator->setFixedSize(indicatorPixmap.size());
    m_indicator->hide();

    //Initial the animation.
    m_start=new QPropertyAnimation(this, "geometry", this);
    m_start->setEasingCurve(QEasingCurve::OutCubic);
}

KNSAOSubMenu::~KNSAOSubMenu()
{
    delete m_indicator;
}

void KNSAOSubMenu::showEvent(QShowEvent *event)
{
    //Stop the animation.
    m_start->stop();
    //Move and show the indicator.
    int menuX=QCursor::pos().x()+m_indicator->width(),
        centerPosition=QCursor::pos().y();
    //Chechk the parent widget.
    if(parentWidget()!=nullptr)
    {
        //Check the parent object.
        if(parentWidget()->objectName()=="SAOSubMenu")
        {
            QMenu *parentMenu=(QMenu *)parentWidget();
            QRect menuActionGeometry=
                    parentMenu->actionGeometry(this->menuAction());
            menuX=parentMenu->x()+
                    menuActionGeometry.x()+
                    menuActionGeometry.width()+
                    m_indicator->width()/2;
            centerPosition=parentMenu->y()+
                    menuActionGeometry.y()+
                    menuActionGeometry.height()/2;
        }
        else
        {
            QPoint globalParentPos=
                    parentWidget()->mapToGlobal(QPoint(0,0));
            menuX=globalParentPos.x() +
                    parentWidget()->width()+
                    m_indicator->width()/2;
            centerPosition=globalParentPos.y()+parentWidget()->height()/2;
        }
    }
    m_indicator->move(menuX-m_indicator->width(),
                      centerPosition-m_indicator->height()/2);
    m_indicator->show();
    //Generate the prefer geometry.
    int preferTopPosition=centerPosition-height()/2;
    if(preferTopPosition<0)
    {
        preferTopPosition=0;
    }
    if(preferTopPosition+height()>QApplication::desktop()->height())
    {
        preferTopPosition=QApplication::desktop()->height()-height();
    }
    //Set the position.
    m_start->setStartValue(QRect(menuX,
                                 preferTopPosition+height()/2,
                                 width(),
                                 1));
    m_start->setEndValue(QRect(menuX,
                               preferTopPosition,
                               width(),
                               height()));
    //Start the animation.
    m_start->start();
    //Do show event.
    QMenu::showEvent(event);
}

void KNSAOSubMenu::hideEvent(QHideEvent *event)
{
    //Hide the indicator.
    m_indicator->hide();
    //Do hide event.
    QMenu::hideEvent(event);
}

