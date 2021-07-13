#include "editpanel.h"
#include <QDebug>
#include <QLabel>

const QString EditPanel::m_title = "Edit Note";

EditPanel::EditPanel(QWidget *parent, Config *p_config) : QDockWidget(m_title, parent), m_config(p_config), m_editNotesLayout(new QGridLayout(this))
{
    QLabel test(this);
    test.setText("toto");
    test.setLayout(m_editNotesLayout);


}

EditPanel::~EditPanel()
{
    qDebug() << "Destructor called";
}
