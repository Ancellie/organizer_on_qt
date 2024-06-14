#include "noteswindow.h"
#include "ui_noteswindow.h"
#include "mainwindow.h"

notesWindow::notesWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::notesWindow)
{
    ui->setupUi(this);
}

notesWindow::~notesWindow()
{
    delete ui;
}

void notesWindow::on_backButton_clicked()
{
    // Створюємо інстанцію registerWindow
    mainWindow *mainwindow = new mainWindow();
    // Показуємо вікно реєстрації
    mainwindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}

