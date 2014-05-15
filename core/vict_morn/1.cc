#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define TEXT "Usage:./1 [OPTION]\n\
\t-h help\n\t-P [PLY_file]\n\t-r [result_file]\n\t-c\
[gnuplot_command]\n\t-i [gnuplt_image_file]\n\t-g [gnuplot_script]\
\n\t-C [XRAY_coefficient]\n\t-S [file with source & screen\
coordinates]\n\t-H [Screen height]\n\t-W [Screen width]\n\t-t\
 [count of pthreads]\n\t-s show picture\n\t-d draw picture"
using namespace std;
using namespace Eigen;

struct Face
{
	vector<Vector3d> dot;
};

struct obj
{
	vector<Face> face;
};

obj o;
Vector3d source;

int dot_belong_polygon(Face f, Vector3d O)
{
	return 1;
}

struct intersection_dot
{
	Vector3d r;
	int exist;
	int fail;
};

int dot_belong_polygon1(Face f, intersection_dot *O)
{
	Vector3d direction = f.dot[0]+f.dot[1]-2*O->r;	
	int k1, k2, k3,bel = 0, j;
	Vector3d a,b,c;
	O->fail = 0;
	for (int i=0;i < f.dot.size();i++) {
		if (i == f.dot.size()-1)
			j = 0;
		else
			j = i+1;
		a = (f.dot[i]-O->r).cross(f.dot[j]-O->r);
		if (a[0] == 0 && a[1] == 0 && a[2] == 0) {
			O->fail = 1;
			break;
		}
		b = (f.dot[i]-O->r).cross(direction);
		if (b[0] == 0 && b[1] == 0 && b[2] == 0) {
			O->fail = 1;
			break;
		}
		c = direction.cross(f.dot[j]-O->r);
		if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
			O->fail = 1;
			break;
		}
		k1 = 0;
		k2 = 0;
		k3 = 0;
		if (a.transpose()*b > 0)
			k1 = 1;
		if (b.transpose()*c > 0)
			k2 = 1;
		if (a.transpose()*c > 0)
			k3 = 1;
		if (k1 == k2 && k2 == k3) {
			bel = 1- bel;
		}
	}
	return bel;
}

double polygon_segment_intersection(Face f, Vector3d x,Vector3d y)
{
	Vector3d n = (f.dot[0]-f.dot[2]).cross(f.dot[0]-f.dot[1]);
	Vector3d delta = x-y;
	double l = delta.transpose()*n;
	intersection_dot O;
	O.exist = 0;
	if (l != 0) {
		double h = (f.dot[0]-y).transpose()*n;
		O.r = y+h/l*delta;
		if ((O.r-y).transpose()*(O.r-x) < 0) {
			int bel = dot_belong_polygon1(f, &O);
			if (O.fail) {
				return 2;
			}
			if (bel)
				O.exist = 1;
		}
	}
	if (O.exist) {
		return (O.r-y).norm();
	}
	else
		return 0;
}
