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
#include <QWidget>
#include <QString>
#include <QStyleFactory>
#include <QPainter>
#include <QStyleOptionMenuItem>

#include "knsaostyle.h"

#include <QDebug>

KNSaoStyle *KNSaoStyle::m_instance=nullptr;

KNSaoStyle::KNSaoStyle() :
    QCommonStyle(),
    m_slideLeft(QPixmap("://public/left_arrow.png")),
    m_slideRight(QPixmap("://public/right_arrow.png")),
    m_fusionStyle(QStyleFactory::create("fusion"))
{
}

KNSaoStyle *KNSaoStyle::instance()
{
    //Construct the style instance if the style pointer is null.
    return m_instance==nullptr?m_instance=new KNSaoStyle:m_instance;
}

KNSaoStyle::~KNSaoStyle()
{
    //Remove the fusion style.
    delete m_fusionStyle;
}

void KNSaoStyle::styleVerticalScrollBar(QWidget *widget)
{
    //The widget should be a QScrollBar or inherited from QScrollBar, and it
    //should should be vertical.
    widget->setStyleSheet("QScrollBar:vertical {"
                          //Clear the border.
                          "   border: 0px solid grey;"
                          //Clear the background.
                          "   background: rgba(0, 0, 0, 0);"
                          //Set the fixed width of the scroll bar.
                          "   width: 8px;"
                          "}"
                          //Configure the handle.
                          "QScrollBar::handle:vertical {"
                          //Reset the handle color.
                          "   background: rgba(100, 100, 100);"
                          //Set the minimal height of the handle.
                          "   min-height: 10px;"
                          //Make the handle to be a rounded rectangle.
                          "   border-radius: 4px;"
                          "}"
                          "QScrollBar::add-line:vertical {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 100);"
                          "   height: 0px;"
                          "   subcontrol-position: down;"
                          "   subcontrol-origin: margin;"
                          "}"
                          "QScrollBar::sub-line:vertical {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 100);"
                          "   height: 0px;"
                          "   subcontrol-position: up;"
                          "   subcontrol-origin: margin;"
                          "}");
}

void KNSaoStyle::styleHorizontalScrollBar(QWidget *widget, int brightness)
{
    //Get the brightness string.
    QString brightnessValue=QString::number(brightness);
    //The widget should be a QScrollBar or inherited from QScrollBar, and it
    //should should be horizontal.
    widget->setStyleSheet(QString(
                          "QScrollBar:horizontal {"
                          //Clear the border.
                          "   border: 0px solid grey;"
                          //Clear the background.
                          "   background: rgba(0, 0, 0, 0);"
                          //Set the fixed width of the scroll bar.
                          "   height: 8px;"
                          "}"
                          //Configure the handle.
                          "QScrollBar::handle:horizontal {"
                          //Reset the handle color.
                          "   background: rgba(255, 255, 255, %1);"
                          //Set the minimal height of the handle.
                          "   min-width: 10px;"
                          //Make the handle to be a rounded rectangle.
                          "   border-radius: 4px;"
                          "}"
                          "QScrollBar::add-line:horizontal {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 100);"
                          "   width: 0px;"
                          "   subcontrol-position: right;"
                          "   subcontrol-origin: margin;"
                          "}"
                          "QScrollBar::sub-line:horizontal {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 100);"
                          "   width: 0px;"
                          "   subcontrol-position: left;"
                          "   subcontrol-origin: margin;"
                          "}").arg(brightnessValue));
}

int KNSaoStyle::scrollBarWidth()
{
    return 8;
}

void KNSaoStyle::drawControl(ControlElement element,
                             const QStyleOption *opt,
                             QPainter *p,
                             const QWidget *w) const
{
    //Initial the rendering hints.
    p->setRenderHints(QPainter::Antialiasing |
                      QPainter::TextAntialiasing, true);
    //Check out the element type
    switch(element)
    {
    //SAO style will repaint the menu item.
    case CE_MenuItem:
    {
        //Recast the menu style option as a menu item.
        //Check the pointer first.
        if(opt!=nullptr)
        {
            //Draw the menu item.
            drawMenuItem(qstyleoption_cast<const QStyleOptionMenuItem *>(opt),
                         p,
                         w);
        }
        break;
    }
    //For all the other controls, simply use fustion style to draw the control.
    default:
        m_fusionStyle->drawControl(element,opt,p,w);
    }
}

