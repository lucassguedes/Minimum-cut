#include "mincut.hpp"

#define NEGATIVE_INFINITY -std::numeric_limits<double>::infinity()
#define POSITIVE_INFINITY std::numeric_limits<double>::infinity()
#define IDENTIFIER 0

#define EPSILON 0.00000001


int getNVertices(std::vector<std::vector<int> > vertices)
{
    int count=0;
    const int n = vertices.size();
    for(int i = 0; i < n; i++)
    {
        if(vertices[i].size())
            count++;
    }
    return count;
}

void showVertexCollection(VertexCollection collection)
{
    for(auto v : collection)
    {
        std::cout << v.first << ": (";
        for(auto element : v.second)
        {
            std::cout << element << " ";
        }
        std::cout << ") ";
    }
    std::cout << std::endl;
}

void showCutSetPool(CutSetPool cutSetPool)
{
    const size_t parts = cutSetPool.size();
    
    for(size_t i = 0; i < parts; i++)
    {
        for(size_t j = 0 ; j < cutSetPool[i].size(); j++)
        {
            std::cout << cutSetPool[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


void mergeVertices(std::vector<std::vector<int> > &collection, std::vector<int> &identifiers, int i, int j, double **matrix)
{
    int newIdx, toBeRemoved;

    /*Fazendo merge dos vértices*/
    if(collection[i][0] < collection[j][0])
    {
        collection[i].insert(collection[i].end(), collection[j].begin(), collection[j].end());
        newIdx = i;
        toBeRemoved = j;
    }else{
        collection[j].insert(collection[j].end(), collection[i].begin(), collection[i].end());
        newIdx = j;
        toBeRemoved = i;
    }

    collection[toBeRemoved].clear();


    identifiers.erase(std::remove(identifiers.begin(), identifiers.end(), toBeRemoved), identifiers.end());

    matrix[newIdx][toBeRemoved] = matrix[toBeRemoved][newIdx] = 0;
    double cost;
    /*Atualizando distâncias*/
    for(auto idx : identifiers)
    {
        cost = (toBeRemoved > idx) ? matrix[idx][toBeRemoved] : matrix[toBeRemoved][idx];
        
        matrix[idx][toBeRemoved] = matrix[toBeRemoved][idx] = 0;

        matrix[newIdx][idx] += cost;
        matrix[idx][newIdx] += cost;

    }

}


int getTightlyVertexId(std::vector<int> collection, std::vector<std::vector<int> > vertices, std::map<int, bool> included, std::vector<int> identifiers, double** distMap)
{
    double maxDist, maxId, dist;

    maxDist = NEGATIVE_INFINITY;
    maxId = -1;
    for(int i : identifiers)//Percorre todos os identificadores (todos os vértices)
    {
        dist = 0;
        if(!included[i])//Se o vértice em questão não estiver incluído na coleçao considerada
        {
            for(int idx : collection)
            {
                const double cost = (idx > i) ? distMap[i][idx] : distMap[idx][i];
                dist += (std::isnan(cost)) ? 0 : cost;
            }
            if(dist > maxDist)
            {
                maxDist = dist;
                maxId = i;
            }
        }
        
    }

    return maxId;
}

double minCutPhase(int n, int &last, int &penultimate, std::vector<std::vector<int> > &bestCut, std::vector<std::vector<int> > vertices, std::vector<int> &identifiers, const int initVertexIdx, double ** matrix)
{
    double cut_of_the_phase = 0;
    int tightVertexId;
    const int N = getNVertices(vertices);

    std::vector<int> A = {initVertexIdx};
    std::map<int, bool> included;

    for(auto k : identifiers)included[k]=false;

    included[initVertexIdx] = true;

    while(A.size() < N)
    {
        tightVertexId = getTightlyVertexId(A, vertices, included, identifiers, matrix);
        A.push_back(tightVertexId);
        included[tightVertexId] = true;
    }

    last = A[A.size() - 1];
    penultimate = A[A.size() - 2];

    

    for(int i : identifiers)
    {
        if(i != last)
        {
            cut_of_the_phase += (last > i) ? matrix[i][last] : matrix[last][i];
        }
    }

    A.pop_back(); /*Removendo último elemento incluído*/
    
    /*Obtendo o corte do grafo*/
    std::vector<int> firstCut;
    for(auto element : A)
    {
        firstCut.insert(firstCut.begin(), vertices[element].begin(), vertices[element].end());
    }
    
	bestCut.push_back(vertices[last]);
	bestCut.push_back(firstCut);


    return cut_of_the_phase;

}

void clearInvalidValues(double ** matrix, const int n)
{
    const int threshold = 1e-8;

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(matrix[i][j] < threshold || std::isnan(matrix[i][j])) matrix[i][j] = 0;
        }
    }

}


CutSetPool minCut(int n, double ** matrix)
{
    /*Identificadores dos vértices que serão unidos*/
    int last, penultimate;

    CutSetPool cutSetPool;
    std::vector<std::vector<int> > cutFound;

    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(matrix[i][j] < EPSILON){
                matrix[i][j] = 0;
            }
        }
    }

    /*Identificadores ainda ativos

      Ao  fazer  a  junção  entre  dois  vértices,  o  maior
      identificador dos dois deve ser desativado e, portanto,
      removido da lista de identificadores.
    */
    std::vector<int> identifiers; 

    std::vector<std::vector<int> > vertices;

    /*Inicializando a coleção de vértices - inicialmente "solitários"*/
    for(size_t i = 0; i < n; i++){
        vertices.push_back(std::vector<int>({i}));
        identifiers.push_back(i);
    }

    double minimum = POSITIVE_INFINITY, cut_of_the_phase;

    while(getNVertices(vertices) > 2)
    {
        cut_of_the_phase = minCutPhase(n, last, penultimate, cutFound, vertices, identifiers, identifiers[0], matrix);

        if(cut_of_the_phase < minimum)
        {
            cutSetPool = cutFound;
            minimum = cut_of_the_phase;
            cutFound.clear();
        }

        mergeVertices(vertices, identifiers, last, penultimate, matrix);

    }

	std::cout << "Minimu = " << minimum << "\n";
    return cutSetPool;
}
