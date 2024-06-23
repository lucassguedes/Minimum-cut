#include "Data.hpp"


void readData(const char * instance_name, double *** costMatrix, int *dim){
    std::string instanceName = instance_name;
    std::ifstream file(instanceName);


    if(!file)
    {
        throw std::runtime_error("Arquivo de entrada não encontrado!");
    }

    double ** matrix;

    file >> *dim;

    matrix = new double*[*dim];
    for(int i = 0; i < *dim; i++)
    {
        matrix[i] = new double[*dim];
        for(int j = 0; j < *dim; j++)
        {   
            file >> matrix[i][j];
        }
    }

    *costMatrix = matrix;

    file.close();
}

void showMatrix(double** costMatrix, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            cout << costMatrix[i][j] << " ";
        }
        cout << endl;
    }
}