#include <iostream>
#include <stdio.h>
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

struct Screen_dot_impotant_struct {
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

void *pthread_f(void *arg)
{
	pthread_arg *argum = (pthread_arg*)arg;
	int amount = o.face.size();
	vector<double> length;
	double x;
	int k;
	int minus;
	int percent = 0;
	for (int j = 0;j < argum->amount;j++) {
		if (argum->det) {
//			cout << "\r" << ((double)j/(double)(
//			argum->amount)*100);
			if ((double)j/(double)(argum->amount)*100 > 
			percent) {
				percent++;
				cout << "\r" << percent;
				cout.flush();
			}
		}//state of process in %
		length.clear();
		(argum->it+j)->intense = 0;
		for (int i = 0;i < amount;i++) {
			x = polygon_segment_intersection
			(o.face[i], ((argum->it)+j)->coordinate,
			source);
			if (x == 2) {
				break;
			}
			if (x != 0)
				length.push_back(x);
		}
		sort(length.begin(), length.end());
		minus = 1;
		for (k = 0; k < length.size();k++) {
			minus *= -1;
			(argum->it+j)->intense += minus*length[k];
		}
	}
}


struct date {
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

double screen
(date D)
{
	vector<pthread_arg> pthread_arg_arr;
	vector<Screen_dot_impotant_struct> screen_dot;
	D.number = D.height*D.width;
	D.step_h = (D.Screen.dot[3]-D.Screen.dot[0])/D.height;
	D.step_w =  (D.Screen.dot[1]-D.Screen.dot[0])/D.width;
	D.partial = D.number/D.pthread_count;
	D.rest = D.number%D.pthread_count;
	D.now = 0;
	for (int i = 0;i < D.width; i++)
		for (int j = 0;j < D.height; j++) {
			screen_dot.push_back(Screen_dot_impotant_struct
			(D.Screen.dot[0]+ i*D.step_w+j*D.step_h));
	};
	for (int i = 0;i < D.rest;i++) {
		pthread_arg_arr.push_back(pthread_arg
		(screen_dot.begin() + D.now,D.partial + 1, 0));
		D.now += D.partial + 1;
	}
	for (int i = D.rest; i < D.pthread_count; i++) {
		pthread_arg_arr.push_back(pthread_arg
		(screen_dot.begin() + D.now,D.partial, 0));
		D.now += D.partial;
	}
	pthread_arg_arr[0].det = 1;
	pthread_t pth[D.pthread_count];
	cout << "Start threads" << endl;
	for (int i = 0; i < D.pthread_count;i++) {
		pthread_create(pth+i, NULL, pthread_f,
		(void*)(&(pthread_arg_arr[i])));
	}
	for (int i = 0; i < D.pthread_count;i++) {
		pthread_join(pth[i], NULL);
	}
	cout << "End threads" << endl;
	ofstream ss;
	ss.open(D.filename.c_str());
	double ch, min = 1;
	for (int i = 0;i < D.height;i++) {
		for (int j = 0; j < D.width;j++) {
			ch = exp(-D.koef*screen_dot[i*D.width+j].
			intense);
			if (ch < min) min = ch;
			ss << ch << " ";
		}
		ss << endl;
	}
	return min;
	ss.close();
}

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

void fill_struct(string filename)
{
	ifstream stream;	
	stream.open(filename.c_str());
	string s;
	string string_const1 = "element vertex";
	string string_const2 = "element face";
	string string_const3 = "end_header";
	stringstream sstream, sstream2;
	int vertexes, faces;
	vector<Vector3d> vet;
	double a,b,c;
	int k, n;
	Face f;
	for (;;) {
		getline(stream,s);
		if (stream.eof())
			break;
		if (s.find(string_const1) != string::npos) {
			sstream << string(s.begin()+
			string_const1.size(),s.end());
			sstream >> vertexes;
		}
		if (s.find(string_const2) != string::npos) {
			sstream2 << string(s.begin()+
			string_const2.size(),s.end());
			sstream2 >> faces;
		}
		if (s.find(string_const3) != string::npos) {
			for (int i = 0; i < vertexes;i++) {
				stream >> a;
				stream >> b;
				stream >> c;
				getline(stream, s);
				vet.push_back(Vector3d(a,b,c));
			}
			for (int i = 0;i < faces;i++) {
				stream >> k;
				f.dot.clear();
				while (k--) {
					stream >> n;
					f.dot.push_back(vet[n]);
				}
				o.face.push_back(f);
				getline(stream, s);
			}
		}
	}
}

void draw
(string filename, string gnu_name, string picture_name,
string gnu_place, double bottom)
{
	ofstream ss;
	ss.open(gnu_name.c_str());
	ss << "#!" << gnu_place << endl;
	ss << "set term jpeg\nset out \"" << picture_name << "\"\n";
	ss << "set size ratio 1\nset palette gray negative\n";
	ss << "set cbrange[" << bottom << ":1]\n";
	ss << "plot \"" << filename << "\" matrix w image noti";
	ss.close();
	if (fork() != 0)
		execl(gnu_place.c_str(),"gnuplot",gnu_name.c_str(),
		NULL);
	wait(NULL);
	sleep(1);
}

int init_data(date& D,string file)
{
	ifstream s;
	string str;
	s.open(file.c_str());
	getline(s, D.PLY);
	getline(s, D.filename);
	getline(s, D.gnu_command);
	getline(s, D.gnu_file_name);
	getline(s, D.data_file);
	getline(s, D.pic_name);
	s >> D.height;
	s >> D.width;
	s >> D.pthread_count;
	s >> D.koef;
	s.close();
	return 0;
}
	
	

int main(int argc, char **argv)
{
	date D;
	init_data(D, "FILE");

	int drawflag = 0, showflag = 0;
	int opt = getopt(argc, argv, "P:r:c:i:g:C:S:H:W:t:hds");
	while (opt != -1) {
		switch (opt) {
			case 'P':
				D.PLY = optarg;
				break;
			case 'r':
				D.filename = optarg;
				break;
			case 'c':
				D.gnu_command = optarg;
				break;
			case 'i':
				D.pic_name = optarg;
				break;
			case 'g':
				D.gnu_file_name = optarg;
				break;
			case 'C':
				D.koef = atoi(optarg);
				break;
			case 'S':
				D.data_file = optarg;
				break;
			case 'H':
				D.height = atoi(optarg);
				break;
			case 'W':
				D.width = atoi(optarg);
				break;
			case 't':
				D.pthread_count = atoi(optarg);
				break;
			case 'h':
				cout << TEXT << endl;
				exit(0);
			case 'd':
				drawflag = 1;
				break;
			case 's':
				showflag = 1;
				break;
		}
		opt = getopt(argc, argv, "P:r:c:i:g:C:S:H:W:t:hds");
	}
	fill_struct(D.PLY);
	double x = example_filling_obj(D);//fill object and calculate
	if (drawflag)
		draw(D.filename, D.gnu_file_name,
		D.pic_name,D.gnu_command,x);
	if (showflag) {
		if (fork() == 0)
			execl("/usr/bin/feh", "feh",D.pic_name.c_str()
			, NULL);
		wait(NULL);
	}
	return 0;
}
