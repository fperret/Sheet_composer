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
    QJsonObject serializeSheet(void) const;

    void setConfig(Config *p_config);

public Q_SLOTS:
    void imageClicked();
    void popupNoteClicked();
    void addNotePopup();

private:
    // Path to the "add note" button's imagee
    const QString ADD_IMAGE_PATH = ":/images/plus_sign.png";
    const char * const ADD_IMAGE_ROW    = "row";
    const QString MAIN_GRID_LAYOUT_NAME = "Central QGridLayout";
    const QString THIS_NAME             = "CentralWidget";

    QGridLayout *m_baseLayout;
    // Est ce qu'on a besoin d'avoir les notes ici et celles dans config ?
    // Ici on n'a pas besoin de stocker le path associe
    QVector<QVector<int> > m_notes;
    QVector<ClickableLabel *> m_imageAdd;
    Config *m_config;

    // Last current column of each row
    QVector<uint32_t> m_lastColumns;
    int m_selectedRow;

    void initSheetDisplay();

    void changeNoteValue();
    void logCurrentNotes() const;
    void deleteCurrentNotes();
    void drawNoteToSheet(const uint &p_noteVal, const int p_row);

    void addWidgetInLastCol(QGridLayout *p_layout, QWidget *p_widget, const int p_row);
    void placeAddImage(const int p_row);

    void addNoteToChoices(QGridLayout *p_layout, uint p_val, const QString &p_imagePath, size_t p_row, size_t p_column);

    void unserializeSheet(const QJsonObject &p_jsonIn);
    void createSheetDisplay();
    void resizeNotesDisplay();

signals:

};

#endif // CENTRALWIDGET_H
