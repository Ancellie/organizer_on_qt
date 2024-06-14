#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QWidget>

namespace Ui {
class notesWindow;
}

class notesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit notesWindow(QWidget *parent = nullptr);
    ~notesWindow();

private slots:
    void on_backButton_clicked();

private:
    Ui::notesWindow *ui;
};

#endif // NOTESWINDOW_H
