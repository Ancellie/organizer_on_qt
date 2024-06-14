#include "loginwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "./ui_loginwindow.h"
#include "mainwindow.h"
#include "registerwindow.h"
#include "userdata.h"

loginWindow::loginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginWindow)
{
    ui->setupUi(this);
}

loginWindow::~loginWindow()
{
    delete ui;
}

void loginWindow::on_registerButton_clicked()
{
    // Створюємо інстанцію registerWindow
    registerWindow *regWindow = new registerWindow();
    // Показуємо вікно реєстрації
    regWindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}

void loginWindow::on_loginButton_clicked()
{
    QString username = ui->userLineEdit->text();
    QString password = ui->passworLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both username and password.");
        return;
    }

    if (authenticateUser(username, password)) {
        QMessageBox::information(this, "Success", "Login successful!");
        // Створюємо інстанцію registerWindow
        mainWindow *mainwindow = new mainWindow();
        // Показуємо вікно реєстрації
        mainwindow->show();
        // Закриваємо вікно логіну (опціонально)
        this->close();
    } else {
        QMessageBox::warning(this, "Error", "Invalid username or password.");
    }
}

bool loginWindow::authenticateUser(const QString &username, const QString &password)
{
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open file.");
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList userData = line.split(",");
        if (userData.size() == 2 && userData[0] == username && userData[1] == password) {
            UserData::username = userData[0];
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}
