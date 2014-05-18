#include "gui.h"
#include "ui_gui.h"

#include <QString>
#include <QFileDialog>
GUI::GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);
}

GUI::~GUI()
{
    delete ui;
}

void GUI::on_actionNew_project_triggered()
{

}

void GUI::on_actionLoad_project_triggered()
{

}

void GUI::on_actionImport_model_triggered()
{
    QString fname = QFileDialog::getOpenFileName(
                this,
                tr("Import model"),
                "/",
                "All files(*.*)");

   ui->widget->import(fname.toStdString());
}
