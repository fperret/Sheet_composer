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
