#ifndef notesWindow_H
#define notesWindow_H

// Provides the main application window on which the user interface
// is built by adding widgets
#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>

// Use the standard UI namespace which is tied to the .ui file
namespace Ui {
class notesWindow;
}

class notesWindow : public QMainWindow
{
    // Declares our class as a QObject which is the base class
    // for all Qt objects
    // QObjects handle events
    // Each QObject executes on a thread
    Q_OBJECT

public:

    // Declare a constructor and by passing 0 we state this widget
    // has no parent
    explicit notesWindow(QWidget *parent = 0);

    // Declare a destructor which will free resources
    ~notesWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionPrint_triggered();

    void on_actionExit_triggered();

    void on_actionSave_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:

    // Point to the ui class
    Ui::notesWindow *ui;

    // A Qt character string that will hold text entered by the user
    QString currentFile = "";
};

#endif // notesWindow_H
