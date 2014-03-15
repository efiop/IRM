#include <GL/glut.h>

class GUI
{
public:
	GUI(int *argc, char **argv);
	void Run(void);

private:
	GUI(void);
	static void Display(void);
	static void Reshape(int w, int h);
};
