#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QWidget>
#include <QCalendarWidget>
#include <QInputDialog>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class CalendarWindow; }
QT_END_NAMESPACE

class CalendarWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarWindow(QWidget *parent = nullptr);
    ~CalendarWindow();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleDateDoubleClicked(const QDate &date);
    void handleContextMenu(const QPoint &pos);

    void on_backButton_clicked();

private:
    Ui::CalendarWindow *ui;
    QMap<QDate, QString> bookmarks; // Зберігання закладок
    QMap<QDate, QTextCharFormat> originalFormats; // Зберігання оригінальних форматів

    void addBookmark(const QDate &date, const QString &name);
    void removeBookmark(const QDate &date);
    void showEventDetails(const QDate &date);
    void saveBookmarks();
    void loadBookmarks();
};

#endif // CALENDARWINDOW_H
