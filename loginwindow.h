#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class loginWindow;
}
QT_END_NAMESPACE

class loginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit loginWindow(QWidget *parent = nullptr);
    ~loginWindow();

private slots:
    void on_registerButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::loginWindow *ui;
    bool authenticateUser(const QString &username, const QString &password);
};
#endif // LOGINWINDOW_H
