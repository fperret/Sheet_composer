#ifndef EDITPANEL_H
#define EDITPANEL_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include "config.h"

class EditPanel : public QDockWidget
{
    Q_OBJECT
public:

    EditPanel(QWidget *parent, Config *p_config);
    ~EditPanel();
    explicit EditPanel(QWidget *parent = nullptr);

signals:


private:
    static const QString m_title;

    Config *m_config;
    // If this is not a pointer program crash when a Widget added to it goes out of scope
    QGridLayout *m_editNotesLayout;

    QLabel retest;

    // will need a QScrollArea
};

#endif // EDITPANEL_H
