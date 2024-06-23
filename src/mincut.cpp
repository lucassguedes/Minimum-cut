#include "mincut.hpp"

#define NEGATIVE_INFINITY -std::numeric_limits<double>::infinity()
#define POSITIVE_INFINITY std::numeric_limits<double>::infinity()
#define IDENTIFIER 0

#define EPSILON 0.00000001

unsigned int getNVertices(vector<vector<int> > vertices)
{
    unsigned int count=0;
    const int n = vertices.size();
    for(int i = 0; i < n; i++)
    {
        if(vertices[i].size())
            count++;
    }
    return count;
}


void showCutSetPool(CutSetPool cutSetPool)
{
    const size_t parts = cutSetPool.size();
    
    for(size_t i = 0; i < parts; i++)
    {
        for(size_t j = 0 ; j < cutSetPool[i].size(); j++)
        {
            cout << cutSetPool[i][j] << " ";
        }
        cout << endl;
    }
}


void mergeVertices(vector<vector<int> > &collection, vector<int> &identifiers, int i, int j, double **matrix)
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
        

        /*Atualizando matriz de distâncias*/
        matrix[idx][toBeRemoved] = matrix[toBeRemoved][idx] = 0;
        matrix[newIdx][idx] += cost;
        matrix[idx][newIdx] += cost;

    }

}


int getTightlyVertexId(vector<int> collection, vector<vector<int> > vertices, std::map<int, bool> included, vector<int> identifiers, double** distMap)
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


/*
    nome: minCutPhase
    Parâmetros:
        * n -> Quantidade de vértices
        * last -> Último vértice adicionado ao conjunto A (ver função minCutPhase)
        * penultimate -> Penúltimo vértice a adicionado ao conjunto A
        * cutFound -> Corte encontrado
        * vertices -> Conjunto de vértices (que é modificado quando vértices são unidos)
        * identifiers -> Identificadores dos vértices. 
            ** Sempre que um vértice Y é unido a um vértice X, o vértice resultante
                será identificado por min(X, Y).
        * matrix -> Matriz de distâncias entre os vértices            
*/
double minCutPhase(int n, int &last, int &penultimate, vector<vector<int> > &bestCut, vector<vector<int> > vertices, vector<int> &identifiers, const int initVertexIdx, double ** matrix)
{
    double cut_of_the_phase = 0;
    int tightVertexId;
    const unsigned int N = getNVertices(vertices);

    vector<int> A = {initVertexIdx};
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
    vector<int> firstCut;
    for(auto element : A)
    {
        firstCut.insert(firstCut.begin(), vertices[element].begin(), vertices[element].end());
    }
    
	bestCut.push_back(vertices[last]);
	bestCut.push_back(firstCut);


    return cut_of_the_phase;

}

CutSetPool minCut(int n, double ** matrix)
{
    /*Identificadores dos vértices que serão unidos*/
    int last, penultimate;

    CutSetPool cutSetPool;
    vector<vector<int> > cutFound;
    /*Identificadores ainda ativos

      Ao  fazer  a  junção  entre  dois  vértices,  o  maior
      identificador dos dois deve ser desativado e, portanto,
      removido da lista de identificadores.
    */
    vector<int> identifiers; 

    vector<vector<int> > vertices;

    /*Inicializando a coleção de vértices - inicialmente "solitários"*/
    for(int i = 0; i < n; i++){
        vertices.push_back(vector<int>({i}));
        identifiers.push_back(i);
    }

    double minimum = POSITIVE_INFINITY, cut_of_the_phase;

    /*A cada iteração, vértices são "fundidos", tornando a quantidade de vértices menor.
      O loop deve se repetir enquanto houverem mais de dois vértices no conjunto.
    */
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

	cout << "Minimum = " << minimum << "\n";
    return cutSetPool;
}
