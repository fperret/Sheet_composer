#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>

#include "Utility.h"

#include "centralwidget.h"

void MainWindow::addNewNote()
{
    QAction *l_caller = qobject_cast<QAction *>(sender());

    QVariant l_value = l_caller->data();

}

void MainWindow::createToolBar()
{
    // toolbar is a line below the menuBar with icons
    QToolBar *l_fileToolBar = addToolBar(("File"));

    l_fileToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);

    QAction *l_testAction = new QAction("toto");
    l_testAction->setData(1);
    connect(l_testAction, &QAction::triggered, this, &MainWindow::addNewNote);

    l_fileToolBar->addAction(l_testAction);

    /*const QIcon testIcon = QIcon

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAction = new QAction(newIcon, tr("&New"), this);
    l_fileToolBar->addAction(newAction);*/

}

bool MainWindow::saveSheet() const
{
    QJsonObject l_root;

    ((CentralWidget *)centralWidget())->serializeSheet(l_root);
    saveJsonObject(l_root, m_currentSheetPath);

    return true;
}

void MainWindow::openSheet()
{
    QJsonObject l_root;

    loadJsonObjectFromFile(l_root, m_currentSheetPath);
    ((CentralWidget *)centralWidget())->unserializeSheet(l_root);
    ((CentralWidget *)centralWidget())->createSheetDisplay();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_currentSheetPath("../toto.json")
{
    ui->setupUi(this);

    createToolBar();

    // & in front of the string creates a keyboard shortcut linked to the first letter
    // menuBar is the menu at the very top of the window
    QMenu *fileMenu = menuBar()->addMenu("&Filefe");

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAction = new QAction(newIcon, tr("&New"), this);

    fileMenu->addAction(newAction);

    QAction *l_saveAction = new QAction(tr("&Save"), this);
    l_saveAction->connect(l_saveAction, &QAction::triggered, this, &MainWindow::saveSheet);
    menuBar()->addAction(l_saveAction);

    QAction *l_openAction = new QAction(tr("&Open"), this);
    l_saveAction->connect(l_openAction, &QAction::triggered, this, &MainWindow::openSheet);
    menuBar()->addAction(l_openAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}

