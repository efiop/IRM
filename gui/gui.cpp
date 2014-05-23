#include "gui.h"
#include "ui_gui.h"
#include "glwidget.h"
#include "dialog.h"
#include "ui_dialog.h"

#include <QString>
#include <QFileDialog>
#include <QWidget>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

#define CORE "core/core"
#define CORE_NAME "IRM_core"
#define KEY1 "-sdR"
#define KEY2 "-P"

GUI::GUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI)
{
    ui->setupUi(this);
    connect(this, SIGNAL(widthChanged(int)), ui->widget, SLOT(setWidth(int)));
    connect(this, SIGNAL(heightChanged(int)), ui->widget, SLOT(setHeight(int)));
    /*connect(ui->widget, SIGNAL(clearf()), this, SLOT(clf()));*/
    ui->verticalLayout->setAlignment(Qt::AlignRight);
}

GUI::~GUI()
{
    delete ui;
}

void GUI::on_spinBox1_valueChanged(int value)
{
    emit widthChanged(value);
}

void GUI::on_spinBox2_valueChanged(int value)
{
    emit heightChanged(value);
}
void GUI::on_actionExit_triggered()
{
    exit(0);
}

void GUI::on_actionNew_project_triggered()
{

}

void GUI::on_actionLoad_project_triggered()
{

}

void GUI::on_actionImport_model_triggered()
{
    fname = QFileDialog::getOpenFileName(
                this,
                tr("Import model"),
                "/",
                "All files(*.*)");

   ui->widget->import(fname.toStdString());
}

void GUI::on_pushButton_clicked()
{
//	int p = 0;
    if (fork() == 0) {
        if (execl(CORE,CORE_NAME,KEY1,KEY2,fname.toStdString().c_str(),NULL) == -1) {
            std::cerr << "can't turn on core\n";
//			p = 1;
            exit(0);
		}
	}
    wait(NULL);
	cout << CORE << CORE_NAME << KEY1 << KEY2 << fname.toStdString();
}
/*void GUI::clf()
{
        qDebug() << "pr";
        ui->spinBox1->setFocusPolicy(Qt::NoFocus);
        //ui->spinBox2->clearFocus();
}
*/
void GUI::on_actionLearn_attenation_triggered()
{
    Dialog *dial=new Dialog(this);
    dial->show();
}
