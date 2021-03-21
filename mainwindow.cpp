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

#include <QLayout>

/*
 * Create a dialog to select an image so that it becomes available as a note to add.
 */
void MainWindow::createNoteWidget()
{
    QDialog l_dialog;
    l_dialog.resize(300, 100);

    QPushButton l_chooseImageButton(&l_dialog);
    l_chooseImageButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    l_chooseImageButton.ensurePolished();
    l_chooseImageButton.move(10, 10);
    l_chooseImageButton.setMinimumWidth(280);

    QString l_filePath;
    l_chooseImageButton.connect(&l_chooseImageButton, &QPushButton::clicked, this, [this, &l_filePath]() {
        QString l_selectFile = QFileDialog::getOpenFileName(this, tr("Open Image"), "D:/Images/", tr("Image Files (*.png *jpg *.bmp)"));
        if (l_selectFile.size() != 0) {
            l_filePath = l_selectFile;
            qobject_cast<QPushButton *>(sender())->setText(l_filePath.splitRef("/").last().toString());
            qobject_cast<QPushButton *>(sender())->adjustSize();
        }
    });
    l_chooseImageButton.setText("Select an image for the note");

    QDialogButtonBox l_buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &l_dialog);
    connect(&l_buttonBox, &QDialogButtonBox::accepted, &l_dialog, &QDialog::accept);
    connect(&l_buttonBox, &QDialogButtonBox::rejected, &l_dialog, &QDialog::reject);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    l_dialog.setLayout(verticalLayout);
    verticalLayout->addWidget(&l_buttonBox);
    verticalLayout->setAlignment(&l_buttonBox, Qt::AlignRight | Qt::AlignBottom);

    if (l_dialog.exec()) {
        m_notesPaths.push_back(l_filePath);
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

