#ifndef EDITPANEL_H
#define EDITPANEL_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

#include "config.h"

class MainWindow;
class CentralWidget;

// This is used to restrain access of some functions (initialize()) of EditPanel to only some classes (MainWindow)
class EditPanelAccessKey {
    friend class MainWindow;
private:
    EditPanelAccessKey() {}
};

class EditPanel : public QDockWidget
{
    Q_OBJECT
public:

    EditPanel(QWidget *parent, Config *p_config, const CentralWidget *p_centralWidget);
    ~EditPanel();
    explicit EditPanel(QWidget *parent = nullptr);

    // This should only be called by MainWindow
    // We cannot start the initialization in the constructor because the config will not be ready yet
    void initialize(EditPanelAccessKey);

signals:

private:
    const size_t NOTE_CHOICES_MAX_COLUMNS = 2;

    const CentralWidget    *m_centralWidget;
    Config *m_config;
    QScrollArea *m_scrollArea;
    // If this is not a pointer program crash when a Widget added to it goes out of scope
    QGridLayout *m_editNotesLayout;


    QLabel retest;

    // will need a QScrollArea
};

#endif // EDITPANEL_H
