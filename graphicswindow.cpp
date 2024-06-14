#include "graphicswindow.h"
#include "mainwindow.h"
#include "ui_graphicswindow.h"
#include "userdata.h"

graphicsWindow::graphicsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::graphicsWindow)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    loadTable();
    updatePlot();
}

void graphicsWindow::closeEvent(QCloseEvent *event)
{
    saveTable();
}

void graphicsWindow::loadTable()
{
    QFile file("tables/" + UserData::username + ".csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading";
        return;
    }

    QTextStream in(&file);
    int row = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.isEmpty())
            continue;

        if (row >= ui->tableWidget->rowCount())
            ui->tableWidget->setRowCount(row + 1);

        ui->tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(fields.first()));

        for (int col = 1; col < fields.size(); ++col)
        {
            QTableWidgetItem *item = new QTableWidgetItem(fields.at(col));
            ui->tableWidget->setItem(row, col - 1, item);
        }

        ++row;
    }

    file.close();
}


void graphicsWindow::saveTable()
{
    QDir tablesDir = QString::fromStdString("/tables");
    if (!tablesDir.exists()) {
        QDir::current().mkdir("tables");
    }
    QFile file("tables/" + UserData::username + ".csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writing";
        return;
    }

    QTextStream out(&file);

    int rowCount = ui->tableWidget->rowCount();
    int columnCount = ui->tableWidget->columnCount();

    for (int row = 0; row < rowCount; ++row)
    {
        QString line = ui->tableWidget->verticalHeaderItem(row)->text();

        for (int col = 0; col < columnCount; ++col)
        {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item)
            {
                line += "," + item->text();
            }
            else
            {
                line += ",";
            }
        }

        out << line << "\n";
    }

    file.close();
}


graphicsWindow::~graphicsWindow()
{
    delete ui;
}

void graphicsWindow::on_pushButton_clicked()
{
    // Створюємо інстанцію registerWindow
    mainWindow *mainwindow = new mainWindow();
    // Показуємо вікно реєстрації
    mainwindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}

void graphicsWindow::on_tableWidget_cellChanged(int row, int column)
{
    updatePlot();
}

void graphicsWindow::updatePlot()
{
    ui->customPlot->clearGraphs(); // Очищуємо графіки

    int rowCount = ui->tableWidget->rowCount();
    int columnCount = ui->tableWidget->columnCount();
    QVector<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow, Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow};


    // Проходимо через всі рядки таблиці
    for (int i = 0; i < rowCount; ++i)
    {
        QVector<double> x, y;

        // Збираємо дані з таблиці
        for (int j = 0; j < columnCount; ++j)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            if (!item || item->text().isEmpty()) continue;

            x.append(j + 1); // Значення X - номер стовпця, починаючи з 1
            y.append(item->text().toDouble()); // Значення Y - вміст комірки
        }

        ui->customPlot->addGraph(); // Додаємо новий графік
        ui->customPlot->graph(i)->setData(x, y); // Встановлюємо дані для графіку
        ui->customPlot->graph(i)->setPen(QPen(colors[i % colors.size()]));
    }

    redraw();
}

void graphicsWindow::redraw() {
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}



void graphicsWindow::on_removeButton_clicked()
{
    bool ok;
    QString rowName = QInputDialog::getText(this, tr("Remove Row"),
                                               tr("Row name:"), QLineEdit::Normal,
                                               tr("Subject"), &ok);
    if (ok && !rowName.isEmpty())
    {
        for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
        {
            if (ui->tableWidget->verticalHeaderItem(i)->text() == rowName)
            {
                ui->tableWidget->removeRow(i);
                break;
            }
        }
    }
}


void graphicsWindow::on_addButton_clicked()
{
    bool ok;
    QString rowName = QInputDialog::getText(this, tr("Add Row"),
                                               tr("Row name:"), QLineEdit::Normal,
                                               tr("Subject"), &ok);
    if (ok && !rowName.isEmpty())
    {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setVerticalHeaderItem(rowCount, new QTableWidgetItem(rowName));
    }
}


void graphicsWindow::on_graphicsWindow_destroyed()
{
    saveTable();
}

