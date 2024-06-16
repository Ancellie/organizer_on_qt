#include "mainwindow.h"
#include "calendarwindow.h"
#include "graphicswindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "noteswindow.h"
#include "todowindow.h"
#include "ctodolist.h"

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

    LoginWindow *logWindow = new LoginWindow();

    logWindow->show();

    this->close();
}

void mainWindow::on_notesButton_clicked()
{

    notesWindow *noteswindow = new notesWindow();

    noteswindow->show();

    this->close();
}

void mainWindow::on_todoButton_clicked()
{

    CToDoList *todowindow = new CToDoList();

    todowindow->show();

    this->close();
}

void mainWindow::on_grahicsButton_clicked()
{

    GraphicsWindow *graphicswindow = new GraphicsWindow();

    graphicswindow->show();

    this->close();
}

void mainWindow::on_calendarButton_clicked()
{
    CalendarWindow *calendarwindow = new CalendarWindow();
    calendarwindow->show();
    this->close();
}

