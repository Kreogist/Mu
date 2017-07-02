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
#ifndef KNFONTDIALOG_H
#define KNFONTDIALOG_H

#include <QList>
#include <QDialog>

class QLabel;
class QSpinBox;
class QCheckBox;
class QSlider;
class QPushButton;
class QFontComboBox;
class QListWidget;
class QLineEdit;
class QListView;
class QAbstractItemModel;
/*!
 * \brief The KNFontDialog class provide a dialog for selecting a font with its
 * settings including bold, underline, size and so on.\n
 * This dialog is one part of the common dialog rebuilt project.
 */
class KNFontDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNFontDialog widget.
     * \param parent The parent widget.
     */
    explicit KNFontDialog(QWidget *parent = 0);
    ~KNFontDialog();

    /*!
     * \brief This static font will quickly get font without initializing a font
     * dialog panel.
     * \param selectedFont
     * \param title
     * \param defaultFont
     * \return
     */
    static bool getFont(QFont &selectedFont,
                        const QString &title=QString(),
                        const QFont &defaultFont=QFont());

    /*!
     * \brief Set the font to the dialog, and let the dialog to display its
     * parameter.
     * \param font The current configuring font.
     */
    void setCurrentFont(const QFont &font);

    /*!
     * \brief Get the current configuring font.
     * \return The font which is editing.
     */
    QFont currentFont() const;

signals:
    /*!
     * \brief When the current editing font is changed, this signal will be
     * emitted.
     * \param font The editing font.
     */
    void currentFontChanged(const QFont &font);

public slots:

private slots:
    void onFontChanged(int currentIndex);
    void onSizeChanged(int currentSize);
    void synchronizeFont();

private:
    enum FontStyles
    {
        StyleBold,
        StyleItalic,
        StyleUnderline,
        StyleStrikeout,
        StyleKerning,
        FontStylesCount
    };

    inline void applyFontData();
    inline void applySizeChanged(int currentSize);
    inline void blockFontListWidget();
    inline void releaseFontListWidget();
    inline void blockFontSizeWidget();
    inline void releaseFontSizeWidget();
    inline void blockFontStyleWidget();
    inline void releaseFontStyleWidget();
    QList<int> m_standardSize;
    QFont m_currentFont;
    QLabel *m_caption[4];
    QCheckBox *m_fontStyles[FontStylesCount];
    QFontComboBox *m_fontFamily;
    QListWidget *m_fontSizeList;
    QAbstractItemModel *m_fontModel;
    QSpinBox *m_sizeSpin;
    QSlider *m_sizeSlider;
    QLineEdit *m_previewEdit;
    QListView *m_fontList;
    QPushButton *m_okayButton, *m_cancelButton;
};

#endif // KNFONTDIALOG_H
