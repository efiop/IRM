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

struct Screen_dot_impotant_struct
{
	Screen_dot_impotant_struct(Vector3d a) {
		coordinate = a;
	}
	Vector3d coordinate;
	double intense;
};

struct pthread_arg
{
	pthread_arg
	(vector<Screen_dot_impotant_struct>::iterator it1, int amount1
	,int det1)
	{
		it = it1;
		amount = amount1;
		det = det1;
	}
	vector<Screen_dot_impotant_struct>::iterator it;
	int amount;
	int det;
};

struct data
{
	int height, width;
	int number;
	int pthread_count;
	int partial;
	int rest;
	int now;
	double koef;
	string data_file,PLY,filename, gnu_command, gnu_file_name,
	pic_name;
	Face Screen;
	Vector3d step_h;
	Vector3d step_w;
};


