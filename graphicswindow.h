#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include <QWidget>

namespace Ui {
class graphicsWindow;
}

class graphicsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit graphicsWindow(QWidget *parent = nullptr);
    ~graphicsWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::graphicsWindow *ui;
};

#endif // GRAPHICSWINDOW_H
