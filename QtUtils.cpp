#include "QtUtils.h"

void deleteWidgetsFromLayout(QLayout *p_layout)
{
    if (!p_layout) {
        qWarning("Cannot delete widgets from layout because layout is NULL");
        return ;
    }
    qWarning() << "Will delete all widgets from layout  " << p_layout->objectName();
    QLayoutItem *l_item;
    while ((l_item = p_layout->takeAt(0)) != NULL)
    {
        delete l_item->widget();
        delete l_item;
    }
}

// Could be templated to return specific subclasses of QLayout ?
QLayout *getLayoutOfParent(const QWidget *p_widget)
{
    const QWidget *l_parentWidget = qobject_cast<const QWidget *>(p_widget->parent());
    if (l_parentWidget == nullptr) {
        qWarning("Parent of QWidget is not a QWidget");
        return nullptr;
    }

    QLayout *l_parentLayout = qobject_cast<QLayout *>(l_parentWidget->layout());
    if (l_parentLayout == nullptr) {
        qWarning("Parent of QWidget does not have a QLayout");
        return nullptr;
    } else
        return l_parentLayout;
}

/*
 * @p_widgetRow, @p_widgetCol, @p_parentLayoutFound are only set if this function returns true.
 * @p_expectedParentLayout is a weird parameter because we basically give the parent layout and then we expect to get the parent layout in @p_parentLayoutFound
 * But it does not cost much as an extra security check while we are not confident on the global architecture of the layouts and widgets.
 *
 * @p_widget                [in] Cannot be defined const because of QLayout::indexOf()
 * @p_expectedParentLayout  [in] If != nullptr, will be compared to the found parent layout to help make sure @p_widget is parented the way we think
 * @p_widgetRow             [out] Row of @p_widget in its parent's QGridLayout
 * @p_widgetCol             [out] Col of @p_widget in its parent's QGridLayout
 * @p_parentLayoutFound     [out] @p_wdiget Parent's QGridLayout found, set for convenience
 * @return  False if any of the step to get @p_widgetRow and @p_widgetCol failed, else true
 */
bool getPosOfWidgetInGridLayout(QWidget *p_widget, const QGridLayout *p_expectedParentLayout, int &p_widgetRow, int &p_widgetCol, QGridLayout *&p_parentLayoutFound)
{
    QLayout *l_widgetParentLayout = getLayoutOfParent(p_widget);
    if (l_widgetParentLayout == nullptr
    || ((p_expectedParentLayout != nullptr) && (l_widgetParentLayout != p_expectedParentLayout))) {
        qWarning() << "Parent's layout of QWidget " << p_widget->objectName() << " not found or it does not match the one expected";
        return false;
    }

    // Why don't you take a const * :(
    const int l_indexOfImageInLayout = l_widgetParentLayout->indexOf(p_widget);
    if (l_indexOfImageInLayout < 0) {
        qWarning() << "Index of QWidget " << p_widget->objectName() << " could not be found in its parent's layout";
        return false;
    }

    QGridLayout *l_gridParentLayout = qobject_cast<QGridLayout *>(l_widgetParentLayout);
    if (l_gridParentLayout == nullptr) {
        qWarning() << "Parent's layout of QWidget " << p_widget->objectName() << " is not of type QGridLayout *";
        return false;
    }
    // Dummy value
    int _;
    int l_row = -1;
    int l_col = -1;
    l_gridParentLayout->getItemPosition(l_indexOfImageInLayout, &l_row, &l_col, &_, &_);
    // Should not happen since the index we provide is the one we found just before
    if (l_row == -1 || l_col == -1) {
        qWarning() << "Row and Column could not be found for QWidget " << p_widget->objectName();
        return false;
    }

    p_widgetRow = l_row;
    p_widgetCol = l_col;
    p_parentLayoutFound = l_gridParentLayout;
    return true;
}

/*
 * Overload of getPosOfWidgetInGridLayout when we do not need the parent's layout that has been found.
 * @p_widgetRow, @p_widgetCol are only set if this function returns true
 *
 * @p_widget                [in] Cannot be defined const because of QLayout::indexOf()
 * @p_expectedParentLayout  [in] If != nullptr, will be compared to the found parent layout to help make sure @p_widget is parented the way we think
 * @p_widgetRow             [out] Row of @p_widget in its parent's QGridLayout
 * @p_widgetCol             [out] Col of @p_widget in its parent's QGridLayout
 * @return  False if any of the step to get @p_widgetRow and @p_widgetCol failed, else true
 */
bool getPosOfWidgetInGridLayout(QWidget *p_widget, const QGridLayout *p_expectedParentLayout, int &p_widgetRow, int &p_widgetCol)
{
    // Dummy value
    QGridLayout *_ = nullptr;
    return getPosOfWidgetInGridLayout(p_widget, p_expectedParentLayout, p_widgetRow, p_widgetCol, _);
}

/*
 * Overload of getPosOfWidgetInGridLayout when we do not know the expected parent and we do not get its pointer.
 * @p_widgetRow, @p_widgetCol, are only set if this function returns true
 *
 * @p_widget                [in] Cannot be defined const because of QLayout::indexOf()
 * @p_widgetRow             [out] Row of @p_widget in its parent's QGridLayout
 * @p_widgetCol             [out] Col of @p_widget in its parent's QGridLayout
 * @return  False if any of the step to get @p_widgetRow and @p_widgetCol failed, else true
 */
bool getPosOfWidgetInGridLayout(QWidget *p_widget, int &p_widgetRow, int &p_widgetCol)
{
     // Dummy value
    QGridLayout *_ = nullptr;
    return getPosOfWidgetInGridLayout(p_widget, nullptr, p_widgetRow, p_widgetCol, _);
}

/*
 * Overload of getPosOfWidgetInGridLayout when we do not know the expected parent but want to get its pointer.
 * @p_widgetRow, @p_widgetCol, @p_parentLayoutFound are only set if this function returns true
 *
 * @p_widget                [in] Cannot be defined const because of QLayout::indexOf()
 * @p_widgetRow             [out] Row of @p_widget in its parent's QGridLayout
 * @p_widgetCol             [out] Col of @p_widget in its parent's QGridLayout
 * @p_parentLayoutFound     [out] @p_wdiget Parent's QGridLayout found, set for convenience
 * @return  False if any of the step to get @p_widgetRow and @p_widgetCol failed, else true
 */
bool getPosOfWidgetInGridLayout(QWidget *p_widget, int &p_widgetRow, int &p_widgetCol, QGridLayout *&p_parentLayoutFound)
{
    return getPosOfWidgetInGridLayout(p_widget, nullptr, p_widgetRow, p_widgetCol, p_parentLayoutFound);
}
