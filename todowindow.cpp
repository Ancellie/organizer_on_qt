#include "todowindow.h"
#include "mainwindow.h"
#include "ui_todowindow.h"

todoWindow::todoWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::todoWindow)
{
    ui->setupUi(this);


}

todoWindow::~todoWindow()
{
    delete ui;
}

void todoWindow::on_backButton_clicked()
{
    // Створюємо інстанцію registerWindow
    mainWindow *mainwindow = new mainWindow();
    // Показуємо вікно реєстрації
    mainwindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}

void todoWindow::on_addButton_clicked()
{

}

