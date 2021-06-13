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

    m_selectedRow = 0;

    m_baseLayout->setSpacing(1);

    placeAddImage(0);

    // At the beginning the rows are super big because they expand to take all available places
    // could add fake rows at the beginning to even things out
}

void CentralWidget::setConfig(Config *p_config)
{
    m_config = p_config;
}

void CentralWidget::addWidgetInLastCol(QGridLayout *p_layout, QWidget *p_widget, const int p_row)
{
    // Widget is added in the first column of a new row
    while (m_lastColumns.size() <= p_row) {
        qDebug() << "Set stretch of row " << p_row;
        m_baseLayout->setRowStretch(p_row, 1);
        m_lastColumns.push_back(0);
    }
    p_layout->addWidget(p_widget, p_row, m_lastColumns[p_row]);
    // This will be repeated and do nothing for each row, not sure if it is a problem
    m_baseLayout->setColumnStretch(m_lastColumns[p_row], 1);
}

void CentralWidget::placeAddImage(const int p_row)
{
    while (m_imageAdd.size() <= p_row) {
        m_imageAdd.push_back(nullptr);
    }

    // When we clear the sheet display we delete all widgets on it including this one
    // So we need to recreate it
    ClickableLabel *l_label = m_imageAdd[p_row];
    if (!l_label) {
        QPixmap l_addImage(ADD_IMAGE_PATH);
        l_label = new ClickableLabel(this);
        l_label->setPixmap(l_addImage);
        l_label->setStyleSheet("QLabel { background-color : red; }");
        connect(l_label, &ClickableLabel::clicked, this, &CentralWidget::addNotePopup);
    }
    l_label->setProperty(ADD_IMAGE_ROW, p_row);
    addWidgetInLastCol(m_baseLayout, l_label, p_row);
}

void CentralWidget::drawNoteToSheet(const uint &p_noteVal, const int p_row)
{
    const QString &l_imagePath = m_config->getNotes()[p_noteVal];
    QPixmap l_image(l_imagePath);

    ClickableLabel *l_imageLabel = new ClickableLabel(this);
    l_imageLabel->setPixmap(l_image);
    l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
    l_imageLabel->setScaledContents(true);
    l_imageLabel->resize(m_config->getSheetNoteWidth(), m_config->getSheetNoteHeight());
    addWidgetInLastCol(m_baseLayout, l_imageLabel, p_row);
    m_lastColumns[p_row]++;

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
}

void CentralWidget::logCurrentNotes() const
{
    std::string l_log;
    for (auto l_row : m_notes) {
        for (auto l_note : l_row) {
            l_log += std::to_string(l_note) + "|";
        }
    }
    qDebug() << "Current notes : " << l_log.c_str();
}

void CentralWidget::initSheetDisplay()
{
    deleteWidgetsFromLayout(m_baseLayout);
    m_selectedRow = 0;
    m_lastColumns.clear();
}

void CentralWidget::loadSheetFromJson(const QJsonObject &p_jsonIn)
{
    initSheetDisplay();
    unserializeSheet(p_jsonIn);
    createSheetDisplay();
}

void CentralWidget::createSheetDisplay()
{
    uint l_rowIndex = 0;
    for (auto l_row : m_notes) {
        for (auto l_note : l_row) {
            drawNoteToSheet(l_note, l_rowIndex);
        }
        placeAddImage(l_rowIndex);
        l_rowIndex++;
    }
    placeAddImage(l_rowIndex);
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

QJsonObject CentralWidget::serializeSheet(void) const
{
    QJsonObject l_jsonOut;
    QJsonArray l_new;

    for (auto l_rowNotes : m_notes) {
        QJsonArray l_rowJson;
        for (auto l_note : l_rowNotes) {
            qDebug() << l_note;
            l_rowJson.push_back(QJsonValue(l_note));
        }
        // use index of row as key ?
        l_new.push_back(l_rowJson);
    }
    l_jsonOut.insert("", l_new);
    return l_jsonOut;
}

void CentralWidget::unserializeSheet(const QJsonObject &p_jsonIn)
{
    QJsonArray l_array = p_jsonIn[""].toArray();

    deleteCurrentNotes();
    for (auto l_rowJson : l_array) {
        QVector<int> l_rowNotes;
        for (auto l_elem : l_rowJson.toArray()) {
            l_rowNotes.push_back(l_elem.toInt());
        }
        m_notes.push_back(l_rowNotes);
        m_lastColumns.push_back(0);
    }
    m_lastColumns.push_back(0);

    qDebug() << "lastcolumns size after unserialize : " << m_lastColumns.size();
    logCurrentNotes();
}

// TODO : remove if we do not add anything else than m_notes.clear()
void CentralWidget::deleteCurrentNotes()
{
    m_notes.clear();
}

CentralWidget::~CentralWidget()
{
    deleteCurrentNotes();
}
