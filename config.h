#ifndef CONFIG_H
#define CONFIG_H

#include <Qt>
#include <QtCore>
#include <QApplication>

class Config : public QObject
{
    Q_OBJECT
public:
    Config(QObject *parent);
    ~Config();

    bool addNote(const QString &p_notePath);
    void addNote(const uint &p_value, const QString &p_notePath);

    void setInstruments(const QVector<QString> &p_instruments);
    void setNotes(const QMap<uint, QString> &p_notes);

    const QVector<QString> &getInstruments(void) const;
    const QMap<uint, QString> &getNotes(void) const;

    int getSheetNoteWidth(void) const {
        return m_sheetNoteWidth;
    }
    void setSheetNoteWidth(const int p_newWidth);


    int getSheetNoteHeight(void) const {
        return m_sheetNoteHeight;
    }
    void setSheetNoteHeight(const int p_newHeight);


public Q_SLOTS:
    void increaseSheetNoteWidth();
    void decreaseSheetNoteWidth();
    void increaseSheetNoteHeight();
    void decreaseSheetNoteHeight();


private:
    QVector<QString> m_instruments;
    // Note : value of note could be a string
    QMap<uint, QString> m_notes;

    int m_sheetNoteWidth = 100;
    //int m_sheetNoteHeight = 250;
    int m_sheetNoteHeight = 600;

    const int ABSOLUTE_MAX_NOTE_HEIGHT = 1000;
    const int ABSOLUTE_MAX_NOTE_WIDTH = 1000;
};

#endif // CONFIG_H

