#include <QTimeLine>
#include <QMouseEvent>

#include "kncategorysettingbutton.h"

#include "kncategoryswitcherwidget.h"

KNCategorySwitcherWidget::KNCategorySwitcherWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Set background.
    m_palette=palette();
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);

    //Initial the buttons.
    initialButtons();

    //Initial timeline.
    m_inBackground=new QTimeLine(200, this);
    m_inBackground->setEndFrame(200);
    initialTimeLine(m_inBackground);

    m_outBackground=new QTimeLine(200, this);
    m_outBackground->setEndFrame(0);
    initialTimeLine(m_outBackground);
    connect(m_outBackground, &QTimeLine::finished,
            this, &KNCategorySwitcherWidget::onActionHideComplete);
}

void KNCategorySwitcherWidget::showSwitcher()
{
    //Stop all the anime.
    stopAllAnime();
    //Set properties.
    m_inBackground->setStartFrame(m_backgroundColor.alpha());
    //Start anime.
    m_inBackground->start();
    //Make widget visible.
    show();
    //Start button animations.
    startButtonInAnime();
}

void KNCategorySwitcherWidget::hideSwitcher()
{
    //Stop all the anime.
    stopAllAnime();
    //Set properties.
    m_outBackground->setStartFrame(m_backgroundColor.alpha());
    //Start anime.
    m_outBackground->start();
    //Start button animations.
    startButtonOutAnime();
}

void KNCategorySwitcherWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    //Set the flag.
    m_pressed=true;
}

void KNCategorySwitcherWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    //Check the flag.
    if(m_pressed)
    {
        m_pressed=false;
        //Check is the user release the position in the rect.
        if(rect().contains(event->pos()))
        {
            //Means: cancel switch.
            hideSwitcher();
        }
    }
}

void KNCategorySwitcherWidget::onActionInOutBackground(const int &frame)
{
    //Change the alpha.
    m_backgroundColor.setAlpha(frame);
    //Set the background color.
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}

void KNCategorySwitcherWidget::onActionHideComplete()
{
    //Ask to restore header button
    emit requireResetHeaderButton();
    //Hide the widget.
    hide();
}

void KNCategorySwitcherWidget::initialTimeLine(QTimeLine *timeline)
{
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(5);
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNCategorySwitcherWidget::onActionInOutBackground);
}

void KNCategorySwitcherWidget::initialButtons()
{
    m_settingButton=new KNCategorySettingButton(this);
    m_settingButton->setIcon(QPixmap(":/plugin/configure/common/icon.png"));
    m_settingButton->setText(tr("Preference"));
    connect(m_settingButton, &KNCategorySettingButton::clicked,
            [=]
            {
                m_settingButton->setChecked(false);
                emit requireShowPreference();
            });
}

void KNCategorySwitcherWidget::startButtonInAnime()
{
    m_settingButton->startInAnime();
}

void KNCategorySwitcherWidget::startButtonOutAnime()
{
    m_settingButton->startOutAnime();
}

void KNCategorySwitcherWidget::stopAllAnime()
{
    m_inBackground->stop();
    m_outBackground->stop();
}
