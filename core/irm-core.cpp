#include <iostream>
#include "Core.hpp"

/*
 * FIXME consider using getopt(3) to parse CLI options.
 * Consider using something like this:
 * "irm-core --source [files] --sample [files] --detector [files]"
 */


int main(int argc, char *argv[])
{
	if (argc != 2)
		goto usage;

    return 0;

usage:
	std::cout<<"Usage:"<<argv[0]<<"<aaaand some args>"<<std::endl;

	return 1;
}
