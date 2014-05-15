#include "3.cc"

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
