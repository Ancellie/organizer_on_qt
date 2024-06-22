#include "graphicswindow.h"
#include "mainwindow.h"
#include "ui_graphicswindow.h"
#include "userdata.h"
#include <QDateTime>

GraphicsWindow::GraphicsWindow(bool isGroup, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::graphicsWindow),
    isGroup(isGroup)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);


    loadTable();
    updatePlot();
}

void GraphicsWindow::closeEvent(QCloseEvent *event)
{
    saveTable();
}

void GraphicsWindow::loadTable()
{
    // Clear the existing data in the table
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QString filepath = "tables/" + UserData::username + ".csv";
    if(isGroup){
        filepath = "tables.csv";
        QFile file1("tables.pub");
        if (file1.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file1);
            QString firstLine = in.readLine();
            ui->label_2->setText(firstLine);
            file1.close();
        }
    }
    QFile file(filepath);
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


void GraphicsWindow::saveTable()
{
    QDir tablesDir = QString::fromStdString("/tables");
    if (!tablesDir.exists()) {
        QDir::current().mkdir("tables");
    }
    QString filepath = "tables/" + UserData::username + ".csv";
    if(isGroup){
        filepath = "tables.csv";
        QFile log("tables.pub");
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
    QFile file(filepath);
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


GraphicsWindow::~GraphicsWindow()
{
    delete ui;
}

void GraphicsWindow::on_pushButton_clicked()
{
    // Створюємо інстанцію registerWindow
    mainWindow *mainwindow = new mainWindow();
    // Показуємо вікно реєстрації
    mainwindow->show();
    // Закриваємо вікно логіну (опціонально)
    this->close();
}

void GraphicsWindow::on_tableWidget_cellChanged(int row, int column)
{
    updatePlot();
}

void GraphicsWindow::updatePlot()
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

void GraphicsWindow::redraw() {
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}



void GraphicsWindow::on_removeButton_clicked()
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


void GraphicsWindow::on_addButton_clicked()
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


void GraphicsWindow::on_graphicsWindow_destroyed()
{
    saveTable();
}


void GraphicsWindow::on_changeButton_clicked()
{
    QString text = isGroup ? "Change to private" : "Change to public";
    ui->changeButton->setText(text);
    saveTable();
    isGroup = !isGroup;
    loadTable();
    updatePlot();
}


void GraphicsWindow::on_avgBtn_clicked()
{
    bool ok;
    QString rowName = QInputDialog::getText(this, tr("Get average value"),
                                            tr("Row name:"), QLineEdit::Normal,
                                            tr("Subject"), &ok);

    if (!ok || rowName.isEmpty()) return;

    int index = getRow(rowName);

    if (index == -1) {
        QMessageBox::warning(this, "Error", "Row not found.");
        return;
    }

    double average = 0;
    int itemCount = 0;

    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        auto item = ui->tableWidget->item(index, i);
        if (!item || item->text().isEmpty()) continue;
        bool isDouble;
        double value = item->text().toDouble(&isDouble);
        if (!isDouble) continue;
        average += value;
        itemCount++;
    }
    QString outMsg = QString("Average grade for %1 is %2").arg(rowName).arg(average/itemCount, 0, 'f', 3);

    QMessageBox::information(this, "Data", outMsg);
}


void GraphicsWindow::on_cmpBtn_clicked()
{
    bool ok;
    QString firstRowName = QInputDialog::getText(this, tr("Compare"),
                                            tr("Base row name:"), QLineEdit::Normal,
                                            tr("Subject"), &ok);
    if (!ok || firstRowName.isEmpty()) return;
    QString secondRowName = QInputDialog::getText(this, tr("Compare"),
                                             tr("Compare row name:"), QLineEdit::Normal,
                                             tr("Subject"), &ok);
    if (!ok || secondRowName.isEmpty()) return;

    int firstRow = getRow(firstRowName);
    int secondRow = getRow(secondRowName);

    if (firstRow == -1 || secondRow == -1) {
        QMessageBox::warning(this, "Error", "Row not found.");
        return;
    }

    double fICount = 0, sICount = 0;

    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        auto fItem = ui->tableWidget->item(firstRow, i);
        auto sItem = ui->tableWidget->item(secondRow, i);

        if (fItem && !fItem->text().isEmpty()) {
            fItem->text().toDouble(&ok);
            if (ok) fICount++;
        }
        if (sItem && !sItem->text().isEmpty()) {
            sItem->text().toDouble(&ok);
            if (ok) sICount++;
        }
    }

    double diff = fICount / sICount * 100;
    QString diffDir = diff < 0 ? "less" : "more";
    QString msg = QString("%1 has %2\% %3 marks than %4")
        .arg(firstRowName)
        .arg(diff, 0, 'f', 2)
        .arg(diffDir)
        .arg(secondRow);
    QMessageBox::information(this, "Data", msg);
}

int GraphicsWindow::getRow(QString name) {
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        if (ui->tableWidget->verticalHeaderItem(i)->text() != name) continue;

        return i;
    }

    return -1;
}

