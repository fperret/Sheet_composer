#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QDebug>

#include <QGridLayout>

#include <QJsonObject>

#include "Note.h"
#include "clickablelabel.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

    void createSheetDisplay();

    void serializeSheet(QJsonObject &p_jsonOut) const;
    void unserializeSheet(QJsonObject &p_jsonIn);

public Q_SLOTS:
    void imageClicked();
    void addNote();
    void addNotePopup();

private:
    QGridLayout *m_baseLayout;
    QVector<Note *> m_notes;
    Note *m_imageSelected;
    ClickableLabel *m_imageAdd;

    int m_maxColumns;
    // TODO : Remove
    // We should have a way to add new rows without changing the size of the current ones
    int m_maxRows;
    int m_currentLastColumn;
    int m_currentLastRow;

    void changeNoteValue();
    void logCurrentNotes() const;
    void deleteCurrentNotes();
    void addImage(QGridLayout *p_layout, const NoteVal &p_note);

    void addWidgetInLastPos(QGridLayout *p_layout, QWidget *p_widget);
    void placeAddImage();

signals:

};

#endif // CENTRALWIDGET_H
