#include "centralwidget.h"

#include <QApplication>
//#include <QtWidgets/QTextEdit>
//#include <QtWidgets/QLayout>

//#include <QGraphicsView>
//#include <QGraphicsPixmapItem>
//#include <QLabel>
//#include <QPushButton>
#include <QDialog>

//#include <QJsonArray>
//#include <QJsonValue>

//#include "clickablelabel.h"

static const size_t ADD_NOTE_CHOICES_MAX_COLUMNS = 5;

void CentralWidget::addNoteToChoices(QGridLayout *p_layout, uint p_val, const QString &p_imagePath, size_t p_row, size_t p_column)
{
    QPixmap l_image(p_imagePath);

    ClickableLabel *l_imageLabel = new ClickableLabel(this);
    l_imageLabel->setPixmap(l_image);
    l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
    l_imageLabel->setProperty("value", p_val);
    p_layout->addWidget(l_imageLabel, p_row, p_column);
    connect(l_imageLabel, &ClickableLabel::clicked, this, &CentralWidget::popupNoteClicked);
}

void CentralWidget::popupNoteClicked()
{
    int l_val = sender()->property("value").value<int>();
    if (m_notes.size() <= m_selectedRow)
        m_notes.insert(m_selectedRow, QVector<int>());
    m_notes[m_selectedRow].push_back(l_val);
    drawNoteToSheet(l_val, m_selectedRow);
    placeAddImage(m_selectedRow);

    // If we added the first note to a row, place the "add image" button for the next row
    if (m_lastColumns[m_selectedRow] == 1)
        placeAddImage(m_selectedRow + 1);
}

void CentralWidget::editNoteClicked()
{
    int l_val = sender()->property("value").value<int>();
    if (isSelectedNoteValid()) {
        deleteSelectedSheetNote(true);
        m_notes[m_posOfSelectedNote.row][m_posOfSelectedNote.column] = l_val;
        ClickableLabel *l_imageLabel = createNoteForSheet(l_val);
        m_baseLayout->addWidget(l_imageLabel, m_posOfSelectedNote.row, m_posOfSelectedNote.column);
        m_selectedNoteWidget = l_imageLabel;
        m_selectedNoteOverlay.raise();
    }
    // If we allow the "addImage" to be selected we will need to recreate one here
}

void CentralWidget::addNotePopup()
{
    if (sender()) {
        m_selectedRow = sender()->property(ADD_IMAGE_ROW).value<int>();
    }

    QDialog *l_dialog = new QDialog(this);
    QGridLayout *l_popupGrid = new QGridLayout(l_dialog);
    const QMap<int, QString> &l_configNotes = m_config->getNotes();

    l_popupGrid->setSpacing(1);
    size_t l_row = 0;
    size_t l_column = 0;
    for (auto l_noteVal : l_configNotes.keys()) {
        if (l_configNotes.value(l_noteVal).isEmpty())
            continue;
        addNoteToChoices(l_popupGrid, l_noteVal, l_configNotes.value(l_noteVal), l_row, l_column);
        l_column++;
        if (l_column == ADD_NOTE_CHOICES_MAX_COLUMNS) {
            l_column = 0;
            l_row++;
        }
    }

    l_dialog->setLayout(l_popupGrid);
    l_dialog->exec();
    // delete popupgrid ?
}
