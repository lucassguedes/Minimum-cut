#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include "mincut.hpp"


void convertInput(double ***costMatrix, int &dim)
{
    /*As linhas 265 a 328 são somente para transformar a entrada para o formato de matriz*/
    int nConnections;
    char cap1, cap2;
    char island1, island2;
    double importance;
    Matrixd vertices;
    std::map<int, std::map<int, double> > distances;

    std::map<char, std::map<char, double> > connections;
    std::vector<char>unique_islands;

    std::cin >> nConnections >> cap1 >> cap2;

    insertElement(vertices, 0);
    insertElement(vertices, 1);

    for(size_t i = 0; i < nConnections; i++)
    {
        std::cin >> island1 >> island2 >> importance;

        if(std::find(unique_islands.begin(), unique_islands.end(), island1) == unique_islands.end())
            unique_islands.push_back(island1);

        if(std::find(unique_islands.begin(), unique_islands.end(), island2) == unique_islands.end())
            unique_islands.push_back(island2);


        int k, u;

        k = std::find(unique_islands.begin(), unique_islands.end(), island1) - unique_islands.begin();
        u = std::find(unique_islands.begin(), unique_islands.end(), island2) - unique_islands.begin();
        
        connections[island1][island2] = connections[island2][island1] = importance;
        distances[k][u] = distances[u][k] = importance;

        insertElement(vertices, k);
        insertElement(vertices, u);
    }

    dim = unique_islands.size();
    double** dist = new double*[dim];
    for(size_t i = 0; i < dim; i++)
    {
        (dist)[i] = new double[dim];
        for(size_t j = 0; j < dim; j++)
        {   
            if(distances[i].find(j) != distances[i].end())
                dist[i][j] = distances[i][j];
            else
                dist[i][j] = 0;
        }
    }
    *costMatrix = dist;
}

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

int main(void)
{
    int dim;
    double **dist;


    /*A entrada deve ser digitada no terminal, de acordo com o formato das
      entradas encontradas na pasta "inputs".*/
    convertInput(&dist, dim);


    std::cout << "Matriz de distancias: \n";
    showMatrix(dist, dim);

    Matrixd cutSetPool = minCut(dim, dist);

    showVertices(cutSetPool);

    for(size_t i = 0; i < dim; i++)
        delete[] dist[i];
    delete[] dist;


    return 0;
}