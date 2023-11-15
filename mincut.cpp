#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

typedef std::vector< std::vector<char> > Matrixc;

/*Precisa ser testada*/
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

/*Precisa ser testada*/
double calculateDistMat(Matrixc A, std::vector<char> v2, std::map<char, std::map<char, double> > &connections)
{
    double dist = 0;
    for(size_t i = 0; i < A.size(); i++)
    {
        dist += calculateDist(A[i], v2, connections);
    }

    return dist;
}


/*Precisa ser testada*/
double getTighlyConnectedVertex(int aIndex, Matrixc A, Matrixc vertices, std::map<char, std::map<char, double> > &connections)
{
    double greaterDist = -std::numeric_limits<double>::infinity();
    double greaterIdx = -1;
    double calculatedDist;

    std::vector< std::pair< int , double> > idx_dist; /*Índices dos "vertices" e seus custos associados*/
    for(size_t i = 0; i < vertices.size() && i != aIndex; i++)
    {
        calculatedDist = calculateDistMat(A, vertices[i], connections);

        idx_dist.push_back(std::make_pair(i, calculatedDist));
    }

    /*Ordenando os vértices em "vertices" pelas distâncias*/
    std::sort(idx_dist.begin(), idx_dist.end(), [](const std::pair<int, double> &a, 
                                                   const std::pair<int, double> &b) {return a.second > b.second;});

    /*Inserindo vértices dependendo do quão fortemente ligados a A eles estão*/
    for(size_t i = 0; i < idx_dist.size(); i++)
    {
        A.push_back(vertices[idx_dist[i].first]);
    }

    /*Juntar aqui os últimos dois vértices que foram inseridos em A*/
    double cut_of_the_phase = idx_dist[0].second;


    return cut_of_the_phase;
}


double minCutPhase(Matrixc &vertices, int aIndex)
{
    Matrixc A = {vertices[aIndex]};

    while(A.size() != vertices.size())
    {

    }
    

    return 0.0;
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

    showVertices(vertices);


    return 0;
}