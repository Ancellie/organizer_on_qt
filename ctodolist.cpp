#include "ctodolist.h"
#include <QLabel>
#include <QToolBar>
#include <QBoxLayout>
#include <QPushButton>
#include "mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QDebug>

CToDoList::CToDoList()
{
    QWidget* pWidget = new QWidget(this);
    pWidget->setStyleSheet("background-color: #ECF0F1");
    setCentralWidget(pWidget);

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pWidget->setLayout(pMainLayout);

    QLabel* pwTitle = new QLabel("To Do List", this);
    pMainLayout->addWidget(pwTitle);
    pwTitle->setAlignment(Qt::AlignCenter);
    pwTitle->setStyleSheet("font-size: 30pt; margin: 10%;");

    QHBoxLayout* pHLayoutLabels = new QHBoxLayout();
    pMainLayout->addLayout(pHLayoutLabels);

    QLabel* plblPending = new QLabel("Pending", this);
    plblPending->setStyleSheet("font-size: 15pt;");
    pHLayoutLabels->addWidget(plblPending);

    QLabel* plblCompleted = new QLabel("Completed", this);
    plblCompleted->setStyleSheet("font-size: 15pt;");
    pHLayoutLabels->addWidget(plblCompleted);

    QHBoxLayout* pHLayout = new QHBoxLayout();
    pMainLayout->addLayout(pHLayout);

    m_pwPending = new QListView(this);
    m_pwPending->setDragEnabled(true);
    m_pwPending->setAcceptDrops(true);
    m_pwPending->setDropIndicatorShown(true);
    m_pwPending->setDefaultDropAction(Qt::MoveAction);
    pHLayout->addWidget(m_pwPending);

    m_pwCompleted = new QListView(this);
    m_pwCompleted->setDragEnabled(true);
    m_pwCompleted->setAcceptDrops(true);
    m_pwCompleted->setDropIndicatorShown(true);
    m_pwCompleted->setDefaultDropAction(Qt::MoveAction);
    pHLayout->addWidget(m_pwCompleted);

    m_pwPending->setModel(new QStringListModel());
    m_pwCompleted->setModel(new QStringListModel());

    m_pwPending->setStyleSheet
        ("QListView { font-size: 20pt; font-weight: bold; }"
         "QListView::item { background-color: #E74C3C; padding: 10%;"
         "border: 1px solid #C0392B; }"
         "QListView::item::hover { background-color: #C0392B }");

    m_pwCompleted->setStyleSheet
        ("QListView { font-size: 20pt; font-weight: bold; }"
         "QListView::item { background-color: #2ECC71; padding: 10%;"
         "border: 1px solid #27AE60; }"
         "QListView::item::hover { background-color: #27AE60 }");

    QToolBar* pToolBar = new QToolBar(this);
    addToolBar(pToolBar);

    m_pActAdd = new QAction(this);
    m_pActAdd->setIcon(QIcon("D:/qt/organizer_on_qt/static/add.jpg"));
    connect(m_pActAdd, &QAction::triggered, this, &CToDoList::onAdd);

    m_pActRemove = new QAction(this);
    m_pActRemove->setIcon(QIcon("D:/qt/organizer_on_qt/static/remove.png"));
    connect(m_pActRemove, &QAction::triggered, this, &CToDoList::onRemove);

    pToolBar->addAction(m_pActAdd);
    pToolBar->addAction(m_pActRemove);

    QPushButton* changeWidgetButton = new QPushButton("Back to manu", this);
    connect(changeWidgetButton, &QPushButton::clicked, this, &CToDoList::onChangeWidgetClicked);
    pMainLayout->addWidget(changeWidgetButton);

    loadTasksFromFile("tasks/tasks.txt");
}

void CToDoList::onAdd()
{
    m_pwPending->model()->insertRow(m_pwPending->model()->rowCount());
    QModelIndex oIndex = m_pwPending->model()->index(
        m_pwPending->model()->rowCount() - 1, 0);
    m_pwPending->edit(oIndex);
}

void CToDoList::onRemove()
{
    QModelIndex oIndex = m_pwPending->currentIndex();
    if (oIndex.isValid()) {
        m_pwPending->model()->removeRow(oIndex.row());
    } else {
        oIndex = m_pwCompleted->currentIndex();
        if (oIndex.isValid()) {
            m_pwCompleted->model()->removeRow(oIndex.row());
        }
    }
}

void CToDoList::onChangeWidgetClicked()
{
    saveTasksToFile("tasks/tasks.txt");
    mainWindow *mainwindow = new mainWindow();
    mainwindow->show();
    this->close();
}

void CToDoList::loadTasksFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << fileName;
        return;
    }

    QTextStream in(&file);

    QStringList pendingTasks;
    QStringList completedTasks;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 2) {
            QString task = parts[0];
            QString status = parts[1].trimmed();

            if (status == "pending") {
                pendingTasks << task;
            } else if (status == "completed") {
                completedTasks << task;
            }
        }
    }
    file.close();

    static_cast<QStringListModel*>(m_pwPending->model())->setStringList(pendingTasks);
    static_cast<QStringListModel*>(m_pwCompleted->model())->setStringList(completedTasks);

    qDebug() << "Loaded tasks from file:" << fileName;
    qDebug() << "Pending tasks:" << pendingTasks;
    qDebug() << "Completed tasks:" << completedTasks;
}

void CToDoList::saveTasksToFile(const QString &fileName)
{
    QDir dir;
    if (!dir.exists("tasks")) {
        dir.mkpath("tasks");
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << fileName;
        return;
    }

    QTextStream out(&file);

    QStringList pendingTasks = static_cast<QStringListModel*>(m_pwPending->model())->stringList();
    QStringList completedTasks = static_cast<QStringListModel*>(m_pwCompleted->model())->stringList();

    for (const QString &task : pendingTasks) {
        out << task << ", pending\n";
    }

    for (const QString &task : completedTasks) {
        out << task << ", completed\n";
    }

    file.close();

    qDebug() << "Saved tasks to file:" << fileName;
    qDebug() << "Pending tasks:" << pendingTasks;
    qDebug() << "Completed tasks:" << completedTasks;
}