inline void KNSaoStyle::drawMenuItem(const QStyleOptionMenuItem *opt,
                                     QPainter *p,
                                     const QWidget *w) const
{
    //Check out the menu item type.
    switch(opt->menuItemType)
    {
    //For all the sub menu and main menu, it should be paint as SAO style.
    case QStyleOptionMenuItem::SubMenu:
    case QStyleOptionMenuItem::Normal:
    {
        //Ok, it's a hack-way to accomplished our design.
        QStyleOptionMenuItem tmp=*opt;
        //Clear the original icon.
        tmp.icon=QIcon();
        //Force the highlight color.
        tmp.palette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
        //Use fusion style to draw the control.
        m_fusionStyle->drawControl(CE_MenuItem, &tmp, p, w);
        //Get the rect position.
        int rectX=opt->rect.x(), rectY=opt->rect.y(), rectW=opt->rect.width(),
            rectH=opt->rect.height();
        //Check the original icon, if it's not null, then draw the icon.
        if(!opt->icon.isNull())
        {
            //We have to force the image size to be 30x30.
            p->drawPixmap(11,
                          rectY + ((rectH-30)>>1),
                          30,
                          30,
                          opt->icon.pixmap(30, 30));
        }
        //Check out whether is the item is the last one, if the item is not the
        //last one, we have to draw the split line.
        if(rectX+rectH -1 < opt->menuRect.y() + opt->menuRect.height())
        {
            //Draw the bottom line.
            p->drawLine(rectX,rectY+rectH-1,rectX+rectW,rectY+rectH-1);
        }
        break;
    }
    default:
        //For all the other kinds of menu item, draw the item.
        m_fusionStyle->drawControl(CE_MenuItem, opt, p, w);
    }
}

void KNSaoStyle::drawPrimitive(PrimitiveElement pe,
                               const QStyleOption *opt,
                               QPainter *p,
                               const QWidget *w) const
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->drawPrimitive(pe,opt,p,w);
}

void KNSaoStyle::drawComplexControl(ComplexControl cc,
                                    const QStyleOptionComplex *opt,
                                    QPainter *p,
                                    const QWidget *w) const
{
    //Check out the complex control type.
    if(cc==CC_ScrollBar)
    {
        //Draw the control.
        const QStyleOptionSlider *slider=
                qstyleoption_cast<const QStyleOptionSlider *>(opt);
        //Check slider first.
        if(!slider || (slider->maximum == slider->minimum))
        {
            //Failed to draw the slider.
            return;
        }
        //Save the painter.
        p->save();
        //Set the rendering hint.
        p->setRenderHints(QPainter::Antialiasing |
                          QPainter::SmoothPixmapTransform, true);
        //Calculate the parameter.
        int roundedRadius=(opt->rect.height() < opt->rect.width()?
                               opt->rect.height():
                               opt->rect.width())>>1;
        //Set the brush to painter, clear the pen.
        p->setPen(Qt::NoPen);
        //Draw the base.
        p->setBrush(opt->palette.color(QPalette::Base));
        p->drawRoundedRect(opt->rect, roundedRadius, roundedRadius);
        //Draw the button slider.
        p->setBrush(opt->palette.color(QPalette::Button));
        //Get the control rect.
        QRect sliderRect=subControlRect(CC_ScrollBar,
                                        opt,
                                        SC_ScrollBarSlider,
                                        w);
        p->drawRoundedRect(sliderRect, roundedRadius, roundedRadius);
        //Draw the button.
        // Draw left button first.
        sliderRect=subControlRect(CC_ScrollBar,
                                  opt,
                                  SC_ScrollBarAddLine,
                                  w);
        //Draw the pixmap.
        p->drawPixmap(QPoint(sliderRect.x()+
                             ((sliderRect.width()-m_slideRight.width())>>1),
                             sliderRect.y()+
                             ((sliderRect.height()-m_slideRight.height())>>1)),
                      m_slideRight);
        //Draw right button.
        sliderRect=subControlRect(CC_ScrollBar,
                                  opt,
                                  SC_ScrollBarSubLine,
                                  w);
        p->drawPixmap(QPoint(sliderRect.x()+
                             ((sliderRect.width()-m_slideLeft.width())>>1),
                             sliderRect.y()+
                             ((sliderRect.height()-m_slideLeft.height())>>1)),
                      m_slideLeft);
        //Restore the painter.
        p->restore();
        return;
    }
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->drawComplexControl(cc,opt,p,w);
}

