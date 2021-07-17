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
    void addNote(const int &p_value, const QString &p_notePath);

    void setInstruments(const QVector<QString> &p_instruments);
    void setNotes(const QMap<int, QString> &p_notes);
    void setNotes(QMap<int, QString> &&p_notes);

    const QVector<QString> &getInstruments(void) const;
    const QMap<int, QString> &getNotes(void) const;

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

Q_SIGNALS:
    void configNotesChanged(void);

private:
    QVector<QString> m_instruments;
    // All possible notes available
    // Each key in a row is the value of the note, the value is the path to the image
    QMap<int, QString> m_notes;

    int m_sheetNoteWidth = 100;
    int m_sheetNoteHeight = 250;

    const int ABSOLUTE_MAX_NOTE_HEIGHT = 1000;
    const int ABSOLUTE_MAX_NOTE_WIDTH = 1000;
};

#endif // CONFIG_H

