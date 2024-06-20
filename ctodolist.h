#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QLabel>

class CToDoList : public QMainWindow
{
    Q_OBJECT
public:
    CToDoList();

protected slots:
    void onAdd();
    void onRemove();
    void onAddURL();
    void onChangeWidgetClicked();
    void onChangeModeClicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QAction* m_pActAdd;
    QAction* m_pActRemove;
    QAction* m_pActAddURL;  // Add this line for the new action
    QListView* m_pwPending;
    QListView* m_pwCompleted;
    QString m_userURL;  // Add this line to store the URL

    QString getUserTasksFilePath() const;
    void loadTasksFromFile(const QString &fileName);
    void saveTasksToFile(const QString &fileName);
    QLabel* modeLabel;  // Додано поле для відображення поточного режиму
    bool isGroup;  // Додано поле для відстеження поточного режиму
};

#endif // TODOLIST_H
