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
    uint *l_val = new uint(sender()->property("value").value<uint>());
    m_notes.push_back(l_val);
    drawNoteToSheet(*l_val);
    placeAddImage(false);

    // If we added the first note to a row, place the "add image" button for the next row
    if (m_lastColumns[m_selectedRow] == 1)
        placeAddImage(true);
}

void CentralWidget::addNotePopup()
{
    if (sender()) {
        m_selectedRow = sender()->property(ADD_IMAGE_ROW).value<int>();
    }

    QDialog *l_dialog = new QDialog(this);
    QGridLayout *l_popupGrid = new QGridLayout(l_dialog);
    const QMap<uint, QString> &l_configNotes = m_config->getNotes();

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
