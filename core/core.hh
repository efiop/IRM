#include <iostream>
#include <vector>
#include <Eigen>

typedef std::vector <Matrix3d::Vector3d> face_t;

class Color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}

class Face
{
	std::vector <Matrix3d::Vector3d> vertices;
	Color color;
}

class Source
{
	Matrix3d::Vector3d r;
	double intencity;
}

std::vector <Source> Sources;

class Sample
{
	std::vector <Face> faces;
	double k;
}

std::vector <Sample> Samples;

class Detector
{
	std::vector <Face> faces;
}

std::vector <Detector> Detectors;
