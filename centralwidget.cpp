#include "centralwidget.h"

#include <QApplication>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLayout>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPushButton>

#include <QDialog>

#include <QJsonArray>
#include <QJsonValue>

#include "clickablelabel.h"
#include "QtUtils.h"

class MainWindow;

CentralWidget::CentralWidget(MainWindow *p_mainWindow, QWidget *parent) : QWidget(parent)
{
    connect(p_mainWindow, &MainWindow::resizeNeeded, this, &CentralWidget::resizeNotesDisplay);
    m_config = &p_mainWindow->m_config;
    m_baseLayout = new QGridLayout(this);

    m_imageSelected = nullptr;
    m_imageAdd = nullptr;

    m_maxColumns = 5;
    m_maxRows = 5;
    m_currentLastColumn = 0;
    m_currentLastRow = 0;

    m_baseLayout->setSpacing(1);
    for (int i = 0; i < m_maxColumns; ++i) {
        m_baseLayout->setColumnStretch(i, 0);
        //m_baseLayout->setColumnMinimumWidth(i, 100);
    }
    for (int i = 0; i < m_maxRows; ++i) {
        m_baseLayout->setRowStretch(i, 0);
        //m_baseLayout->setRowMinimumHeight(i, 600);
    }

    placeAddImage();
}

void CentralWidget::setConfig(Config *p_config)
{
    m_config = p_config;
}

void CentralWidget::addWidgetInLastPos(QGridLayout *p_layout, QWidget *p_widget)
{
    p_layout->addWidget(p_widget, m_currentLastRow, m_currentLastColumn);

    // This needs to stay before addWidget because we use these values to set the "Add" image
    if (m_currentLastColumn == m_maxColumns - 1) {
        m_currentLastColumn = 0;
        m_currentLastRow += 1;
    } else {
        m_currentLastColumn += 1;
    }
}

void CentralWidget::placeAddImage()
{
    // When we clear the sheet display we delete all widgets on it including this one
    // So we need to recreate it
    if (!m_imageAdd) {
        QPixmap l_addImage(ADD_IMAGE_PATH);
        m_imageAdd = new ClickableLabel(this);
        m_imageAdd->setPixmap(l_addImage);
        m_imageAdd->setStyleSheet("QLabel { background-color : red; }");
        connect(m_imageAdd, &ClickableLabel::clicked, this, &CentralWidget::addNotePopup);
    }
    m_imageAdd->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_baseLayout->addWidget(m_imageAdd, m_currentLastRow, m_currentLastColumn);
    m_imageAdd->resize(m_config->getSheetNoteWidth(), m_config->getSheetNoteHeight());
}

void CentralWidget::drawNoteToSheet(const uint &p_noteVal)
{
    const QString &l_imagePath = m_config->getNotes()[p_noteVal];
    QPixmap l_image(l_imagePath);

    ClickableLabel *l_imageLabel = new ClickableLabel(this);
    l_imageLabel->setPixmap(l_image);
    l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
    //l_imageLabel->setScaledContents(true);
    l_imageLabel->setScaledContents(false);

    qDebug() << "rowCount : " << m_baseLayout->rowCount() << " / columnCount " << m_baseLayout->columnCount();

    addWidgetInLastPos(m_baseLayout, l_imageLabel);
    l_imageLabel->resize(m_config->getSheetNoteWidth(), m_config->getSheetNoteHeight());
    // fixed size ca fonctionne mais il faut modifier la fixed size a chaque fois + ce n'est plus responsive
    //l_imageLabel->setFixedSize(QSize(100, 600));
    //l_imageLabel->setMinimumHeight(600);

    connect(l_imageLabel, &ClickableLabel::clicked, this, &CentralWidget::imageClicked);
}

void CentralWidget::imageClicked()
{
    //m_imageSelected = qobject_cast<ClickableLabel *>(sender());
}

void CentralWidget::changeNoteValue()
{
    if (!m_imageSelected)
        return ;

    *m_imageSelected = 1;
}

void CentralWidget::logCurrentNotes() const
{
    std::string l_log;
    for (auto l_note : m_notes) {
        l_log += std::to_string(*l_note) + "|";
    }
    qDebug() << "Current notes : " << l_log.c_str();
}

void CentralWidget::initSheetDisplay()
{
    m_currentLastColumn = 0;
    m_currentLastRow = 0;
    deleteWidgetsFromLayout(m_baseLayout);
    m_imageAdd = nullptr;
}

void CentralWidget::loadSheetFromJson(const QJsonObject &p_jsonIn)
{
    unserializeSheet(p_jsonIn);
    initSheetDisplay();
    createSheetDisplay();
}

void CentralWidget::createSheetDisplay()
{
    for (auto l_note : m_notes) {
        drawNoteToSheet(*l_note);
    }
    placeAddImage();
    resizeNotesDisplay();
}

/*
 * For now this will cause widgets other than the ones for the notes to be resized
 */
void CentralWidget::resizeNotesDisplay()
{
    for (int i = 0; i < m_baseLayout->count(); ++i) {
        QWidget *l_widget = m_baseLayout->itemAt(i)->widget();
        if (l_widget != nullptr)
            l_widget->resize(m_config->getSheetNoteWidth(), m_config->getSheetNoteHeight());
    }
}

void CentralWidget::serializeSheet(QJsonObject &p_jsonOut) const
{
    QJsonArray l_new;
    // l_note can theoretically be > max size of int
    // but anyway l_note should be changed to a string in the future
    for (auto l_note : m_notes) {
        qDebug() << static_cast<int>(*l_note);
        l_new.push_back(QJsonValue(static_cast<int>(*l_note)));
        p_jsonOut.insert("", l_new);
    }
    qDebug() << p_jsonOut;
}

void CentralWidget::unserializeSheet(const QJsonObject &p_jsonIn)
{
    QJsonArray l_array = p_jsonIn[""].toArray();

    deleteCurrentNotes();
    for (auto l_elem : l_array) {
        uint *l_noteVal = new uint(l_elem.toInt());
        m_notes.push_back(l_noteVal);
    }
    logCurrentNotes();
}

void CentralWidget::deleteCurrentNotes()
{
    for (auto l_elem : m_notes) {
         delete l_elem;
    }
    m_notes.clear();
}

CentralWidget::~CentralWidget()
{
    deleteCurrentNotes();
}
