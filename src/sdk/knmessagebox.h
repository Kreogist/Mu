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

    /*!
     * \brief Show up the message box, and it could provide a line edit box to
     * get some sample input data from user. It's good for asking some basic
     * information, like file name. It provides a hint text label and it could
     * display the hint text.
     * \param text The hint text. It will be display on the message box.
     * \param title The message box title.
     * \param originalText The original text. Leave it blank if you don't have
     * the raw text.
     * \return If the okay button is clicked, it will return the new text in the
     * line edit box. Or else it will return a null QString object. Check it
     * with isEmpty() function of the QString.
     */
    static QString getText(const QString &text,
                           const QString &title,
                           const QString &originalText=QString());

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
     * \param autoDelete Automatically recover the memory of the widget. The
     * widget at the pointer will be recovered after the message box delete.\n
     * If the auto delete is false, the parent of the widget will be nullptr.
     * The widget will be hide as well.
     */
    void setContentWidget(QWidget *widget, bool autoDelete=true);

    /*!
     * \brief Set whether the dialog show the cancel button. Sometimes you may
     * only need the cancel button.
     * \param showOkayButton To make the okay button visible, set it to true. It
     * is true as default.
     */
    void setOkayButtonVisible(bool okayButtonVisible);

    /*!
     * \brief Set whether the dialog show the cancel button. The dialog won't
     * need to display the cancel button at some time, like message box.
     * \param showCancelButton To make the cancel button visible, set it to
     * true. It is false as default.
     */
    void setCancelButtonVisible(bool showCancelButton);

    /*!
     * \brief Resize the size of the message dialog box.
     * \param contentSize The new size of the content. Message box will
     * calculate the message box size to fit the content.
     */
    void resizeDialog(const QSize &contentSize);

protected:
    /*!
     * \brief Reimplemented from QDialog::showEvent().
     */
    void showEvent(QShowEvent *event);

    /*!
     * \brief Reimplemented from QDialog::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented from QDialog::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented from QDialog::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
     * \brief This function is used to judge is it okay to accept the result.
     * It will be called after user clicked okay button. Only when this function
     * return true the dialog will be closed and set the value to accept.\n
     * It will return true as default.
     * \return True if it's okay to accept the data user preferred.
     */
    virtual bool okayPressed();

    /*!
     * \brief This function will be called after user clicked cancel button. But
     * this function cannot stop the close event.
     */
    virtual void cancelPressed();

    /*!
     * \brief Set the visiblility of okay or cancel button.
     * \param isOkayButton If this is true, the visibility to okay button will
     * changed.
     * \param isVisible If this is true, button will be shown, or else hidden.
     */
    void setButtonVisible(bool isOkayButton, bool isVisible);

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
    bool m_showCancelButton, m_showOkayButton;

    //Animations.
    QSequentialAnimationGroup *m_showAnime;
    QParallelAnimationGroup *m_hideAnime;
    QPropertyAnimation *m_zoomIn, *m_fadeIn, *m_expand, *m_fold, *m_fadeOut,
                       *m_dialogResizing;

    //Move elements.
    bool m_pressed;
    QPoint m_referencePoint;
};

#endif // KNMESSAGEBOX_H
