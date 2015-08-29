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
/*!
 * \brief The KNMessageBox class provides a SAO styled animated dialog. It can
 * give out two kinds of result: Accept or Reject. You can set a content widget
 * to this dialog.
 */
class KNMessageBox : public QDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMessageBox class.
     * \param parent The parent widget.
     */
    explicit KNMessageBox(QWidget *parent = 0);

    /*!
     * \brief Get the title text.
     * \return The title text.
     */
    QString titleText() const;

    /*!
     * \brief Whether the cancel button is visible.
     * \return If the cancel button is visible, it will be true. Or else false.
     */
    bool showCancelButton() const;

    /*!
     * \brief Show up information using a KNMessageBox.
     * \param text The text content of the message box.
     * \param title The title of the message box. If you leave it empty, it will
     * be "Message".
     * \param alignment The alignment of the text message.
     */
    static void information(const QString &text,
                            const QString &title=QString("Message"),
                            const Qt::Alignment &alignment=Qt::AlignLeft);

signals:

public slots:
    /*!
     * \brief Set the title text.
     * \param titleText The prefer title text.
     */
    void setTitleText(const QString &titleText);

    /*!
     * \brief Set the content widget.
     * \param widget The widget pointer.
     */
    void setContentWidget(QWidget *widget);

    /*!
     * \brief Set whether the dialog show the cancel button. The dialog won't
     * need to display the cancel button at some time, like message box.
     * \param showCancelButton To make the cancel button visible, set it to
     * true.
     */
    void setShowCancelButton(bool showCancelButton);

protected:
    /*!
     * \brief Reimplement from QDialog::showEvent().
     */
    void showEvent(QShowEvent *event);

    /*!
     * \brief Reimplement from QDialog::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplement from QDialog::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
     * \brief Reimplement from QDialog::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
     * \brief This function is used to judge is it okay to accept the result.
     * It will be called after user clicked okay button. Only when this function
     * return true the dialog will be closed and set the value to accept.\n
     * It will return true as default.
     * \return True if it's okay to accept the data user prefered.
     */
    virtual bool okayPressed();

    /*!
     * \brief This funciton will be called after user clicked cancel button. But
     * this function cannot stop the close event.
     */
    virtual void cancelPressed();

private slots:
    void onActionZoomFinished();
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
    QString m_titleText;
    bool m_showCancelButton;

    //Animations.
    QSequentialAnimationGroup *m_showAnime;
    QParallelAnimationGroup *m_hideAnime;
    QPropertyAnimation *m_zoomIn, *m_fadeIn, *m_expand, *m_fold, *m_fadeOut;

    //Move elements.
    bool m_pressed;
    QPoint m_referencePoint;
};

#endif // KNMESSAGEBOX_H
