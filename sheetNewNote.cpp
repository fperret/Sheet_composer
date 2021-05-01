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

#include "Note.h"
//#include "clickablelabel.h"


void CentralWidget::addImageToGrid(QGridLayout *p_layout, const NoteVal &p_note, size_t p_row, size_t p_column)
{
    const std::string l_imagePath = "totot";//getNoteImagePath(p_note);
    QPixmap l_image(l_imagePath.c_str());

    ClickableLabel *l_imageLabel = new ClickableLabel(this);
    l_imageLabel->setPixmap(l_image);
    l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
    l_imageLabel->setProperty("value", p_note);
    p_layout->addWidget(l_imageLabel, p_row, p_column);
    connect(l_imageLabel, &ClickableLabel::clicked, this, &CentralWidget::popupNoteClicked);
}

void CentralWidget::popupNoteClicked()
{
    const NoteVal l_val = sender()->property("value").value<NoteVal>();
    addNoteToSheet(l_val);
    placeAddImage();
}

void CentralWidget::addNotePopup()
{
    QDialog *l_dialog = new QDialog(this);
    QGridLayout *l_popupGrid = new QGridLayout(l_dialog);

    l_popupGrid->setSpacing(1);
    addImageToGrid(l_popupGrid, NoteVal::DO, 0, 0);
    addImageToGrid(l_popupGrid, NoteVal::RE, 0, 1);
    addImageToGrid(l_popupGrid, NoteVal::MI, 0, 2);
    addImageToGrid(l_popupGrid, NoteVal::FA, 0, 3);
    addImageToGrid(l_popupGrid, NoteVal::SO, 0, 4);
    addImageToGrid(l_popupGrid, NoteVal::LA, 0, 5);
    addImageToGrid(l_popupGrid, NoteVal::SI, 0, 6);

    l_dialog->setLayout(l_popupGrid);
    l_dialog->exec();
    // delete popupgrid ?
}
