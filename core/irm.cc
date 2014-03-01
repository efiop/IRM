#include <iostream>
#include "core.hh"

int main( int argc, char *argv[])
{
    Core core;

	if (argc != 2)
		goto usage;

    core.add_objects_from_file(argv[1]);

    return 0;

usage:
	std::cout<<"Usage:"<<argv[0]<<"<aaaand some args>"<<endl;

	return 1;
}
