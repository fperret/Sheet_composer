#include "clickablelabel.h"

/*ClickableLabel::ClickableLabel(QWidget *p_parent, Qt::WindowFlags p_flags) : QLabel(p_parent)
{

}*/
ClickableLabel::ClickableLabel() : QLabel()
{

}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent *p_event)
{
    emit clicked();
}
