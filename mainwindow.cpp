#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "notesWindow.h"
#include "todowindow.h"
#include "graphicswindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::on_exitButton_clicked()
{
    // Створюємо інстанцію registerWindow
    loginWindow *logWindow = new loginWindow();
    // Показуємо вікно реєстрації
    logWindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}


void mainWindow::on_notesButton_clicked()
{
    // Створюємо інстанцію registerWindow
    notesWindow *noteswindow = new notesWindow();
    // Показуємо вікно реєстрації
    noteswindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}


void mainWindow::on_todoButton_clicked()
{
    // Створюємо інстанцію registerWindow
    todoWindow *todowindow = new todoWindow();
    // Показуємо вікно реєстрації
    todowindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}


void mainWindow::on_grahicsButton_clicked()
{
    // Створюємо інстанцію registerWindow
    graphicsWindow *graphicswindow = new graphicsWindow();
    // Показуємо вікно реєстрації
    graphicswindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}
