#include "calendarwindow.h"
#include "mainwindow.h"
#include "ui_calendarwindow.h"
#include "userdata.h"

#include <QContextMenuEvent>
#include <QDate>
#include <QCloseEvent>
#include <QTextStream>

CalendarWindow::CalendarWindow(bool isGroup, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CalendarWindow),
    isGroup(isGroup)
{
    ui->setupUi(this);

    connect(ui->calendarWidget, &QCalendarWidget::activated, this, &CalendarWindow::handleDateDoubleClicked);
    ui->calendarWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->calendarWidget, &QWidget::customContextMenuRequested, this, &CalendarWindow::handleContextMenu);

    loadBookmarks();
}

CalendarWindow::~CalendarWindow()
{
    delete ui;
}

void CalendarWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QDate date = ui->calendarWidget->selectedDate();
    QMenu menu(this);
    QAction *addOrEditAction = menu.addAction(bookmarks.contains(date) ? tr("Edit Bookmark") : tr("Add Bookmark"));
    if (bookmarks.contains(date)) {
        QAction *removeAction = menu.addAction(tr("Remove Bookmark"));
        connect(removeAction, &QAction::triggered, this, [this, date]() {
            removeBookmark(date);
        });
    }

    connect(addOrEditAction, &QAction::triggered, this, [this, date]() {
        bool ok;
        QString name = QInputDialog::getText(this, bookmarks.contains(date) ? tr("Edit Bookmark") : tr("Add Bookmark"),
                                             tr("Bookmark name:"), QLineEdit::Normal,
                                             bookmarks.value(date), &ok);
        if (ok) {
            if (name.isEmpty()) {
                removeBookmark(date);
            } else {
                addBookmark(date, name);
            }
        }
    });

    menu.exec(event->globalPos());
}

void CalendarWindow::handleDateDoubleClicked(const QDate &date)
{
    showEventDetails(date);
}

void CalendarWindow::handleContextMenu(const QPoint &pos)
{
    QDate date = ui->calendarWidget->selectedDate();
    QMenu menu(this);
    QAction *addOrEditAction = menu.addAction(bookmarks.contains(date) ? tr("Edit Bookmark") : tr("Add Bookmark"));
    if (bookmarks.contains(date)) {
        QAction *removeAction = menu.addAction(tr("Remove Bookmark"));
        connect(removeAction, &QAction::triggered, this, [this, date]() {
            removeBookmark(date);
        });
    }

    connect(addOrEditAction, &QAction::triggered, this, [this, date]() {
        bool ok;
        QString name = QInputDialog::getText(this, bookmarks.contains(date) ? tr("Edit Bookmark") : tr("Add Bookmark"),
                                             tr("Bookmark name:"), QLineEdit::Normal,
                                             bookmarks.value(date), &ok);
        if (ok) {
            if (name.isEmpty()) {
                removeBookmark(date);
            } else {
                addBookmark(date, name);
            }
        }
    });

    menu.exec(ui->calendarWidget->mapToGlobal(pos));
}

void CalendarWindow::addBookmark(const QDate &date, const QString &name) {
    if (!originalFormats.contains(date)) {
        originalFormats[date] = ui->calendarWidget->dateTextFormat(date);
    }

    QTextCharFormat format = ui->calendarWidget->dateTextFormat(date);
    format.setBackground(Qt::yellow); // Колір фону для позначки
    ui->calendarWidget->setDateTextFormat(date, format);
    bookmarks[date] = name;
}

void CalendarWindow::removeBookmark(const QDate &date) {
    if (originalFormats.contains(date)) {
        ui->calendarWidget->setDateTextFormat(date, originalFormats[date]);
        originalFormats.remove(date);
    }
    bookmarks.remove(date);
}

void CalendarWindow::showEventDetails(const QDate &date)
{
    if (bookmarks.contains(date)) {
        QMessageBox::information(this, tr("Event Details"), bookmarks.value(date));
    } else {
        QMessageBox::information(this, tr("Event Details"), tr("No event for this date."));
    }
}

void CalendarWindow::saveBookmarks()
{
    QString dirPath = "events";
    QDir dir;

    if (!dir.exists(dirPath))
    {
        if (!dir.mkpath(dirPath))
        {
            qDebug() << "Failed to create directory: " << dirPath;
            return;
        }
    }

    QString filePath = dirPath + "/" + UserData::username + ".csv";

    if(isGroup){
        filePath = "events.csv";
        QFile log("events.pub");
        if (!log.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Failed to open file for writing";
            return;
        }
        QDateTime now = QDateTime::currentDateTime();
        QString formattedTime = now.toString("yyyy-MM-dd HH:mm:ss");
        QTextStream out(&log);

        out << UserData::username << "::" << formattedTime;
        log.close();
    }

    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (auto it = bookmarks.begin(); it != bookmarks.end(); ++it) {
            out << it.key().toString(Qt::ISODate) << "," << it.value() << "\n";
        }
        file.close();
    }
    else
    {
        qDebug() << "Failed to open file for writing: " << filePath;
    }
}

void CalendarWindow::loadBookmarks()
{
    QString dirPath = "events";
    QString filePath = dirPath + "/" + UserData::username + ".csv";
    if(isGroup){
        filePath = "events.csv";
        QFile file1("events.pub");
        if (file1.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file1);
            QString firstLine = in.readLine();
            ui->label_2->setText(firstLine);
            file1.close();
        }
    }
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                QDate date = QDate::fromString(parts[0], Qt::ISODate);
                QString name = parts[1];
                addBookmark(date, name);
            }
        }
        file.close();
    }
    else
    {
        qDebug() << "Failed to open file for reading: " << filePath;
    }
}

void CalendarWindow::closeEvent(QCloseEvent *event)
{
    saveBookmarks();
    QWidget::closeEvent(event);
}



void CalendarWindow::on_backButton_clicked()
{
    mainWindow *mainwindow = new mainWindow();
    mainwindow->show();
    this->close();
}

void CalendarWindow::on_changeButton_clicked()
{
    QString text = isGroup ? "Change to private" : "Change to public";
    ui->changeButton->setText(text);
    saveBookmarks(); // Зберігаємо поточні закладки перед зміною режиму
    clearAllBookmarks(); // Очищаємо всі закладки перед завантаженням нових
    isGroup = !isGroup; // Зміна режиму
    loadBookmarks(); // Завантаження нових закладок відповідно до нового режиму
}

void CalendarWindow::clearAllBookmarks()
{
    // Очищаємо всі позначки
    for (auto it = bookmarks.begin(); it != bookmarks.end(); ++it) {
        QDate date = it.key();
        if (originalFormats.contains(date)) {
            ui->calendarWidget->setDateTextFormat(date, originalFormats[date]);
            originalFormats.remove(date);
        } else {
            QTextCharFormat format;
            ui->calendarWidget->setDateTextFormat(date, format);
        }
    }
    bookmarks.clear();
}
