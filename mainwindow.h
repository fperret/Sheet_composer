#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "config.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Config m_config;

public Q_SLOTS:
    void addNewNote();
    void saveSheet() const;
    void openSheet();
    void createNoteWidget();
    void editSettings();

    void sheetSaveAs();

Q_SIGNALS:
    void resizeNeeded(void);

private:
    Ui::MainWindow *ui;
    QString m_currentSheetPath;

    void createToolBar();
    void saveCurrentInstrument(void) const;
    void loadNotesForInstrument(const std::string &p_instrumentPath);
};
#endif // MAINWINDOW_H
