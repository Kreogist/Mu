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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNPREFERENCELANGUAGEPANELITEM_H
#define KNPREFERENCELANGUAGEPANELITEM_H

#include <QAbstractButton>

class QTimeLine;
class KNPreferenceLanguagePanelItem : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferenceLanguagePanelItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceLanguagePanelItem(QWidget *parent = 0);

    /*!
     * \brief languageIcon
     * \return
     */
    QPixmap languageIcon() const;

    /*!
     * \brief languageName
     * \return
     */
    QString languageName() const;

signals:

public slots:
    /*!
     * \brief setLanguageName
     * \param languageName
     */
    void setLanguageName(const QString &languageName);

    /*!
     * \brief setLanguageIcon
     * \param languageIcon
     */
    void setLanguageIcon(const QPixmap &languageIcon);

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionChangeHighlight(const int &frame);

private:
    inline void startAnime(const int &endFrame);
    QLinearGradient m_highlight;
    QPixmap m_languageIcon;
    QString m_languageName;
    QTimeLine *m_mouseAnime;
    qreal m_highLightOpacity;
    int m_languageIndex;
};

#endif // KNPREFERENCELANGUAGEPANELITEM_H
