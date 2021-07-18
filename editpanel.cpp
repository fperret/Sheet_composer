#include "editpanel.h"
#include <QDebug>
#include "clickablelabel.h"

#include "mainwindow.h"
#include "centralwidget.h"
#include "QtUtils.h"

namespace {

const QString EDIT_PANEL_TITLE = "Edit Note";

}


EditPanel::EditPanel(QWidget *parent, Config *p_config, const CentralWidget *p_centralWidget) :
    QDockWidget(EDIT_PANEL_TITLE, parent),
    m_centralWidget(p_centralWidget),
    m_config(p_config),
    m_scrollArea(new QScrollArea(this)),
    m_scrollWidget(new QWidget(this)),
    m_editNotesLayout(new QGridLayout(m_scrollArea))
{
    setWidget(m_scrollArea);
    // TODO : dynamic size depending on image size
    m_scrollArea->setMinimumWidth(250);
    m_editNotesLayout->setSpacing(3);
}

EditPanel::~EditPanel()
{
}

void EditPanel::initialize(EditPanelAccessKey)
{
    qDebug() << Q_FUNC_INFO;
    // need to factorize this with CentralWidget::addNotePopup()
    const QMap<int, QString> &l_configNotes = m_config->getNotes();

    deleteWidgetsFromLayout(m_editNotesLayout);

    size_t l_row = 0;
    size_t l_column = 0;
    for (auto l_noteVal : l_configNotes.keys()) {
        if (l_configNotes.value(l_noteVal).isEmpty())
            continue;

        QPixmap l_image(l_configNotes.value(l_noteVal));
        ClickableLabel *l_imageLabel = new ClickableLabel(m_scrollArea);

        l_imageLabel->setPixmap(l_image);
        l_imageLabel->setStyleSheet("QLabel { background-color : red; }");
        l_imageLabel->setProperty("value", l_noteVal);
        // This is important so that the QLabel takes the same space as QPixmap
        // It would seem more logic to me if this was QSizePolicy::Minimum ???
        l_imageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        m_editNotesLayout->addWidget(l_imageLabel, l_row, l_column);
        connect(l_imageLabel, &ClickableLabel::clicked, m_centralWidget, &CentralWidget::editNoteClicked);
        l_column++;
        if (l_column == NOTE_CHOICES_MAX_COLUMNS) {
            l_column = 0;
            l_row++;
        }
    }

    // QScrollArea uses its own layout so we need to wrap our QGridLayout in a QWidget
    m_scrollWidget->setLayout(m_editNotesLayout);
    // From QT docs :
    // Note that You must add the layout of the parameter before you call this function; if you add it later, the widget will not be visible
    m_scrollArea->setWidget(m_scrollWidget);
}
