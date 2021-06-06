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

    m_maxColumns = 5;
    m_maxRows = 5;
    m_selectedRow = 0;

    m_baseLayout->setSpacing(1);

    for (int i = 0; i < m_maxColumns; ++i) {
        m_baseLayout->setColumnStretch(i, 1);
        m_lastColumns.push_back(0);
    }
    for (int i = 0; i < m_maxRows; ++i) {
        m_baseLayout->setRowStretch(i, 1);
        m_lastColumns.push_back(0);
    }

    placeAddImage(false);
}

void CentralWidget::setConfig(Config *p_config)
{
    m_config = p_config;
}

void CentralWidget::addWidgetInLastCol(QGridLayout *p_layout, QWidget *p_widget)
{
    p_layout->addWidget(p_widget, m_selectedRow, m_lastColumns[m_selectedRow]);
    m_lastColumns[m_selectedRow]++;
}

void CentralWidget::placeAddImage(const bool p_nextRow)
{
    const int l_row = (p_nextRow ? m_selectedRow + 1 : m_selectedRow);

    while (m_imageAdd.size() <= l_row) {
        m_imageAdd.push_back(nullptr);
    }

    // When we clear the sheet display we delete all widgets on it including this one
    // So we need to recreate it
    ClickableLabel *l_label = m_imageAdd[l_row];
    if (!l_label) {
        QPixmap l_addImage(ADD_IMAGE_PATH);
        l_label = new ClickableLabel(this);
        l_label->setPixmap(l_addImage);
        l_label->setStyleSheet("QLabel { background-color : red; }");
        connect(l_label, &ClickableLabel::clicked, this, &CentralWidget::addNotePopup);
    }
    l_label->setProperty(ADD_IMAGE_ROW, l_row);
    m_baseLayout->addWidget(l_label, l_row, m_lastColumns[l_row]);
}

void CentralWidget::drawNoteToSheet(const uint &p_noteVal)
{
    const QString &l_imagePath = m_config->getNotes()[p_noteVal];
    QPixmap l_image(l_imagePath);

    ClickableLabel *l_imageLabel = new ClickableLabel(this);
    l_imageLabel->setPixmap(l_image);
    l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
    l_imageLabel->setScaledContents(true);

    qDebug() << "rowCount : " << m_baseLayout->rowCount() << " / columnCount " << m_baseLayout->columnCount();

    addWidgetInLastCol(m_baseLayout, l_imageLabel);
    l_imageLabel->resize(m_config->getSheetNoteWidth(), m_config->getSheetNoteHeight());
    // fixed size ca fonctionne mais il faut modifier la fixed size a chaque fois + ce n'est plus responsive
    //l_imageLabel->setFixedSize(QSize(100, 600));
    //l_imageLabel->setMinimumHeight(600);

    connect(l_imageLabel, &ClickableLabel::clicked, this, &CentralWidget::imageClicked);
}

void CentralWidget::imageClicked()
{
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
    deleteWidgetsFromLayout(m_baseLayout);
    m_selectedRow = 0;
    m_imageSelected = nullptr;
    m_lastColumns.clear();
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
    placeAddImage(false);
    placeAddImage(true);
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
