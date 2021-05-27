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

    QGridLayout *m_baseLayout;
    QVector<uint *> m_notes;
    uint *m_imageSelected;
    ClickableLabel *m_imageAdd;
    Config *m_config;

    int m_maxColumns;
    // TODO : Remove
    // We should have a way to add new rows without changing the size of the current ones
    int m_maxRows;
    int m_currentLastColumn;
    int m_currentLastRow;

    void initSheetDisplay();

    void changeNoteValue();
    void logCurrentNotes() const;
    void deleteCurrentNotes();
    void addNoteToSheet(const uint &p_noteVal);

    void addWidgetInLastPos(QGridLayout *p_layout, QWidget *p_widget);
    void placeAddImage();

    void addNoteToChoices(QGridLayout *p_layout, uint p_val, const QString &p_imagePath, size_t p_row, size_t p_column);

    void unserializeSheet(const QJsonObject &p_jsonIn);
    void createSheetDisplay();

signals:

};

#endif // CENTRALWIDGET_H
