#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

#define NEGATIVE_INFINITY -std::numeric_limits<double>::infinity()

typedef std::vector< std::vector<char> > Matrixc;
typedef std::vector< std::vector<int> > Matrixd;

void mergeVertices(Matrixd &A, int pos1, int pos2) {

  if(pos2 < pos1)
  {
    std::cout << "\033[1;35mpos1 = " << pos1 << ", pos2 = " << pos2 << std::endl;
    std::cout << "trocando...\n";
    std::swap(pos1, pos2);
    std::cout << "pos1 = " << pos1 << ", pos2 = " << pos2 << "\033[0m\n";
  }


  std::vector<int> v = A[pos2];

  A.erase(A.begin() + pos2);

  A[pos1].insert(A[pos1].end(), v.begin(), v.end());
}

double calculateDist(std::vector<int> v1, std::vector<int> v2, double ** connections)
{
    double dist = 0;
    for(size_t i = 0; i < v1.size(); i++)
    {
        for(size_t j = 0; j < v2.size(); j++)
        {
            dist += connections[v1[i]][v2[j]];
        }
    }

    return dist;

}

double calculateDistMat(Matrixd A, std::vector<int> v2, double ** connections)
{
    double dist = 0;
    for(size_t i = 0; i < A.size(); i++)
    {
        dist += calculateDist(A[i], v2, connections);
    }

    return dist;
}

double calculateMaxDistMat(Matrixd A, std::vector<int> v2, double ** connections)
{
    double dist = -std::numeric_limits<double>::infinity();
    double calcdist;
    for(size_t i = 0; i < A.size(); i++)
    {
        calcdist = calculateDist(A[i], v2, connections);
        if(calcdist > dist)
            dist = calcdist;
    }

    return dist;
}

void showVertices(Matrixd vertices)
{
    std::cout << "Vertices: ";
    for(auto k : vertices)
    {
        std::cout << "(";
        for(auto u : k)
        {
            std::cout << u << ", ";
        }
        std::cout << ") -- ";
    }
    std::cout << std::endl;
}


double minCutPhase(Matrixd &vertices, int aIndex, double ** connections)
{
    double cut_of_the_phase = std::numeric_limits<double>::infinity();
    int greaterIdx;
    double greaterCost = NEGATIVE_INFINITY;
    const int N = vertices.size();
    double dist;
    std::vector<int> lastVerticesIdx; /*Armazena os índices dos últimos dois vértices incluídos em A*/

    Matrixd A = {vertices[aIndex]};

    std::cout << "\n############################\nN = " << N << std::endl;

    
    std::vector<int> remainingIdx; //Lista de vértices que ainda não foram incluídos em A
    for(size_t i = 0; i < N; i++) remainingIdx.push_back(i);


    if(remainingIdx.size() == 2)
        lastVerticesIdx = remainingIdx;

    /*Removendo aIndex da lista de índices de vértices disponíveis*/
    remainingIdx.erase(std::remove(remainingIdx.begin(), remainingIdx.end(), aIndex), remainingIdx.end());

    std::cout<<"Inicio - Indices dos vertices restantes: ";
    for(auto v : remainingIdx) std::cout << v << " ";
    std::cout << std::endl;


    // std::cout << "Iniciando minCutPhase...\n";
    while(A.size() < N)
    {
        if(remainingIdx.size() == 2)
            lastVerticesIdx = remainingIdx;
        
        if(remainingIdx.size() == 1)
        {
            // std::cout << "Calculando distancia até este vertice: (";
            // for(auto v :  vertices[remainingIdx[0]]) std::cout << v << ", ";
            // std::cout << ")\n";
            cut_of_the_phase = calculateDistMat(A, vertices[remainingIdx[0]], connections);
        }
        /*Procurando pelo vértice mais fortemente ligado à A*/
        std::cout << "\033[1;36mA - ";
        showVertices(A);
        std::cout <<  "\033[0m\n";

        std::cout<<"Indices dos vertices restantes: ";
        for(auto v : remainingIdx) std::cout << v << " ";
        std::cout << std::endl;

        greaterCost = NEGATIVE_INFINITY;
        std::cout << "\nProcurando pelo vertice mais fortemente ligado...\n";
        for(size_t i : remainingIdx)
        {
            dist = calculateDistMat(A, vertices[i], connections);
            // std::cout << "dist: " << dist << std::endl;

            std::cout << "\tVertice - (";
            for(auto element : vertices[i])std::cout << element << ", ";
            std::cout << ") , custo: " << dist << std::endl;
            if(dist > greaterCost)
            {
                greaterCost = dist;
                greaterIdx = i;
            }
        }
        std::cout << "\033[1;33mSelecionando vertice da posicao " << greaterIdx << ", cujo custo eh " << greaterCost << "\033[0m\n";
        /*Inserindo vértice mais fortemente ligado à A em A*/
        A.push_back(vertices[greaterIdx]);
        /*Removendo o índice desse vértice da lista de vértices que ainda estão disponíveis*/
        remainingIdx.erase(std::remove(remainingIdx.begin(), remainingIdx.end(), greaterIdx), remainingIdx.end());
    }

    
    std::cout << "\033[1;36mA - ";
    showVertices(A);
    std::cout <<  "\033[0m\n";

    std::cout << "\033[1;34mCut-of-the-phase: " << cut_of_the_phase << "\033[0m\n";

    /*Fazendo o merge dos últimos dois vértices inclusos em A*/
    std::cout << "Fazendo merge dos vertices nas posicoes " << lastVerticesIdx[0] << " e " << lastVerticesIdx[1] << std::endl;
    mergeVertices(vertices, lastVerticesIdx[0], lastVerticesIdx[1]);

    return cut_of_the_phase;
}

