#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QPoint>

#include "camera.h"
#include <string>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <sstream>
#include <fstream>
#include <QKeyEvent>
#include <QDebug>
#include <QPoint>
#include <QMouseEvent>
#include <QtGui>


struct MyPoint{
    float x,y,z;
};

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    bool import(const std::string& pFile);
    int zoom;
    int screen_width;
    int screen_height;
    GLfloat transform_matrix[16];

signals:
//void clearf();
public slots:

    void setWidth(int value);
    void setHeight(int value);

private:

    QTimer timer;

    Camera camera;

    int light_draw_permission;
    MyPoint light;
    void draw_light();

    int screen_draw_permission;
    MyPoint left_bottom, right_bottom, left_top, right_top, center;
    float x_rotation,  y_rotation, z_rotation;
    void draw_screen();

    void draw_ground(void);
    void draw_my(void);
    int mmy;
    MyPoint multiply_matrix(MyPoint vect);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // GLWIDGET_H
