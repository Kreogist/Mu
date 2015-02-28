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
#include <QStyleOptionMenuItem>
#include <QStyle>
#include <QStyleFactory>
#include <QPainter>
#include <QIcon>
#include <QWidget>
#include <QPalette>
#include <QColor>
#include <QMenu>
#include <QDebug>

#include "knsaostyle.h"

KNSAOStyle::KNSAOStyle()
{
    m_fusionStyle=QStyleFactory::create("fusion");
}

KNSAOStyle *KNSAOStyle::m_instance=nullptr;

KNSAOStyle *KNSAOStyle::instance()
{
    return m_instance==nullptr?m_instance=new KNSAOStyle:m_instance;
}

KNSAOStyle::~KNSAOStyle()
{
    delete m_fusionStyle;
}

void KNSAOStyle::setScrollBarStyleSheet(QWidget *widget)
{
    widget->setStyleSheet("QScrollBar:vertical {"
                          "   border: 0px solid grey;"
                          "   background: rgba(0, 0, 0, 0);"
                          "   width: 8px;"
                          "}"
                          "QScrollBar::handle:vertical {"
                          "   background: rgba(100, 100, 100);"
                          "   min-height: 10px;"
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

void KNSAOStyle::drawControl(ControlElement element,
                             const QStyleOption *opt,
                             QPainter *p,
                             const QWidget *w) const
{
    //Initial the rendering hints.
    p->setRenderHints(QPainter::Antialiasing |
                      QPainter::TextAntialiasing, true);
    switch(element)
    {
    case CE_MenuItem:
    {
        const QStyleOptionMenuItem *menuItemOption=
                qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
        if(menuItemOption != nullptr)
        {
            drawMenuItem(menuItemOption, p, w);
        }
        break;
    }
    default:
        m_fusionStyle->drawControl(element,opt,p,w);
        break;
    }
}

void KNSAOStyle::drawMenuItem(const QStyleOptionMenuItem *opt,
                              QPainter *p,
                              const QWidget *w) const
{
    switch(opt->menuItemType)
    {
    case QStyleOptionMenuItem::SubMenu:
    case QStyleOptionMenuItem::Normal:
    {
        //Ok, it's a hack-way to accomplished our design.
        QStyleOptionMenuItem tmp=*opt;
        tmp.icon=QIcon();
        tmp.palette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));

        m_fusionStyle->drawControl(CE_MenuItem, &tmp, p, w);
        int rectX=opt->rect.x(),
            rectY=opt->rect.y(),
            rectW=opt->rect.width(),
            rectH=opt->rect.height();

        if(!opt->icon.isNull())
        {
            p->drawPixmap(11,
                          rectY + (rectH - 30)/2,
                          30,
                          30,
                          opt->icon.pixmap(30, 30));
        }

        if(rectX+rectH -1 < opt->menuRect.y() + opt->menuRect.height())
        {
            p->drawLine(rectX,rectY+rectH-1,rectX+rectW,rectY+rectH-1);
        }
        break;
    }

    default:
        m_fusionStyle->drawControl(CE_MenuItem,opt,p,w);
    }
}

void KNSAOStyle::drawPrimitive(PrimitiveElement pe,
                               const QStyleOption *opt,
                               QPainter *p,
                               const QWidget *w) const
{
    m_fusionStyle->drawPrimitive(pe,opt,p,w);
}

void KNSAOStyle::drawComplexControl(ComplexControl cc,
                                    const QStyleOptionComplex *opt,
                                    QPainter *p,
                                    const QWidget *w) const
{
    m_fusionStyle->drawComplexControl(cc,opt,p,w);
}

void KNSAOStyle::drawItemText(QPainter *painter,
                              const QRect &rect,
                              int flags,
                              const QPalette &pal,
                              bool enabled,
                              const QString &text,
                              QPalette::ColorRole textRole) const
{
    m_fusionStyle->drawItemText(painter,rect,flags,pal,enabled,text,textRole);
}

