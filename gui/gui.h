#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <dialog.h>
#include <QFileDialog>
#include <QKeyEvent>

namespace Ui {
class GUI;
}

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = 0);
    ~GUI();

private slots:
    void on_pushButton_clicked();

    void on_spinBox1_valueChanged(int value);

    void on_spinBox2_valueChanged(int value);

    void on_actionNew_project_triggered();

    void on_actionLoad_project_triggered();

    void on_actionExit_triggered();

    void on_actionImport_model_triggered();

 // void clf();

    void on_actionLearn_attenation_triggered();

signals:
    void widthChanged(int newValue);
    void heightChanged(int newValue);

private:
    Ui::GUI *ui;
    QString fname;
};

#endif // GUI_H
