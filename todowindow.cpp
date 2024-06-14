#include "todowindow.h"
#include "ui_todowindow.h"
#include "mainwindow.h"

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

