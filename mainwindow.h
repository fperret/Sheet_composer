#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "config.h"
#include "editpanel.h"
#include "centralwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace toolbar {
enum class ToolBarAction {
    EXPAND_WIDTH = 0,
    REDUCE_WIDTH,
    EXPAND_HEIGHT,
    REDUCE_HEIGHT,
    DELETE_SHEET_NOTE,
};

inline uint qHash(ToolBarAction p_key, uint p_seed)
{
    return ::qHash(static_cast<uint>(p_key), p_seed);
}
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Config m_config;
    void sheetNoteSelectedChanged(const bool p_selected);

public Q_SLOTS:
    void addNewNote();
    void saveSheet() const;
    void openSheet();
    void createNoteWidget();
    void editSettings();

    void sheetSaveAs();
    void deleteSelectedSheetNote();

Q_SIGNALS:
    void resizeNeeded(void);

private:
    Ui::MainWindow *ui;
    QString m_currentSheetPath;
    // For now we only have one toolBar and it's created at the beginning
    QToolBar *m_toolBar;
    QHash<toolbar::ToolBarAction, QAction *> m_toolBarActions;
    CentralWidget   m_centralWidget;
    EditPanel       m_editPanel;

    void createToolBar();
    void saveCurrentInstrument(void) const;
    void loadNotesForInstrument(const std::string &p_instrumentPath);
};
#endif // MAINWINDOW_H
