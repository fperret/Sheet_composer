#ifndef CONFIG_H
#define CONFIG_H

#include <Qt>
#include <QtCore>

class Config
{

public:
    Config();
    ~Config();

    bool addNote(const QString &p_notePath);
    void addNote(const uint &p_value, const QString &p_notePath);

    void setInstruments(const QVector<QString> &p_instruments);
    void setNotes(const QMap<uint, QString> &p_notes);

    const QVector<QString> &getInstruments(void) const;
    const QMap<uint, QString> &getNotes(void) const;


private:
    QVector<QString> m_instruments;
    // Note : value of note could be a string
    QMap<uint, QString> m_notes;
};

#endif // CONFIG_H
