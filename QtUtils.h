#ifndef QTUTILS_H
#define QTUTILS_H

#include <Qt>
#include <QLayout>
#include <QWidget>

void deleteWidgetsFromLayout(QLayout *p_layout);
const QLayout *getLayoutOfParent(const QWidget *p_widget);

#endif // QTUTILS_H
