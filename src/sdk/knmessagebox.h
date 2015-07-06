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

#ifndef KNMESSAGEBOX_H
#define KNMESSAGEBOX_H

#include <QDialog>

class QSequentialAnimationGroup;
class QParallelAnimationGroup;
class QPropertyAnimation;
class KNMessageBoxBlock;
class KNMessageBoxContent;
class KNOpacityAnimeButton;
class KNMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit KNMessageBox(QWidget *parent = 0);

    QString titleText() const;
    void setTitleText(const QString &titleText);

signals:

public slots:

protected:
    void showEvent(QShowEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    virtual bool okayPressed();
    virtual void cancelPressed();

private slots:
    void onActionShowFinished();
    void onActionOkayClicked();
    void onActionCancelClicked();

private:
    inline void initialBlocks();
    inline void initialAnimes();
    inline void configureElements();
    inline KNOpacityAnimeButton *generateButton(const QString &iconPath);
    void startCloseAnime();
    //UI elements.
    KNMessageBoxBlock *m_topBlock, *m_bottomBlock;
    KNMessageBoxContent *m_content;
    KNOpacityAnimeButton *m_okayButton, *m_cancelButton;

    //Animations.
    QSequentialAnimationGroup *m_showAnime;
    QParallelAnimationGroup *m_hideAnime;
    QPropertyAnimation *m_zoomIn, *m_fadeIn, *m_expand, *m_fold, *m_fadeOut;

    //Move elements.
    bool m_pressed;
    QPoint m_referencePoint;
};

#endif // KNMESSAGEBOX_H
