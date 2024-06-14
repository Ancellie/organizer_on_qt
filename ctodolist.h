#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>

class CToDoList : public QMainWindow
{
    Q_OBJECT
public:
    CToDoList();

protected slots:
    void onAdd();
    void onRemove();
    void onChangeWidgetClicked();
    void loadTasksFromFile(const QString &fileName);
    void saveTasksToFile(const QString &fileName);

private:
    QListView* m_pwPending = nullptr;
    QListView* m_pwCompleted = nullptr;

    QAction* m_pActAdd = nullptr;
    QAction* m_pActRemove = nullptr;
};


#endif // TODOLIST_H
