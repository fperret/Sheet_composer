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
    bool saveSheet() const;
    void openSheet();
    void createNoteWidget();
    void editSettings();

private:
    Ui::MainWindow *ui;
    std::string m_currentSheetPath;

    void createToolBar();
    void saveCurrentInstrument(void) const;
    void loadNotesForInstrument(const std::string &p_instrumentPath);
};
#endif // MAINWINDOW_H
