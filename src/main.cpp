#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <fstream>
#include "mincut.hpp"


void showMatrix(double** costMatrix, int n)
{
    for(size_t i = 0; i < n; i++)
    {
        for(size_t j = 0; j < n; j++)
        {
            std::cout << costMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char ** argv)
{
    int dim;
    std::string instanceName = argv[1];


    std::ifstream file(instanceName);


    file >> dim;


    double** dist = new double*[dim];
    for(size_t i = 0; i < dim; i++)
    {
        (dist)[i] = new double[dim];
        for(size_t j = 0; j < dim; j++)
        {   
            file >> dist[i][j];
        }
    }

    std::cout << "DIM: " << dim << std::endl;
    for(size_t i = 0; i < dim; i++)
    {
        for(size_t j = 0; j < dim; j++)
        {
            std::cout << dist[i][j];
            if(j < (dim-1))
            {
                std::cout << " ";
            }else{
                std::cout << std::endl;
            }
        }
    }



    std::cout << "Matriz de distancias: \n";
    showMatrix(dist, dim);

    Matrixd cutSetPool = minCut(dim, dist);

    showVertices(cutSetPool);

    for(size_t i = 0; i < dim; i++)
        delete[] dist[i];
    delete[] dist;


    return 0;
}