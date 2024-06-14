#include "registerwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "loginwindow.h"
#include "ui_registerwindow.h"

registerWindow::registerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::registerWindow)
{
    ui->setupUi(this);
}

registerWindow::~registerWindow()
{
    delete ui;
}

void registerWindow::on_loginButton_clicked()
{
    // Створюємо інстанцію registerWindow
    loginWindow *logWindow = new loginWindow();
    // Показуємо вікно реєстрації
    logWindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}

void registerWindow::saveUser(const QString &username, const QString &password)
{
    QFile file("users.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open file.");
        return;
    }

    QTextStream out(&file);
    out << username << "," << password << "\n";
    file.close();
}

void registerWindow::on_registerButton_clicked()
{
    QString username = ui->userLineEdit->text();
    QString password = ui->passworLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both username and password.");
        return;
    }

    if (userExists(username)) {
        QMessageBox::warning(this, "Error", "User already exists.");
    } else {
        saveUser(username, password);
        QMessageBox::information(this, "Success", "User registered successfully!");
        // Optionally, close the registration window and open the login window
        loginWindow *logWindow = new loginWindow();
        logWindow->show();
        this->close();
    }
}

bool registerWindow::userExists(const QString &username)
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
        if (userData.size() == 2 && userData[0] == username) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}
