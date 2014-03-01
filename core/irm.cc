#include <iostream>
#include "parse_input_file.hh"
#include "core.hh"

int main( int argc, char *argv[])
{
	if (argc != 2)
		goto usage;

	if (parse_input_file(argv[1]))
		return 1;

    return 0;

usage:
	std::cout<<"Usage:"<<argv[0]<<"<aaaand some args>"<<endl;

	return 1;
}
