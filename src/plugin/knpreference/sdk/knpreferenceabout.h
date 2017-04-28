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

#ifndef KNPREFERENCEABOUT_H
#define KNPREFERENCEABOUT_H

#include <QWidget>

class QLabel;
class QTextEdit;
class KNImageLabel;
class KNLabelButton;
/*!
 * \brief The KNPreferenceAbout class is a simple class which provides the
 * version information and some other links about this product.
 */
class KNPreferenceAbout : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNPreferenceAbout widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceAbout(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionShowUpdate();

private:
    enum ContentIndex
    {
        Title,
        Copyright,
        ContentIndexCount
    };

    QLabel *m_textContent[ContentIndexCount];
    KNImageLabel *m_iconContent;
    KNLabelButton *m_checkUpdate;
    QTextEdit *m_otherText;
};

#endif // KNPREFERENCEABOUT_H