void KNSaoStyle::drawItemText(QPainter *painter,
                              const QRect &rect,
                              int flags,
                              const QPalette &pal,
                              bool enabled,
                              const QString &text,
                              QPalette::ColorRole textRole) const
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->drawItemText(painter,rect,flags,pal,enabled,text,textRole);
}

QSize KNSaoStyle::sizeFromContents(ContentsType ct,
                                   const QStyleOption *opt,
                                   const QSize &contentsSize,
                                   const QWidget *widget) const
{
    //Check the content item type.
    switch(ct)
    {
    //For all the menu item.
    case CT_MenuItem:
    {
        //Get the original item size.
        QSize menuItemSize=m_fusionStyle->sizeFromContents(ct,
                                                           opt,
                                                           contentsSize,
                                                           widget);
        //Check out the menu item size.
        //The minimum size of the SAO style menu, it should be 210.
        return QSize(menuItemSize.width()<210 ? 210 : menuItemSize.width(),
                     //If the item is a separator, then the height will be 0.
                     qstyleoption_cast<const QStyleOptionMenuItem *>(opt)->
                                menuItemType==QStyleOptionMenuItem::Separator?
                         0:
                         46);
    }
    default:
        //For all the other widget, it should be the original one.
        return m_fusionStyle->sizeFromContents(ct,opt,contentsSize,widget);
    }
}

int KNSaoStyle::pixelMetric(PixelMetric metric,
                            const QStyleOption *option,
                            const QWidget *widget) const
{
    //Check out the metric type.
    switch(metric)
    {
    //For small icon, it should be 40.
    case QStyle::PM_SmallIconSize:
        return 40;
    //I don't know why I change this value.
    case QStyle::PM_ButtonIconSize:
        return 25;
    //Ignore the menu margin, make it 0.
    case QStyle::PM_MenuHMargin:
    case QStyle::PM_MenuVMargin:
        return 0;
    //For all the other metric, be the original one.
    default:
        return m_fusionStyle->pixelMetric(metric,option,widget);
    }
}

QRect KNSaoStyle::subElementRect(SubElement r,
                                 const QStyleOption *opt,
                                 const QWidget *widget) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->subElementRect(r,opt,widget);
}

QStyle::SubControl KNSaoStyle::hitTestComplexControl(
        ComplexControl cc,
        const QStyleOptionComplex *opt,
        const QPoint &pt,
        const QWidget *w) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->hitTestComplexControl(cc,opt,pt,w);
}

QRect KNSaoStyle::subControlRect(ComplexControl cc,
                                 const QStyleOptionComplex *opt,
                                 SubControl sc,
                                 const QWidget *widget) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->subControlRect(cc,opt,sc,widget);
}

QPixmap KNSaoStyle::generatedIconPixmap(QIcon::Mode iconMode,
                                        const QPixmap &pixmap,
                                        const QStyleOption *opt) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->generatedIconPixmap(iconMode,pixmap,opt);
}

int KNSaoStyle::styleHint(StyleHint hint,
                          const QStyleOption *option,
                          const QWidget *widget,
                          QStyleHintReturn *returnData) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->styleHint(hint,option,widget,returnData);
}

QRect KNSaoStyle::itemPixmapRect(const QRect &r,
                                 int flags,
                                 const QPixmap &pixmap) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->itemPixmapRect(r,flags,pixmap);
}

QIcon KNSaoStyle::standardIcon(StandardPixmap standardIcon,
                               const QStyleOption *option,
                               const QWidget *widget) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->standardIcon(standardIcon,option,widget);
}

QPixmap KNSaoStyle::standardPixmap(StandardPixmap standardPixmap,
                                   const QStyleOption *opt,
                                   const QWidget *widget) const
{
    //Simply called the fusion style function to get the same value.
    return m_fusionStyle->standardPixmap(standardPixmap,opt,widget);
}

void KNSaoStyle::drawItemPixmap(QPainter *painter, const QRect &rect,
                                int alignment, const QPixmap &pixmap) const
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->drawItemPixmap(painter,rect,alignment,pixmap);
}

void KNSaoStyle::polish(QWidget *widget)
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->polish(widget);
}

void KNSaoStyle::polish(QApplication *app)
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->polish(app);
}

void KNSaoStyle::polish(QPalette &pal)
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->polish(pal);
}

void KNSaoStyle::unpolish(QWidget *widget)
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->unpolish(widget);
}

void KNSaoStyle::unpolish(QApplication *app)
{
    //Simply called the fusion style function to do the same thing.
    m_fusionStyle->unpolish(app);
}
