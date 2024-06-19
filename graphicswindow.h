#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include <QWidget>

namespace Ui {
class graphicsWindow;
}

class GraphicsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphicsWindow(bool isGroup = false, QWidget *parent = nullptr);
    ~GraphicsWindow();

private slots:
    void on_pushButton_clicked();
    void on_changeButton_clicked();

    void on_tableWidget_cellChanged(int row, int column);
    void redraw();
    void updatePlot();
    void saveTable();
    void loadTable();

    void on_removeButton_clicked();

    void on_addButton_clicked();

    void on_graphicsWindow_destroyed();

private:
    bool isGroup;
    Ui::graphicsWindow *ui;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // GRAPHICSWINDOW_H
