#include "6.cc"

int main(int argc, char **argv)
{
	date D;
	D.PLY = "13";
	D.filename = "result";
	D.gnu_command = "/usr/bin/gnuplot";
	D.gnu_file_name = "gnufile";
	D.data_file = "DA";
	D.pic_name = "pic";
	D.height = 10;
	D.width = 10;
	D.pthread_count = 4;
	D.koef = 1;

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
				break;
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
	double x = example_filling_obj(D);
	if (drawflag)
		draw(D.filename, D.gnu_file_name,
		D.pic_name,D.gnu_command,x);
	if (showflag)
		if (fork() == 0)
			execl("/usr/bin/feh", "feh",D.pic_name.c_str()
			, NULL);
	return 0;
}
