#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
class mainWindow;
}

class mainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private slots:
    void on_exitButton_clicked();

    void on_notesButton_clicked();

    void on_todoButton_clicked();

    void on_grahicsButton_clicked();

    void on_calendarButton_clicked();

private:
    Ui::mainWindow *ui;
};

#endif // MAINWINDOW_H
