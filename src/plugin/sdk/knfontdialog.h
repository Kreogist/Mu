/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
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
class KNFontDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KNFontDialog(QWidget *parent = 0);
    ~KNFontDialog();
    static QFont getFont(const QFont &defaultFont);
    void setCurrentFont(const QFont &font);
    QFont currentFont() const;

signals:
    void currentFontChanged(const QFont &font);

public slots:

private slots:
    void fontChanged(const int &currentIndex);
    void sizeChanged(const int &currentSize);
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
    inline void applySizeChanged(const int &currentSize);
    inline void blockFontListWidget();
    inline void releaseFontListWidget();
    inline void blockFontSizeWidget();
    inline void releaseFontSizeWidget();
    inline void blockFontStyleWidget();
    inline void releaseFontStyleWidget();
    QFont m_currentFont;
    QFontComboBox *m_fontFamily;
    QList<int> m_standardSize;
    QListWidget *m_fontSizeList;
    QLabel *m_caption[4];
    QCheckBox *m_fontStyles[FontStylesCount];
    QAbstractItemModel *m_fontModel;
    QSpinBox *m_sizeSpin;
    QSlider *m_sizeSlider;
    QLineEdit *m_previewEdit;
    QListView *m_fontList;
    QPushButton *m_okayButton, *m_cancelButton;
};

#endif // KNFONTDIALOG_H