double minCut(Matrixd &vertices, double ** connections)
{
    double minimum = std::numeric_limits<double>::infinity();
    double cut_of_the_phase;
    Matrixd bestSet;
    while(vertices.size() > 1)
    {
        std::cout << "\033[1;31mMincut - ";
        showVertices(vertices);
        std::cout << "\033[0m\n";
        cut_of_the_phase = minCutPhase(vertices, 0, connections);

        std::cout << "\033[1;32mMincut (vertices unidos) - ";
        showVertices(vertices);
        std::cout << "\033[0m\n";

        if(cut_of_the_phase < minimum)
        {
            bestSet = vertices;
            minimum = cut_of_the_phase;
        }
    }

    std::cout << "Melhor particao - ";
    showVertices(bestSet);

    return minimum;
}

/*Esta função é usada apenas para inserir vértices que não são múltiplos*/
void insertElement(Matrixd &vertices, int element)
{
    bool found = false;
    for(size_t i = 0; i < vertices.size(); i++)
    {
        if(std::find(vertices[i].begin(), vertices[i].end(), element) != vertices[i].end())
        {
            found = true;
            break;
        }
            
    }
    if(!found)
        vertices.push_back(std::vector<int>({element}));
}

int main(void)
{
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
        std::cout << "k = " << k << ", u = " << u << std::endl;
        
        connections[island1][island2] = connections[island2][island1] = importance;
        distances[k][u] = distances[u][k] = importance;

        insertElement(vertices, k);
        insertElement(vertices, u);
    }

    std::cout << "Elementos unicos: ";
    for(auto k : unique_islands)std::cout << k << " ";
    std::cout << std::endl;

    int dim = unique_islands.size();
    double ** dist = new double*[dim];

    std::cout << "Matriz de distancias: \n";
    for(size_t i = 0; i < dim; i++)
    {
        dist[i] = new double[dim];
        for(size_t j = 0; j < dim; j++)
        {   
            if(distances[i].find(j) != distances[i].end())
                dist[i][j] = distances[i][j];
            else
                dist[i][j] = 0;
            std::cout << dist[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // std::cout.setstate(std::ios_base::failbit);
    double minimum = minCut(vertices, dist);
    // std::cout.clear();
    std::cout <<  minimum << std::endl;




    /*Tests*/

    // std::cout << calculateDist(std::vector<char>({'G', 'U'}), std::vector<char>({'Z'}), connections) << std::endl;

    // Matrixc A = Matrixc();

    // A.push_back(std::vector<char>({'G', 'U'}));
    // A.push_back(std::vector<char>({'Z'}));

    // std::cout << calculateDistMat(A, std::vector<char>({'J'}), connections) << std::endl;


    return 0;
}