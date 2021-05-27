#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>
#include <QPushButton>
#include <QSize>
#include <QLayout>

#include "Utility.h"
#include "lineedit.h"
#include "centralwidget.h"

void MainWindow::addNewNote()
{
    QAction *l_caller = qobject_cast<QAction *>(sender());

    QVariant l_value = l_caller->data();

}

void MainWindow::increaseSheetNoteWidth()
{
    m_config.increaseSheetNoteWidth();
    emit redrawNeeded();
}

void MainWindow::createToolBar()
{
    // toolbar is a line below the menuBar with icons
    QToolBar *l_fileToolBar = addToolBar(("File"));

    l_fileToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    const QIcon l_expandWidthIcon = QIcon(":/images/expand-width.png");
    QAction *l_expandWidthAction = new QAction(l_expandWidthIcon, tr("Expand width"), this);
    l_fileToolBar->addAction(l_expandWidthAction);
    connect(l_expandWidthAction, &QAction::triggered, this, &MainWindow::increaseSheetNoteWidth);

    const QIcon l_reduceWidthIcon = QIcon(":/images/reduce-width.png");
    QAction *l_reduceWidthAction = new QAction(l_reduceWidthIcon, tr("Reduce width"), this);
    l_fileToolBar->addAction(l_reduceWidthAction);
    connect(l_reduceWidthAction, &QAction::triggered, &m_config, &Config::decreaseSheetNoteWidth);

    const QIcon l_expandHeightIcon = QIcon(":/images/expand-height.png");
    QAction *l_expandHeightAction = new QAction(l_expandHeightIcon, tr("Expand height"), this);
    l_fileToolBar->addAction(l_expandHeightAction);
    connect(l_expandHeightAction, &QAction::triggered, &m_config, &Config::increaseSheetNoteHeight);

    const QIcon l_reduceHeightIcon = QIcon(":/images/reduce-height.png");
    QAction *l_reduceHeightAction = new QAction(l_reduceHeightIcon, tr("Reduce height"), this);
    l_fileToolBar->addAction(l_reduceHeightAction);
    connect(l_reduceHeightAction, &QAction::triggered, &m_config, &Config::decreaseSheetNoteHeight);
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
    ((CentralWidget *)centralWidget())->loadSheetFromJson(l_root);
}

/*
 * Create a dialog to select an image so that it becomes available as a note to add.
 */
void MainWindow::createNoteWidget()
{
    QDialog l_dialog;
    QVBoxLayout *l_verticalLayout = new QVBoxLayout();

    l_dialog.resize(300, 100);
    l_dialog.setLayout(l_verticalLayout);

    QPushButton l_chooseImageButton;
    l_verticalLayout->addWidget(&l_chooseImageButton);
    l_chooseImageButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    l_chooseImageButton.ensurePolished();
    l_chooseImageButton.move(10, 10);
    l_chooseImageButton.setMinimumWidth(280);
    l_chooseImageButton.setText("Select an image for the note");
    QString l_filePath;
    l_chooseImageButton.connect(&l_chooseImageButton, &QPushButton::clicked, this, [this, &l_filePath]() {
        QString l_selectFile = QFileDialog::getOpenFileName(this, tr("Open Image"), "D:/Images/", tr("Image Files (*.png *jpg *.bmp)"));
        if (l_selectFile.size() != 0) {
            l_filePath = l_selectFile;
            qobject_cast<QPushButton *>(sender())->setText(l_filePath.splitRef("/").last().toString());
            qobject_cast<QPushButton *>(sender())->adjustSize();
        }
    });

    // Note : value of note could be a string
    LineEdit l_lineEdit;
    l_verticalLayout->addWidget(&l_lineEdit);
    l_lineEdit.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    l_lineEdit.ensurePolished();
    l_lineEdit.setInputMask("dd");
    l_lineEdit.setPlaceholderText("(Optional) Note value (1..99)");

    QDialogButtonBox l_buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &l_dialog);
    l_verticalLayout->addWidget(&l_buttonBox);
    l_verticalLayout->setAlignment(&l_buttonBox, Qt::AlignRight | Qt::AlignBottom);
    connect(&l_buttonBox, &QDialogButtonBox::accepted, &l_dialog, &QDialog::accept);
    connect(&l_buttonBox, &QDialogButtonBox::rejected, &l_dialog, &QDialog::reject);

    if (l_dialog.exec()) {
        if (l_lineEdit.text().isEmpty())
            m_config.addNote(l_filePath);
        else
            m_config.addNote(l_lineEdit.text().toUInt(), l_filePath);
        saveCurrentInstrument();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_config(this),
      ui(new Ui::MainWindow),
      m_currentSheetPath("../toto.json")
{
    ui->setupUi(this);

    createToolBar();

    // & in front of the string creates a keyboard shortcut linked to the first letter
    // menuBar is the menu at the very top of the window
    QMenu *fileMenu = menuBar()->addMenu("&File");

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

    QAction *l_settingsAction = new QAction(tr("Settings"), this);
    l_settingsAction->connect(l_settingsAction, &QAction::triggered, this, &MainWindow::editSettings);
    menuBar()->addAction(l_settingsAction);

    loadNotesForInstrument("../instruments.json");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveCurrentInstrument(void) const
{
    QJsonObject l_root;
    QJsonObject l_jsonNotes;
    for (auto l_note = m_config.getNotes().constKeyValueBegin(); l_note != m_config.getNotes().constKeyValueEnd(); ++l_note) {
        l_jsonNotes.insert(QString::number(l_note->first), l_note->second);
    }
    // TODO : replace with actual current instrument
    l_root["instrument"] = l_jsonNotes;
    if (!saveJsonObject(l_root, "../instruments.json"))
        return ;
}

void MainWindow::loadNotesForInstrument(const std::string &p_instrumentPath)
{
    QJsonObject l_root;
    if (!loadJsonObjectFromFile(l_root, p_instrumentPath)) {
        qWarning("Failed to load instrument json");
        return ;
    }
    QVector<QString> l_notesPaths;
    QMap<uint, QString> l_notes;
    // TODO : get from actual current instrument
    QJsonObject l_jsonNotes = l_root["instrument"].toObject();
    for (auto l_jsonKey : l_jsonNotes.keys()) {
        l_notes[l_jsonKey.toUInt()] = l_jsonNotes.value(l_jsonKey).toString();
    }
    qDebug() << "Loaded notes : " << l_notes;
    m_config.setNotes(l_notes);
}

void MainWindow::editSettings()
{

}
