#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <vector>
#include "mincut.hpp"
#include "Data.hpp"
#include <stdexcept>

using std::cout;
using std::vector;
using std::endl;

int main(int argc, char ** argv)
{
    double** dist; 
    int dim;

    try{
        readData(argv[1], &dist, &dim);
    }catch(const std::exception &e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    cout << "Dimension: " << dim << endl;
    cout << "Matrix: \n";
    showMatrix(dist, dim);

    /**Minimum cut - Maximum Cardinality Search Algorithm */
    CutSetPool cutSetPool = minCut(dim, dist);

    cout << "Cutsetpool: \n";
    showCutSetPool(cutSetPool);



    for(int i = 0; i < dim; i++)
        delete[] dist[i];
    delete[] dist;
    return EXIT_SUCCESS;
}
