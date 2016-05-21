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

#ifndef KNSAOSTYLE_H
#define KNSAOSTYLE_H

#include <QCommonStyle>

class QStyleOptionMenuItem;
class QStyleOptionTab;
/*!
 * \brief The KNSaoStyle class is a special style. It will only be used in the
 * SAO styled menu for UI hacking.\n
 * Most of the SAO Style is the same as the fusion style. In fact, the SAO style
 * will only be used for a few classes.
 */
class KNSaoStyle : public QCommonStyle
{
    Q_OBJECT
public:
    /*!
     * \brief Get the style instance pointer.
     * \return The global instance pointer.
     */
    static KNSaoStyle *instance();
    ~KNSaoStyle();

    /*!
     * \brief Set the SAO style style sheet to a vertical QScrollBar.
     * \param widget The QScrollBar widget pointer.
     */
    static void styleVerticalScrollBar(QWidget *widget);

    /*!
     * \brief Set the SAO style style sheet to a horizontal QScrollBar.
     * \param widget The QScrollBar widget pointer.
     * \param brightness The brightness of the scroll bar.
     */
    static void styleHorizontalScrollBar(QWidget *widget,
                                         int brightness=100);

    /*!
     * \brief Get the fixed scroll bar width of the data.
     * \return The width of the scroll bar.
     */
    static int scrollBarWidth();

    /*!
     * \brief Reimplemented from QCommonStyle::drawControl().
     */
    void drawControl(ControlElement element,
                     const QStyleOption *opt,
                     QPainter *p,
                     const QWidget *w) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::drawPrimitive().
     */
    void drawPrimitive(PrimitiveElement pe,
                       const QStyleOption *opt,
                       QPainter *p,
                       const QWidget *w) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::drawComplexControl().
     */
    void drawComplexControl(ComplexControl cc,
                            const QStyleOptionComplex *opt,
                            QPainter *p,
                            const QWidget *w) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::sizeFromContents().
     */
    QSize sizeFromContents(ContentsType ct,
                           const QStyleOption *opt,
                           const QSize &contentsSize,
                           const QWidget *widget) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::pixelMetric().
     */
    int pixelMetric(PixelMetric metric,
                    const QStyleOption *option = 0,
                    const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::subElementRect().
     */
    QRect subElementRect(SubElement r,
                         const QStyleOption *opt,
                         const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::hitTestComplexControl().
     */
    SubControl hitTestComplexControl(ComplexControl cc,
                                     const QStyleOptionComplex *opt,
                                     const QPoint &pt,
                                     const QWidget *w = 0) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::subControlRect().
     */
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::generatedIconPixmap().
     */
    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap,
                                const QStyleOption *opt) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::styleHint().
     */
    int styleHint(StyleHint hint, const QStyleOption *option = 0,
                  const QWidget *widget = 0,
                  QStyleHintReturn *returnData = 0) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::itemPixmapRect().
     */
    QRect itemPixmapRect(const QRect &r, int flags,
                         const QPixmap &pixmap) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::standardIcon().
     */
    QIcon standardIcon(StandardPixmap standardIcon,
                       const QStyleOption *option = 0,
                       const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::standardPixmap().
     */
    QPixmap standardPixmap(StandardPixmap standardPixmap,
                           const QStyleOption *opt,
                           const QWidget *widget = 0) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::drawItemPixmap().
     */
    void drawItemPixmap(QPainter *painter, const QRect &rect,
                        int alignment, const QPixmap &pixmap) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::drawItemText().
     */
    void drawItemText(QPainter *painter, const QRect &rect,
                      int flags, const QPalette &pal, bool enabled,
                      const QString &text,
                      QPalette::ColorRole textRole = QPalette::NoRole) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::polish().
     */
    void polish(QWidget *widget) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::polish().
     */
    void polish(QApplication *app) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::polish().
     */
    void polish(QPalette &pal) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::unpolish().
     */
    void unpolish(QWidget *widget) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QCommonStyle::unpolish().
     */
    void unpolish(QApplication *app) Q_DECL_OVERRIDE;

private:
    static KNSaoStyle *m_instance;
    explicit KNSaoStyle();
    inline void drawMenuItem(const QStyleOptionMenuItem *opt,
                             QPainter *p,
                             const QWidget *w) const;
    QPixmap m_slideLeft, m_slideRight, m_slideUp, m_slideDown;
    QStyle *m_fusionStyle;
};

#endif // KNSAOSTYLE_H
