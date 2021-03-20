#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public Q_SLOTS:
    void addNewNote();
    bool saveSheet() const;
    void openSheet();
    void createNoteWidget();

private:
    Ui::MainWindow *ui;
    std::string m_currentSheetPath;

    QString m_selectedFileName;

    void createToolBar();
};
#endif // MAINWINDOW_H
