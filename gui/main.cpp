#include "gui.h"
#include <QApplication>
#include <GL/glut.h>

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    GUI w;
    w.show();

    return a.exec();
}