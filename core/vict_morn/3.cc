#include "2.cc"

double example_filling_obj(date& D)
{
	ifstream stream1;
	stream1.open(D.data_file.c_str());

	double a, b, c;
	stream1 >> a >> b >> c;

	source = Vector3d(a, b, c);

	stream1 >> a >> b >> c;
	D.Screen.dot.push_back(Vector3d(a, b, c));

	stream1 >> a >> b >> c;
	D.Screen.dot.push_back(Vector3d(a, b, c));

	stream1 >> a >> b >> c;
	D.Screen.dot.push_back(Vector3d(a, b, c));

	stream1 >> a >> b >> c;
	D.Screen.dot.push_back(Vector3d(a, b, c));

	stream1.close();
	return screen(D);
}
