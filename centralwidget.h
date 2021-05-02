#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QDebug>

#include <QGridLayout>

#include <QJsonObject>

#include "Note.h"
#include "clickablelabel.h"
#include "config.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();

    void createSheetDisplay();

    void serializeSheet(QJsonObject &p_jsonOut) const;
    void unserializeSheet(QJsonObject &p_jsonIn);

    void setConfig(Config *p_config);

public Q_SLOTS:
    void imageClicked();
    void popupNoteClicked();
    void addNotePopup();

private:
    QGridLayout *m_baseLayout;
    QVector<Note *> m_notes;
    Note *m_imageSelected;
    ClickableLabel *m_imageAdd;
    Config *m_config;

    int m_maxColumns;
    // TODO : Remove
    // We should have a way to add new rows without changing the size of the current ones
    int m_maxRows;
    int m_currentLastColumn;
    int m_currentLastRow;

    void changeNoteValue();
    void logCurrentNotes() const;
    void deleteCurrentNotes();
    void addNoteToSheet(const uint &p_noteVal);

    void addWidgetInLastPos(QGridLayout *p_layout, QWidget *p_widget);
    void placeAddImage();

    void addNoteToChoices(QGridLayout *p_layout, uint p_val, const QString &p_imagePath, size_t p_row, size_t p_column);

signals:

};

#endif // CENTRALWIDGET_H
