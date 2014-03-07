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

class Sample
{
public:
	std::vector <Face> faces;
	double k;
};

class Detector
{
public:
	std::vector <Face> faces;
};

class Core
{
public:
    void add_source(Source source);
    void add_sample(Sample sample);
    void add_detector(Detector detector);
    void run(void);
    void add_objects_from_file(char *fname);
    void add_source_from_file(char *fname);
    void add_sample_from_file(char *fname);
    void add_detector_from_file(char *fname);

protected:
    std::vector <Source>    sources;
    std::vector <Sample>    samples;
    std::vector <Detector>  detectors;
    Vector3d check_intersection(Face face, Vector3d ray_start, Vector3d ray_end);
};
