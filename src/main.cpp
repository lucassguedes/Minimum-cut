#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <vector>
#include "mincut.hpp"
#include "Data.hpp"

using std::cout;
using std::vector;
using std::endl;

int main(int argc, char ** argv)
{
    double** dist; 
    int dim;

    readData(argv[1], &dist, &dim);
    
    
    cout << "Dimension: " << dim << endl;

    cout << "Matrix: \n";
    showMatrix(dist, dim);

    CutSetPool cutSetPool = minCut(dim, dist);

    cout << "Cutsetpool: \n";
    showCutSetPool(cutSetPool);

    for(int i = 0; i < dim; i++)
        delete[] dist[i];
    delete[] dist;


    return 0;
}
