#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

#define NEGATIVE_INFINITY -std::numeric_limits<double>::infinity()

typedef std::vector< std::vector<char> > Matrixc;

void mergeVertices(Matrixc &A, int pos1, int pos2) {

  if(pos2 < pos1)
  {
    std::cout << "\033[1;35mpos1 = " << pos1 << ", pos2 = " << pos2 << std::endl;
    std::cout << "trocando...\n";
    std::swap(pos1, pos2);
    std::cout << "pos1 = " << pos1 << ", pos2 = " << pos2 << "\033[0m\n";
  }


  std::vector<char> v = A[pos2];

  A.erase(A.begin() + pos2);

  A[pos1].insert(A[pos1].end(), v.begin(), v.end());
}

double calculateDist(std::vector<char> v1, std::vector<char> v2, std::map<char, std::map<char, double> > &connections)
{
    double dist = 0;
    for(size_t i = 0; i < v1.size(); i++)
    {
        for(size_t j = 0; j < v2.size(); j++)
        {
            if(connections[v1[i]].find(v2[j]) != connections[v1[i]].end())
            {
                dist += connections[v1[i]][v2[j]];
            }
        }
    }

    return dist;

}

double calculateDistMat(Matrixc A, std::vector<char> v2, std::map<char, std::map<char, double> > &connections)
{
    double dist = 0;
    for(size_t i = 0; i < A.size(); i++)
    {
        dist += calculateDist(A[i], v2, connections);
    }

    return dist;
}

double calculateMaxDistMat(Matrixc A, std::vector<char> v2, std::map<char, std::map<char, double> > &connections)
{
    double dist = -std::numeric_limits<double>::infinity();
    double calcdist;
    for(size_t i = 0; i < A.size(); i++)
    {
        calcdist = calculateDist(A[i], v2, connections);
        if(calcdist > dist && calcdist > 0)
            dist = calcdist;
    }

    return dist;
}

void showVertices(Matrixc vertices)
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


double minCutPhase(Matrixc &vertices, int aIndex, std::map<char, std::map<char, double> > &connections)
{
    double cut_of_the_phase = std::numeric_limits<double>::infinity();
    int greaterIdx;
    double greaterCost = NEGATIVE_INFINITY;
    const int N = vertices.size();
    double dist;
    std::vector<int> lastVerticesIdx; /*Armazena os índices dos últimos dois vértices incluídos em A*/

    Matrixc A = {vertices[aIndex]};

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

double minCut(Matrixc &vertices, std::map<char, std::map<char, double> > &connections)
{
    double minimum = std::numeric_limits<double>::infinity();
    double cut_of_the_phase;
    Matrixc bestSet;
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
void insertElement(Matrixc &vertices, char element)
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
        vertices.push_back(std::vector<char>({element}));
}

int main(void)
{
    int nConnections;
    char cap1, cap2;
    char island1, island2;
    double importance;
    Matrixc vertices;

    std::map<char, std::map<char, double> > connections;

    std::cin >> nConnections >> cap1 >> cap2;

    insertElement(vertices, cap1);
    insertElement(vertices, cap2);

    for(size_t i = 0; i < nConnections; i++)
    {
        std::cin >> island1 >> island2 >> importance;
        
        connections[island1][island2] = connections[island2][island1] = importance;

        insertElement(vertices, island1);
        insertElement(vertices, island2);
    }

    // std::cout.setstate(std::ios_base::failbit);
    double minimum = minCut(vertices, connections);
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