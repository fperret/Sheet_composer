#include "clickablelabel.h"

#include <QDebug>

/*ClickableLabel::ClickableLabel(QWidget *p_parent, Qt::WindowFlags p_flags) : QLabel(p_parent)
{

}*/
ClickableLabel::ClickableLabel(QWidget *p_parent) : QLabel(p_parent)
{

}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent *p_event)
{
    qDebug() << "Label " << objectName() << " clicked";
    (void)p_event;
    qDebug() << "Size : " << size();
    emit clicked();
}