QSize KNSAOStyle::sizeFromContents(ContentsType ct,
                                   const QStyleOption *opt,
                                   const QSize &contentsSize,
                                   const QWidget *widget) const
{

    switch(ct)
    {
    case CT_MenuItem:
    {
        const QStyleOptionMenuItem *menuItemOption=
                qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
        QSize menuItemSize=m_fusionStyle->sizeFromContents(ct,opt,contentsSize,widget);
        return QSize(menuItemSize.width()<210?210:menuItemSize.width(),
                     menuItemOption->menuItemType==QStyleOptionMenuItem::Separator?
                         0:
                         46);
        break;
    }
    default:
    {
        return m_fusionStyle->sizeFromContents(ct,opt,contentsSize,widget);
    }
    }
}

int KNSAOStyle::pixelMetric(PixelMetric metric,
                            const QStyleOption *option,
                            const QWidget *widget) const
{
    switch(metric)
    {
    case QStyle::PM_SmallIconSize:
        return 40;
    case QStyle::PM_ButtonIconSize:
        return 25;
    case QStyle::PM_MenuHMargin:
    case QStyle::PM_MenuVMargin:
        return 0;
    default:
        return m_fusionStyle->pixelMetric(metric,option,widget);
    }
}

QRect KNSAOStyle::subElementRect(SubElement r,
                                 const QStyleOption *opt,
                                 const QWidget *widget) const
{
    return m_fusionStyle->subElementRect(r,opt,widget);
}
QStyle::SubControl KNSAOStyle::hitTestComplexControl(ComplexControl cc,
                                                     const QStyleOptionComplex *opt,
                                                     const QPoint &pt,
                                                     const QWidget *w) const
{
    return m_fusionStyle->hitTestComplexControl(cc,opt,pt,w);
}
QRect KNSAOStyle::subControlRect(ComplexControl cc,
                                 const QStyleOptionComplex *opt,
                                 SubControl sc,
                                 const QWidget *widget) const
{
    return m_fusionStyle->subControlRect(cc,opt,sc,widget);
}
QPixmap KNSAOStyle::generatedIconPixmap(QIcon::Mode iconMode,
                                        const QPixmap &pixmap,
                                        const QStyleOption *opt) const
{
    return m_fusionStyle->generatedIconPixmap(iconMode,pixmap,opt);
}

int KNSAOStyle::styleHint(StyleHint hint,
                          const QStyleOption *option,
                          const QWidget *widget,
                          QStyleHintReturn *returnData) const
{
    return m_fusionStyle->styleHint(hint,option,widget,returnData);
}

QRect KNSAOStyle::itemPixmapRect(const QRect &r,
                                 int flags,
                                 const QPixmap &pixmap) const
{
    return m_fusionStyle->itemPixmapRect(r,flags,pixmap);
}

QIcon KNSAOStyle::standardIcon(StandardPixmap standardIcon,
                               const QStyleOption *option,
                               const QWidget *widget) const
{
    return m_fusionStyle->standardIcon(standardIcon,option,widget);
}
QPixmap KNSAOStyle::standardPixmap(StandardPixmap standardPixmap,
                                   const QStyleOption *opt,
                                   const QWidget *widget) const
{
    return m_fusionStyle->standardPixmap(standardPixmap,opt,widget);
}

void KNSAOStyle::drawItemPixmap(QPainter *painter, const QRect &rect,
                                int alignment, const QPixmap &pixmap) const
{
    return m_fusionStyle->drawItemPixmap(painter,rect,alignment,pixmap);
}

void KNSAOStyle::polish(QWidget *widget)
{
    return m_fusionStyle->polish(widget);
}

void KNSAOStyle::polish(QApplication *app)
{
    return m_fusionStyle->polish(app);
}

void KNSAOStyle::polish(QPalette &pal)
{
    return m_fusionStyle->polish(pal);
}

void KNSAOStyle::unpolish(QWidget *widget)
{
    return m_fusionStyle->unpolish(widget);
}

void KNSAOStyle::unpolish(QApplication *app)
{
    return m_fusionStyle->unpolish(app);
}
