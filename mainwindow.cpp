#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QPushButton>
#include <QSize>

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

void MainWindow::createNoteWidget()
{
    QDialog l_dialog;
    l_dialog.resize(300, 50);

    QPushButton l_chooseImageButton(&l_dialog);
    l_chooseImageButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    l_chooseImageButton.ensurePolished();
    l_chooseImageButton.move(10, 10);
    l_chooseImageButton.setMinimumWidth(280);

    // Add condition if empty string on callback
    l_chooseImageButton.connect(&l_chooseImageButton, &QPushButton::clicked, this, [this]() {
        m_selectedFileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "D:/Images/", tr("Image Files (*.png *jpg *.bmp)"));
        qobject_cast<QPushButton *>(sender())->setText(m_selectedFileName.splitRef("/").last().toString());
        qobject_cast<QPushButton *>(sender())->adjustSize();
    });
    l_chooseImageButton.setText("Select an image for the note");

    // need to add OK / Cancel button
    // When we click the red cross it goes in the else
    if (l_dialog.exec()) {
        qDebug() << "In dialog exec true";
    } else {
        qDebug() << "In else";
    }
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

    QAction *l_createNoteAction = new QAction(tr("Create note"), this);
    l_createNoteAction->connect(l_createNoteAction, &QAction::triggered, this, &MainWindow::createNoteWidget);
    menuBar()->addAction(l_createNoteAction);

}

MainWindow::~MainWindow()
{
    delete ui;
}

