#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QDebug>

#include <QGridLayout>

#include <QJsonObject>

#include "clickablelabel.h"
#include "config.h"
#include "mainwindow.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(MainWindow *p_mainWindow, QWidget *parent = nullptr);
    ~CentralWidget();

    void loadSheetFromJson(const QJsonObject &p_jsonIn);
    void serializeSheet(QJsonObject &p_jsonOut) const;

    void setConfig(Config *p_config);

public Q_SLOTS:
    void imageClicked();
    void popupNoteClicked();
    void addNotePopup();

private:
    // Path to the "add note" button's imagee
    const QString ADD_IMAGE_PATH = ":/images/plus_sign.png";
    const char * const ADD_IMAGE_ROW = "row";

    QGridLayout *m_baseLayout;
    QVector<uint *> m_notes;
    uint *m_imageSelected;
    QVector<ClickableLabel *> m_imageAdd;
    Config *m_config;

    int m_maxColumns;
    // TODO : Remove
    // We should have a way to add new rows without changing the size of the current ones
    int m_maxRows;

    // Last current column of each row
    QVector<uint32_t> m_lastColumns;
    int m_selectedRow;

    void initSheetDisplay();

    void changeNoteValue();
    void logCurrentNotes() const;
    void deleteCurrentNotes();
    void drawNoteToSheet(const uint &p_noteVal);

    void addWidgetInLastCol(QGridLayout *p_layout, QWidget *p_widget);
    void placeAddImage(const bool p_nextRow);

    void addNoteToChoices(QGridLayout *p_layout, uint p_val, const QString &p_imagePath, size_t p_row, size_t p_column);

    void unserializeSheet(const QJsonObject &p_jsonIn);
    void createSheetDisplay();
    void resizeNotesDisplay();

signals:

};

#endif // CENTRALWIDGET_H
