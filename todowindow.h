#ifndef todoWindow_H
#define todoWindow_H

#include <QWidget>

namespace Ui {
class todoWindow;
}

class todoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit todoWindow(QWidget *parent = nullptr);
    ~todoWindow();

private slots:
    void on_backButton_clicked();

    void on_addButton_clicked();

private:
    Ui::todoWindow *ui;
};

#endif // todoWindow_H
