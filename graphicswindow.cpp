#include "graphicswindow.h"
#include "mainwindow.h"
#include "ui_graphicswindow.h"

graphicsWindow::graphicsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::graphicsWindow)
{
    ui->setupUi(this);



}

graphicsWindow::~graphicsWindow()
{
    delete ui;
}

void graphicsWindow::on_pushButton_clicked()
{
    // Створюємо інстанцію registerWindow
    mainWindow *mainwindow = new mainWindow();
    // Показуємо вікно реєстрації
    mainwindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}

void graphicsWindow::on_tableWidget_cellChanged(int row, int column)
{

}

void graphicsWindow::redraw() {
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}
