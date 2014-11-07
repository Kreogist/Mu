#ifndef KNSEARCHBOX_H
#define KNSEARCHBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class QBoxLayout;
class QToolButton;
class QLineEdit;
class QTimeLine;
class QFocusEvent;
class KNSearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit KNSearchLineEdit(QWidget *parent = 0);

signals:
    void getFocus();
    void lostFocus();

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

private:
};

class KNSearchButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNSearchButton(QWidget *parent = 0);

private:
};

class KNSearchBox : public QWidget
{
    Q_OBJECT
public:
    explicit KNSearchBox(QWidget *parent = 0);
    void setPlaceHolderText(const QString &text);
    void setSearchFocus();
    void clear();
    QString text() const;

signals:
    void requireLostFocus();
    void editingFinished();
    void returnPressed();
    void selectionChanged();
    void textChanged(const QString & text);
    void textEdited(const QString & text);

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionBackgroundChanged(const int &frame);
    void onActionTextBackgroundChange(const int &frame);
    void onFocusGet();
    void onFocusLost();

private:
    QPalette m_palette, m_textPalette;
    KNSearchLineEdit *m_keyText;
    QBoxLayout *m_layout;
    KNSearchButton *m_button;
    QTimeLine *m_mouseEnterAnime, *m_mouseLeaveAnime, *m_focusGet, *m_focusLost;
    int m_grey, m_originalGrey, m_fontGrey;
    bool m_focus=false;
};

#endif // KNSEARCHBOX_H
