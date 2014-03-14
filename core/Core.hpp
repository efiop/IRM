#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>

class Core
{
public:
    std::vector <aiScene> sources;
    std::vector <aiScene> samples;
    std::vector <aiScene> detectors;

	void Run(void);
};
