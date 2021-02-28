#ifndef NOTE_H
#define NOTE_H

#include <QApplication>

enum NoteVal
{
    DO = 0,
    RE,
    MI,
    FA,
    SO,
    LA,
    UNSET,
};
Q_DECLARE_METATYPE(NoteVal)

bool isNoteValid(const NoteVal p_note);

class Note
{
public:
    Note();
    ~Note();

    Note(NoteVal p_noteVal);

    NoteVal getVal() const;
    void setVal(NoteVal p_newVal);

private:
    NoteVal m_val;
};

#endif // NOTE_H
