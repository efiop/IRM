#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>

using namespace Eigen;
using namespace std;

class Color
{
public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

class Face
{
public:
	std::vector <Vector3d> vertices;
	Color color;
};

class Source
{
public:
	Vector3d r;
	double intencity;
};

extern std::vector <Source> sources;

class Sample
{
public:
	std::vector <Face> faces;
	double k;
};

extern std::vector <Sample> samples;

class Detector
{
public:
	std::vector <Face> faces;
};

extern std::vector <Detector> detectors;
