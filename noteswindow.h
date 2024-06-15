#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QFile>
#include <QDir>

namespace Ui {
class notesWindow;
}

class notesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit notesWindow(QWidget *parent = nullptr);
    ~notesWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_backButton_clicked();
    void on_lineEdit_textChanged(const QString &text);

private:
    Ui::notesWindow *ui;
    QListWidget *listWidget;

    void addLineEdit(const QString &text = QString());
    void removeLineEdit(int row);
    void loadNotes();
    void saveNotes();
};

#endif // NOTESWINDOW_H
