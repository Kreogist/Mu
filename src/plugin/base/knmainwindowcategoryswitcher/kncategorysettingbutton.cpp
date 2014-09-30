#include <QPropertyAnimation>

#include "kncategorysettingbutton.h"

KNCategorySettingButton::KNCategorySettingButton(QWidget *parent) :
    KNAbstractCategoryButton(parent)
{
    //Initial the slide animation.
    m_slideIn=new QPropertyAnimation(this);
    initialGeometryAnime(m_slideIn);
    m_slideOut=new QPropertyAnimation(this);
    initialGeometryAnime(m_slideOut);
}

void KNCategorySettingButton::startMouseInAnime()
{
    ;
}

void KNCategorySettingButton::startMouseOutAnime()
{
    ;
}

void KNCategorySettingButton::startMouseDownAnime()
{
    ;
}

void KNCategorySettingButton::startMouseUpAnime()
{
    ;
}

void KNCategorySettingButton::startInAnime()
{
    //Stop all slide anime.
    stopSlideAnime();
    //Set parameters.
    m_slideIn->setStartValue(generateOut());
    m_slideIn->setEndValue(generateIn());
    //Start slide animation.
    m_slideIn->start();
}

void KNCategorySettingButton::startOutAnime()
{
    //Stop all slide anime.
    stopSlideAnime();
    //Set parameters.
    m_slideOut->setStartValue(generateIn());
    m_slideOut->setEndValue(generateOut());
    //Start slide animation.
    m_slideOut->start();
}

void KNCategorySettingButton::initialGeometryAnime(QPropertyAnimation *anime)
{
    anime->setPropertyName("geometry");
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setTargetObject(this);
}

void KNCategorySettingButton::stopSlideAnime()
{
    m_slideIn->stop();
    m_slideOut->stop();
}

QRect KNCategorySettingButton::generateOut()
{
    return QRect(0, -height(), width(), height());
}

QRect KNCategorySettingButton::generateIn()
{
    return QRect(0, 0, width(), height());
}
