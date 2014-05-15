#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <eigen3/Eigen/Dense>

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

struct intersection_dot
{
	Vector3d r;
	int exist;
	int fail;
};

int dot_belong_polygon(Face f, intersection_dot *O)
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


void example()
{
	Face f;
	f.dot.push_back(Vector3d(0,0,0));
	f.dot.push_back(Vector3d(0,1,0));
	f.dot.push_back(Vector3d(0,0,1));
	Vector3d x( 0, 1.1, 0.1);
	intersection_dot O;
	O.r = x;
	int g = dot_belong_polygon(f,&O);
	if (O.fail)
		cout << "NO" << endl;
	else
		cout << g << endl;
}

int main()
{
	example();
	return 0;
}
