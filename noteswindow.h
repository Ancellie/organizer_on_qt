#ifndef notesWindow_H
#define notesWindow_H

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
    explicit notesWindow(bool isGroup = false, QWidget *parent = nullptr);
    ~notesWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_backButton_clicked();
    void on_lineEdit_textChanged(const QString &text);


    void on_changeButton_clicked();

private:
    bool isGroup;
    Ui::notesWindow *ui;
    QListWidget *listWidget;

    void addLineEdit(const QString &text = QString());
    void removeLineEdit(int row);
    void loadNotes();
    void saveNotes();
};

#endif // notesWindow_H
