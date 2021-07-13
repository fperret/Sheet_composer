#include "editpanel.h"
#include <QDebug>
#include "clickablelabel.h"

const QString EditPanel::m_title = "Edit Note";

EditPanel::EditPanel(QWidget *parent, Config *p_config) : QDockWidget(m_title, parent), m_config(p_config), m_scrollArea(new QScrollArea(this)), m_editNotesLayout(new QGridLayout(m_scrollArea))
{
    setWidget(m_scrollArea);
    m_scrollArea->setLayout(m_editNotesLayout);
    //m_scrollArea->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    // TODO : dynamic size depending on image size
    m_scrollArea->setMinimumWidth(250);
}

EditPanel::~EditPanel()
{
}

void EditPanel::initialize(EditPanelAccessKey)
{
    // need to factorize this with the new note popup

    const QMap<int, QString> &l_configNotes = m_config->getNotes();
    //qDebug() << m_config->getNotes();
    m_editNotesLayout->setSpacing(1);
    size_t l_row = 0;
    size_t l_column = 0;
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    for (auto l_noteVal : l_configNotes.keys()) {
        if (l_configNotes.value(l_noteVal).isEmpty())
            continue;

        QPixmap l_image(l_configNotes.value(l_noteVal));

        qDebug() << l_image.height();
        qDebug() << "image size " << l_image.size();
        ClickableLabel *l_imageLabel = new ClickableLabel(m_scrollArea);

        QPixmap scaled = l_image.scaled(l_imageLabel->size(), Qt::KeepAspectRatio);


        //l_imageLabel->setPixmap(l_image);
        l_imageLabel->setPixmap(scaled);
        l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
        l_imageLabel->setProperty("value", l_noteVal);
        l_imageLabel->resize(l_image.width(), l_image.height());
qDebug() << l_imageLabel->sizePolicy();
        l_imageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_editNotesLayout->setRowStretch(l_row, 0);
        m_editNotesLayout->setColumnStretch(l_column, 0);
        qDebug() << "Label size " << l_imageLabel->size();
        //l_imageLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_editNotesLayout->addWidget(l_imageLabel, l_row, l_column);
        l_column++;
        if (l_column == NOTE_CHOICES_MAX_COLUMNS) {
            l_column = 0;
            l_row++;
        }
    }
    m_scrollArea->resize(100, 100);
    //m_scrollArea->resize(1000, m_scrollArea->height());
}
