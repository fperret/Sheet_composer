#include "centralwidget.h"

#include <QApplication>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLayout>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QPushButton>

#include <QJsonArray>
#include <QJsonValue>

#include "Note.h"
#include "clickablelabel.h"

bool isNoteValid(const NoteVal p_note)
{
    return (p_note == DO
            || p_note == RE
            || p_note == MI
            || p_note == FA
            || p_note == SO
            || p_note == LA);
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

        default:
            qWarning() << "Note unknown";
            // TODO : add blank image
            return (":/images/blank.png");
    }
}

void CentralWidget::addImage(QGridLayout *p_layout, const NoteVal &p_note)
{
    const std::string l_imagePath = getNoteImagePath(p_note);
    QPixmap l_image(l_imagePath.c_str());
    //QLabel *l_imageLabel = new QLabel();

    ClickableLabel *l_imageLabel = new ClickableLabel();
    l_imageLabel->setPixmap(l_image);
//qDebug() << p_layout->rowCount() << " / " << p_layout->columnCount();
    p_layout->addWidget(l_imageLabel, p_layout->rowCount() - 1, p_layout->columnCount());

    connect(l_imageLabel, &ClickableLabel::clicked, this, &CentralWidget::imageClicked);
    //p_layout->setColumnStretch(p_layout->columnCount() - 1, 2);
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
        addImage(m_baseLayout, l_note->getVal());
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
}

CentralWidget::~CentralWidget()
{
    deleteCurrentNotes();
}
