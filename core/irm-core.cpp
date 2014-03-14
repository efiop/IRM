#include <iostream>
#include "Core.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
		goto usage;

    return 0;

usage:
	std::cout<<"Usage:"<<argv[0]<<"<aaaand some args>"<<std::endl;

	return 1;
}
