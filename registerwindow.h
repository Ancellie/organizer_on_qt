#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class registerWindow;
}
QT_END_NAMESPACE

class registerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit registerWindow(QWidget *parent = nullptr);
    ~registerWindow();

private slots:
    void on_loginButton_clicked();

    void on_registerButton_clicked();

private:
    Ui::registerWindow *ui;
    void saveUser(const QString &username, const QString &password);
    bool userExists(const QString &username);
};

#endif // REGISTERWINDOW_H
