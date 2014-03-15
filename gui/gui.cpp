#include "gui.hpp"

GUI::GUI(int *argc, char **argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("IRM");

	glutReshapeFunc(GUI::Reshape);
	glutDisplayFunc(GUI::Display);

	glClearColor(0.3, 0.3, 0.3, 0.0);

	glEnable(GL_DEPTH_TEST);
}

void GUI::Display(void)
{
}

void GUI::Reshape(int w, int h)
{
}

void GUI::Run(void)
{
	glutMainLoop();
}
