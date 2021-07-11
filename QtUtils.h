#ifndef QTUTILS_H
#define QTUTILS_H

#include <Qt>
#include <QLayout>
#include <QWidget>
#include <QDebug>

void deleteWidgetsFromLayout(QLayout *p_layout);
QLayout *getLayoutOfParent(const QWidget *p_widget);
bool getPosOfWidgetInGridLayout(QWidget *p_widget, const QGridLayout *p_expectedParentLayout, int &p_widgetRow, int &p_widgetCol, QGridLayout *&p_parentLayoutFound);
bool getPosOfWidgetInGridLayout(QWidget *p_widget, const QGridLayout *p_expectedParentLayout, int &p_widgetRow, int &p_widgetCol);
bool getPosOfWidgetInGridLayout(QWidget *p_widget, int &p_widgetRow, int &p_widgetCol);
bool getPosOfWidgetInGridLayout(QWidget *p_widget, int &p_widgetRow, int &p_widgetCol, QGridLayout *&p_parentLayoutFound);
#endif // QTUTILS_H
