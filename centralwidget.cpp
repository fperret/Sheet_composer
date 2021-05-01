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

#include "Note.h"
#include "clickablelabel.h"

void CentralWidget::setConfig(Config *p_config)
{
    m_config = p_config;
}

bool isNoteValid(const NoteVal p_note)
{
    return (p_note == DO
            || p_note == RE
            || p_note == MI
            || p_note == FA
            || p_note == SO
            || p_note == LA
            || p_note == SI);
}

const std::string getNoteImagePath(const NoteVal &p_note)
{
    switch (p_note)
    {
        case DO:
            return (":/images/DO.png");
            break;

        case RE:
            return (":/images/RE.png");
            break;

        case MI:
            return (":/images/MI.png");
            break;

        case FA:
            return (":/images/FA.png");
            break;

        case SO:
            return (":/images/SO.png");
            break;

        case LA:
            return (":/images/LA.png");
            break;

        case SI:
            return (":/images/SI.png");
            break;

        default:
            qWarning() << "Note unknown";
            // TODO : add blank image
            return (":/images/blank.png");
    }
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
    m_baseLayout->addWidget(m_imageAdd, m_currentLastRow, m_currentLastColumn);
}

void CentralWidget::addNoteToSheet(const NoteVal &p_note)
{
    const std::string l_imagePath = getNoteImagePath(p_note);
    QPixmap l_image(l_imagePath.c_str());
    //QLabel *l_imageLabel = new QLabel();

    ClickableLabel *l_imageLabel = new ClickableLabel(this);
    l_imageLabel->setPixmap(l_image);
    l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
qDebug() << m_baseLayout->rowCount() << " / " << m_baseLayout->columnCount();

    addWidgetInLastPos(m_baseLayout, l_imageLabel);
    //p_layout->addWidget(l_imageLabel, m_currentLastRow, m_currentLastColumn - 1);
    connect(l_imageLabel, &ClickableLabel::clicked, this, &CentralWidget::imageClicked);
    //p_layout->setColumnStretch(p_layout->columnCount() - 1, 2);
}

void CentralWidget::addNote()
{

}

void CentralWidget::imageClicked()
{
    //m_imageSelected = qobject_cast<ClickableLabel *>(sender());
}

void CentralWidget::changeNoteValue()
{
    if (!m_imageSelected)
        return ;

    m_imageSelected->setVal(NoteVal::DO);
}

void CentralWidget::logCurrentNotes() const
{
    std::string l_log;
    for (auto l_note : m_notes) {
        qDebug() << l_note->getVal();
    }
}

void CentralWidget::createSheetDisplay()
{
    for (auto l_note : m_notes) {
        addNoteToSheet(l_note->getVal());
    }
}

void CentralWidget::serializeSheet(QJsonObject &p_jsonOut) const
{
    QJsonArray l_new;
    for (auto l_note : m_notes) {
        l_new.push_back(l_note->getVal());
        p_jsonOut.insert("", l_new);
    }
}

void CentralWidget::unserializeSheet(QJsonObject &p_jsonIn)
{
    QJsonArray l_array = p_jsonIn[""].toArray();

    QVector<Note *> new_notes;
    for (auto l_elem : l_array) {
        QVariant l_value(l_elem.toInt());
        Note *l_note = new Note(l_value.value<NoteVal>());

        if (isNoteValid(l_note->getVal()))
            new_notes.push_back(l_note);
        else {// Generer une fenetre d'erreur
            qWarning() << "Invalid value " << l_value << " for note in file";
            delete l_note;
            return ;
        }
    }

    // use QVector::swap() ?
    deleteCurrentNotes();
    m_notes = new_notes;
    logCurrentNotes();
}


void CentralWidget::deleteCurrentNotes()
{
    for (auto l_elem : m_notes) {
        delete l_elem;
    }
    m_notes.clear();
}

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    m_baseLayout = new QGridLayout(this);
    m_imageSelected = nullptr;

    m_maxColumns = 5;
    m_maxRows = 5;
    m_currentLastColumn = 0;
    m_currentLastRow = 0;

    m_baseLayout->setSpacing(1);
    for (int i = 0; i < m_maxColumns; ++i) {
        m_baseLayout->setColumnStretch(i, 1);
    }
    for (int i = 0; i < m_maxRows; ++i) {
        m_baseLayout->setRowStretch(i, 1);
    }

    // Button ADD
    QPixmap l_addImage(":/images/plus_sign.png");
    m_imageAdd = new ClickableLabel(this);
    m_imageAdd->setPixmap(l_addImage);
    m_imageAdd->setStyleSheet("QLabel { background-color : red; }");
    placeAddImage();
    connect(m_imageAdd, &ClickableLabel::clicked, this, &CentralWidget::addNotePopup);
}

CentralWidget::~CentralWidget()
{
    deleteCurrentNotes();
}
