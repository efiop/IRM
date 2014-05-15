#include "5.cc"

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

}
