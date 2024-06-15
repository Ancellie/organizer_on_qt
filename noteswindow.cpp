#include "noteswindow.h"
#include "mainwindow.h"
#include "ui_noteswindow.h"
#include "userdata.h" // Якщо у вас є цей файл для отримання UserData::username
#include <QTextStream>
#include <QDebug>
#include <QCloseEvent>

notesWindow::notesWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::notesWindow)
    , listWidget(new QListWidget(this))
{
    ui->setupUi(this);

    // Додаємо listWidget до головного макета
    ui->verticalLayout->addWidget(listWidget);

    // Завантажуємо нотатки
    loadNotes();
}

notesWindow::~notesWindow()
{
    delete ui;
}

void notesWindow::closeEvent(QCloseEvent *event)
{
    saveNotes();
    event->accept();
}

void notesWindow::on_backButton_clicked()
{
    mainWindow *mainwindow = new mainWindow();
    mainwindow->show();
    this->close();
}

void notesWindow::on_lineEdit_textChanged(const QString &text)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    if (!lineEdit) return;

    // Знайти рядок, який містить цей lineEdit
    int row = -1;
    for (int i = 0; i < listWidget->count(); ++i) {
        if (listWidget->itemWidget(listWidget->item(i)) == lineEdit) {
            row = i;
            break;
        }
    }

    if (row == -1) return;

    if (!text.isEmpty() && row == listWidget->count() - 1) {
        addLineEdit();
    } else if (text.isEmpty() && row < listWidget->count() - 1) {
        removeLineEdit(row);
    }
}

void notesWindow::addLineEdit(const QString &text)
{
    QLineEdit *lineEdit = new QLineEdit(this);
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    listWidget->setItemWidget(item, lineEdit);
    lineEdit->setText(text);

    connect(lineEdit, &QLineEdit::textChanged, this, &notesWindow::on_lineEdit_textChanged);
}

void notesWindow::removeLineEdit(int row)
{
    QListWidgetItem *item = listWidget->takeItem(row);
    QWidget *widget = listWidget->itemWidget(item);
    delete widget;
    delete item;
}

void notesWindow::loadNotes()
{
    QString filePath = "notes/" + UserData::username + ".txt";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading: " << filePath;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        addLineEdit(line);
    }

    file.close();

    // Додаємо пустий lineEdit якщо список не порожній
    if (listWidget->count() == 0 || !static_cast<QLineEdit*>(listWidget->itemWidget(listWidget->item(listWidget->count() - 1)))->text().isEmpty())
    {
        addLineEdit();
    }
}

void notesWindow::saveNotes()
{
    QString dirPath = "notes";
    QDir dir;

    if (!dir.exists(dirPath))
    {
        if (!dir.mkpath(dirPath))
        {
            qDebug() << "Failed to create directory: " << dirPath;
            return;
        }
    }

    QString filePath = dirPath + "/" + UserData::username + ".txt";
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writing: " << filePath;
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < listWidget->count(); ++i)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(listWidget->itemWidget(listWidget->item(i)));
        if (lineEdit && !lineEdit->text().isEmpty())
        {
            out << lineEdit->text() << "\n";
        }
    }

    file.close();
}
