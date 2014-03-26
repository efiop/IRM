#include <GL/glew.h>
#include "glwidget.h"
#include <QKeyEvent>
#include <QDebug>
#include <QPoint>
#include <QMouseEvent>

#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/Importer.hpp>

#include <iostream>
#include <fstream>

#include <GL/glu.h>

#include <GL/gl.h>
#include <GL/glut.h>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);

    //Set focus to allow widget to use keyboard
    this->setFocus();

    //Grab mouse
    this->setMouseTracking(true);

    // Hide cursor
    this->setCursor( Qt::BlankCursor );
}

void GLWidget::initializeGL()
{
    glClearColor(0.1f,0.1f,0.1f,1.f);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    // Enable lightingww
    glEnable(GL_LIGHTING);
    GLfloat  lightPos[] = { 30.0f, -30.0f, -30.0f, 1.0f };
    // Ambient, diffuse and specular lighting values (note that these are ALL FOUR COMPONENT VECTORS!)
    GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    GLint specularMagnitude = 64; // Define how "tight" our specular highlights are (larger number = smaller specular highlight). The valid range is is 1 to 128

    // Setup and enable light 0
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);          // Specify the position of the light
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambientLight);      // Specify ambient light properties
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseLight);      // Specify diffuse light properties
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);     // Specify specular light properties
    glEnable(GL_LIGHT0);

    // Enable colour tracking of materials
    glEnable(GL_COLOR_MATERIAL);

    // Define the shininess of the material we'll use to draw things
    GLfloat materialSpecularReflectance[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    // Use our shiny material and magnitude
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecularReflectance);
    glMateriali(GL_FRONT, GL_SHININESS, specularMagnitude);
}

void GLWidget::resizeGL(int width, int height)
{
    camera.aspect_ratio = (float) width / height;
    camera.field_of_view = 45.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.field_of_view, camera.aspect_ratio, camera.near, camera.far);
    glViewport(0, 0, width, height);
}

void GLWidget::draw_ground()
{
    GLfloat extent      = 600.0f; // How far on the Z-Axis and X-Axis the ground extends
    GLfloat stepSize    = 20.0f;  // The size of the separation between points
    GLfloat groundLevel = -50.0f; // Where on the Y-Axis the ground is drawn

    glLineWidth(2.0f);
    glColor3ub(255, 255, 255);

    glBegin(GL_LINES);
    for (GLint loop = -extent; loop < extent; loop += stepSize)
    {
        // Draw lines along Z-Axis
        glVertex3f(loop, groundLevel,  extent);
        glVertex3f(loop, groundLevel, -extent);

        // Draw lines across X-Axis
        glVertex3f(-extent, groundLevel, loop);
        glVertex3f(extent,  groundLevel, loop);
    }
    glEnd();
}

void GLWidget::paintGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.move();

    // Draw floor and ceiling
    draw_ground();
    glPushMatrix();
    glTranslatef(0.0f, 200.0f, 0.0f);
    draw_ground();
    glPopMatrix();

    // Draw models
    if (!this->scene)
        glutSolidCube(10);
}

bool GLWidget::import(const std::string &pFile)
{
    std::ifstream fin(pFile.c_str());
    if(!fin.fail()) {
        fin.close();
    } else {
        printf("Couldn't open file: %s\n", pFile.c_str());
        printf("%s\n", this->importer.GetErrorString());
        return false;
    }

    this->scene = this->importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);

    if( !this->scene)
    {
        printf("%s\n", this->importer.GetErrorString());
        return false;
    }

    printf("Import of scene %s succeeded.",pFile.c_str());

    return true;
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    qDebug() << "pressed";

    if (event->key() == Qt::Key_W)
        camera.holdingForward = true;

    if (event->key() == Qt::Key_S)
        camera.holdingBackward = true;

    if (event->key() == Qt::Key_A)
        camera.holdingLeftStrafe = true;

    if (event->key() == Qt::Key_D)
        camera.holdingRightStrafe = true;

    event->accept();
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_W)
        camera.holdingForward = false;

    if (event->key() == Qt::Key_S)
        camera.holdingBackward = false;

    if (event->key() == Qt::Key_A)
        camera.holdingLeftStrafe = false;

    if (event->key() == Qt::Key_D)
        camera.holdingRightStrafe = false;

    event->accept();
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int mouseX = event->pos().x();
    int mouseY = event->pos().y();

    GLfloat vertMouseSensitivity  = 50.0f;
    GLfloat horizMouseSensitivity = 50.0f;

    int horizMovement = mouseX - this->width()/2;
    int vertMovement  = mouseY - this->height()/2;

    camera.XRot += vertMovement / vertMouseSensitivity;
    camera.YRot += horizMovement / horizMouseSensitivity;

    if (camera.XRot < -90.0f)
         camera.XRot = -90.0f;

    if (camera.XRot > 90.0f)
    camera.XRot = 90.0f;

    if (camera.YRot < -180.0f)
      camera.YRot += 360.0f;

    if (camera.YRot > 180.0f)
        camera.YRot -= 360.0f;

    QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
    QCursor::setPos(glob);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
}


