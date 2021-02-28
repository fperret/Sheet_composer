#include "Note.h"

Note::Note()
{
    m_val = UNSET;
}

Note::~Note()
{

}

Note::Note(const NoteVal p_noteVal) : m_val(p_noteVal)
{

}

NoteVal Note::getVal() const
{
    return m_val;
}

void Note::setVal(const NoteVal p_newVal)
{
    m_val = p_newVal;
}
