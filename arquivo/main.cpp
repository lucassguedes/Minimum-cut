#include <iostream>
#include <vector>

typedef std::vector<std::vector<int> > Matrixd;

int main(void)
{
    /*
     Cada  linha  da  matriz  corresponde  a um vértice.
     Cada  linha  consiste  em  um vector, assim podemos
     representar  vértices  isolados  como um vector com
     um  único elemento e representar vértices que foram
     unidos como vectors de mais de um elemento.
    */
    Matrixd vertices; 
    double ** dist {nullptr}; /*Matriz de distâncias entre cada um dos vértices individuais (antes de qualquer junção)*/
 
    const int dimension = 10;


    /*Criando a matriz de distâncias*/
    dist = new double*[dimension];

    for(size_t i = 0; i < dimension; i++)
    {
        dist[i] = new double[dimension];
    }

    /*Preenchendo a matriz de distâncias*/
    int counter = 0;
    for(size_t i = 0; i < dimension; i++)
    {
        for(size_t j = 0; j < dimension; j++)
        {
            dist[i][j] = counter++;
        }
    }

    /*Exibindo matriz de distâncias*/
    for(size_t i = 0; i < dimension; i++)
    {
        for(size_t j = 0; j < dimension; j++)
        {
            std::cout << dist[i][j] << " ";
        }
        std::cout << std::endl;
    }


    /*Desalocando matriz de distâncias*/
    for(size_t i = 0; i < dimension; i++)
        for(size_t j = 0; j < dimension; j++)
            delete dist[i];
    delete dist;

    return 0;
}