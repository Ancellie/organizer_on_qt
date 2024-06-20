#include "ctodolist.h"
#include <QToolBar>
#include <QBoxLayout>
#include <QPushButton>
#include "mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include "userdata.h"
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QInputDialog>
#include <QDesktopServices>
#include <QDateTimeEdit>
#include <QRegularExpression>>

CToDoList::CToDoList()
{
    QWidget* pWidget = new QWidget(this);
    setCentralWidget(pWidget);

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pWidget->setLayout(pMainLayout);

    QLabel* pwTitle = new QLabel("To Do List", this);
    pMainLayout->addWidget(pwTitle);
    pwTitle->setAlignment(Qt::AlignCenter);
    pwTitle->setStyleSheet("font-size: 30pt; margin: 10%;");

    QLabel* modeLabel = new QLabel("Mode: Private", this);  // Додано label для відображення режиму
    pMainLayout->addWidget(modeLabel);
    modeLabel->setAlignment(Qt::AlignRight);
    modeLabel->setStyleSheet("font-size: 12pt; margin-right: 10%;");

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
    m_pActAdd->setIcon(QIcon("static/add.jpg"));
    connect(m_pActAdd, &QAction::triggered, this, &CToDoList::onAdd);

    m_pActRemove = new QAction(this);
    m_pActRemove->setIcon(QIcon("static/remove.png"));
    connect(m_pActRemove, &QAction::triggered, this, &CToDoList::onRemove);

    m_pActAddURL = new QAction(this);
    m_pActAddURL->setIcon(QIcon("static/add_url.png")); // Assuming you have an icon for the URL
    connect(m_pActAddURL, &QAction::triggered, this, &CToDoList::onAddURL);

    pToolBar->addAction(m_pActAddURL);

    pToolBar->addAction(m_pActAdd);
    pToolBar->addAction(m_pActRemove);

    QPushButton* changeModeButton = new QPushButton("Open docs", this);
    connect(changeModeButton, &QPushButton::clicked, this, &CToDoList::onChangeModeClicked);
    pMainLayout->addWidget(changeModeButton);

    QPushButton* changeWidgetButton = new QPushButton("Back to menu", this);
    connect(changeWidgetButton, &QPushButton::clicked, this, &CToDoList::onChangeWidgetClicked);
    pMainLayout->addWidget(changeWidgetButton);

    resize(800, 600);

    loadTasksFromFile(getUserTasksFilePath());
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
    mainWindow *mainwindow = new mainWindow();
    mainwindow->show();
    this->close();
}

void CToDoList::closeEvent(QCloseEvent *event)
{
    saveTasksToFile(getUserTasksFilePath());
    QWidget::closeEvent(event);
}

void CToDoList::onAdd()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add Task"),
                                         tr("Task description:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        int days = QInputDialog::getInt(this, tr("Set Due Date"),
                                        tr("Enter number of days for this task:"), 1, 1, 365);

        QString dueDateString = QString::number(days);
        QString taskWithDate;
        if (days == 1) {
            taskWithDate = "!||" + text + "||" + dueDateString;
        } else {
            taskWithDate = "o||" + text + "||" + dueDateString;
        }

        m_pwPending->model()->insertRow(m_pwPending->model()->rowCount());
        QModelIndex oIndex = m_pwPending->model()->index(m_pwPending->model()->rowCount() - 1, 0);
        m_pwPending->model()->setData(oIndex, taskWithDate);
    }
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
        if (parts.size() == 3) {
            QString task = parts[0];
            QString status = parts[1].trimmed();
            QString days = parts[2].trimmed();

            QString taskWithDate;
            int daysInt = days.toInt();
            if (daysInt == 1) {
                taskWithDate = "!||" + task + "||" + days;
            } else {
                taskWithDate = "o||" + task + "||" + days;
            }

            if (status == "pending") {
                pendingTasks << taskWithDate;
            } else if (status == "completed") {
                completedTasks << taskWithDate;
            }
        }
    }
    file.close();

    QStringListModel* pendingModel = static_cast<QStringListModel*>(m_pwPending->model());
    pendingModel->setStringList(pendingTasks);

    QStringListModel* completedModel = static_cast<QStringListModel*>(m_pwCompleted->model());
    completedModel->setStringList(completedTasks);

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

    auto saveTasks = [&](const QStringList &tasks, const QString &status) {
        for (const QString &taskWithDate : tasks) {
            QString task = taskWithDate.section("||", 1, 1).trimmed();
            QString days = taskWithDate.section("||", 2, 2).trimmed();
            out << task << ", " << status << ", " << days << "\n";
        }
    };

    saveTasks(pendingTasks, "pending");
    saveTasks(completedTasks, "completed");

    file.close();

    qDebug() << "Saved tasks to file:" << fileName;
    qDebug() << "Pending tasks:" << pendingTasks;
    qDebug() << "Completed tasks:" << completedTasks;
}



QString CToDoList::getUserTasksFilePath() const
{
    // Враховуємо режим для збереження завдань у різних файлах
    QString fileName = UserData::username + ".csv";
    return "tasks/" + fileName;
}

void CToDoList::onChangeModeClicked()
{
    QFile file("docsURL");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << "docsURL";
        QMessageBox::warning(this, tr("Error"), tr("Could not open the file to read the URL."));
        return;
    }

    QTextStream in(&file);
    QString url = in.readLine();
    file.close();

    if (!url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(url));
        qDebug() << "URL opened:" << url;
    } else {
        QMessageBox::warning(this, tr("Error"), tr("The URL is empty."));
    }
}

void CToDoList::onAddURL()
{
    QFile file("docsURL");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << "docsURL";
        return;
    }

    QTextStream out(&file);
    bool ok;
    QString url = QInputDialog::getText(this, tr("Add URL"),
                                        tr("URL:"), QLineEdit::Normal,
                                        "", &ok);
    if (ok && !url.isEmpty()) {
        // Regular expression for validating a URL
        QRegularExpression urlRegex("^(http|https)://");
        QRegularExpressionMatch match = urlRegex.match(url);
        if (match.hasMatch()) {
            m_userURL = url;
            qDebug() << "URL added:" << m_userURL;
            QMessageBox::information(this, tr("URL Added"), tr("URL has been added successfully!"));
            out << m_userURL;
        } else {
            qDebug() << "Invalid URL entered:" << url;
            QMessageBox::warning(this, tr("Invalid URL"), tr("The entered text is not a valid URL. Please enter a valid URL starting with http:// or https://"));
        }
    }
}
