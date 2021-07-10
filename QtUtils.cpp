#include "QtUtils.h"

void deleteWidgetsFromLayout(QLayout *p_layout)
{
    if (!p_layout) {
        qWarning("Cannot delete widgets from layout because layout is NULL");
        return ;
    }

    QLayoutItem *l_item;
    while ((l_item = p_layout->takeAt(0)) != NULL)
    {
        delete l_item->widget();
        delete l_item;
    }
}

// Could be templated to return specific subclasses of QLayout ?
const QLayout *getLayoutOfParent(const QWidget *p_widget)
{
    const QWidget *l_parentWidget = qobject_cast<const QWidget *>(p_widget->parent());
    if (l_parentWidget == nullptr) {
        qWarning("Parent of QWidget is not a QWidget");
        return nullptr;
    }

    const QLayout *l_parentLayout = qobject_cast<const QLayout *>(l_parentWidget->layout());
    if (l_parentLayout == nullptr) {
        qWarning("Parent of QWidget does not have a QLayout");
        return nullptr;
    } else
        return l_parentLayout;
}
