#include "1.cc"

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
	(vector<Screen_dot_impotant_struct>::iterator it1, int amount1)
	{
		it = it1;
		amount = amount1;
	}
	vector<Screen_dot_impotant_struct>::iterator it;
	int amount;
};

void *pthread_f(void *arg)
{
	pthread_arg *argum = (pthread_arg*)arg;
	int amount = o.face.size();
	vector<double> length;
	double x;
	int k;
	int minus;
	for (int j = 0;j < argum->amount;j++) {
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
		(screen_dot.begin() + D.now,D.partial + 1));
		D.now += D.partial + 1;
	}
	for (int i = D.rest; i < D.pthread_count; i++) {
		pthread_arg_arr.push_back(pthread_arg
		(screen_dot.begin() + D.now,D.partial));
		D.now += D.partial;
	}
	pthread_t pth[D.pthread_count];
	for (int i = 0; i < D.pthread_count;i++) {
		pthread_create(pth+i, NULL, pthread_f,
		(void*)(&(pthread_arg_arr[i])));
	}
	for (int i = 0; i < D.pthread_count;i++) {
		pthread_join(pth[i], NULL);
	}
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
