#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit();
protected:
    void mousePressEvent(QMouseEvent *p_event) override
    {
        QLineEdit::mousePressEvent(p_event);
        setCursorPosition(0);
    }
};

#endif // LINEEDIT_H
