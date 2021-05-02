#include "config.h"

Config::Config()
{

}

Config::~Config()
{
}

// Note : value of note could be a string
bool Config::addNote(const QString &p_notePath)
{
    QList<uint> l_keys = m_notes.keys();
    // Not sure if the sort is needed ?
    std::sort(l_keys.begin(), l_keys.end());

    // Search for the smallest value not already present in the map
    uint l_previousKey = l_keys.empty() ? 0 : l_keys.first();
    for (auto key : l_keys) {
        if (key > (l_previousKey + 1))
            break;
        l_previousKey = key;
    }
    if (l_previousKey == 99) {
        qWarning() << "No more values available to register note";
        return false;
    }

    addNote(l_previousKey + 1, p_notePath);
    return true;
}

/*
 * Will override existing value
 */
// Note : value of note could be a string
void Config::addNote(const uint &p_value, const QString &p_notePath)
{
    m_notes[p_value] = p_notePath;
    qDebug() << "Notes : " << m_notes;
}


void Config::setInstruments(const QVector<QString> &p_instruments)
{
    m_instruments = p_instruments;
}

void Config::setNotes(const QMap<uint, QString> &p_notes)
{
    m_notes = p_notes;
}

const QVector<QString> &Config::getInstruments(void) const
{
    return m_instruments;
}

const QMap<uint, QString> &Config::getNotes(void) const
{
    return m_notes;
}

void Config::increaseSheetNoteWidth() {
    setSheetNoteWidth(m_sheetNoteWidth + 1);
}

void Config::decreaseSheetNoteWidth() {
    setSheetNoteWidth(m_sheetNoteWidth - 1);
}

void Config::increaseSheetNoteHeight() {
    setSheetNoteHeight(m_sheetNoteHeight + 1);
}

void Config::decreaseSheetNoteHeight() {
    setSheetNoteHeight(m_sheetNoteHeight - 1);
}

void Config::setSheetNoteWidth(const int p_newWidth) {
    if (p_newWidth <= 0)
        qWarning() << "Cannot set Note width because value is too small";
    else if (p_newWidth > ABSOLUTE_MAX_NOTE_WIDTH)
        qWarning() << "Cannot set Note width because value is too big";
    else
        m_sheetNoteWidth = p_newWidth;
}

void Config::setSheetNoteHeight(const int p_newHeight) {
    if (p_newHeight <= 0)
        qWarning() << "Cannot set Note height because value is too small";
    else if (p_newHeight > ABSOLUTE_MAX_NOTE_HEIGHT)
        qWarning() << "Cannot set Note height because value is too big";
    else
        m_sheetNoteHeight = p_newHeight;
    qDebug() << "New height : " << m_sheetNoteHeight;
}
