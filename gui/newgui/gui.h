#ifndef GUI_H
#define GUI_H

#include <QMainWindow>

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
    void on_actionNew_project_triggered();

    void on_actionLoad_project_triggered();

    void on_actionImport_model_triggered();

private:
    Ui::GUI *ui;
};

#endif // GUI_H
