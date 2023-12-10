#include "mincut.hpp"

#define NEGATIVE_INFINITY -std::numeric_limits<double>::infinity()
#define POSITIVE_INFINITY std::numeric_limits<double>::infinity()
#define IDENTIFIER 0

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


void mergeVertices(VertexCollection & collection, std::vector<int> &identifiers, int i, int j, DistMap &distMap)
{
    int greaterIdentifier, smallerIdentifier;

    if(i > j)
    {
        greaterIdentifier = i;
        smallerIdentifier = j;
    }else{
        greaterIdentifier = j;
        smallerIdentifier = i;
    }

    /*Juntando vértices*/
    collection[smallerIdentifier].insert(collection[smallerIdentifier].begin(), collection[greaterIdentifier].begin(), collection[greaterIdentifier].end());
    collection.erase(greaterIdentifier);

    distMap[smallerIdentifier][greaterIdentifier] = distMap[greaterIdentifier][smallerIdentifier] = 0;
    /*Atualizando custos*/
    for(auto id : identifiers){
        distMap[smallerIdentifier][id] += distMap[greaterIdentifier][id];
        distMap[id][smallerIdentifier] += distMap[greaterIdentifier][id];
        distMap[smallerIdentifier].erase(greaterIdentifier);
        distMap[id].erase(greaterIdentifier);
    }

    identifiers.erase(std::remove(identifiers.begin(), identifiers.end(), greaterIdentifier), identifiers.end());
    distMap.erase(greaterIdentifier);
}



/*Função  para  obter a matriz de distâncias na forma de um map.
  A  utilização desta estrutura facilita na hora de atualizar os
  valores das distâncias, já que nos permite facilmente eliminar
  linhas e colunas da "matriz" sem alterar os índices das linhas
  e colunas remanescentes.
*/
DistMap getDistMap(double ** matrix, const int n)
{
    const double threshold = 0.00000001;
    DistMap dists;

    for(size_t i = 0; i < n; i++) dists[i] = std::map<int, double> ();

    std::cout << "Definindo matriz: \n";
    for(size_t i = 0; i < n; i++)
    {
        for(size_t j = 0; j < n; j++)
        {
            if(matrix[i][j] < threshold) matrix[i][j] = 0.0;
            dists[i][j] = dists[j][i] = matrix[i][j];
            std::cout << dists[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return dists;
}


int getTightlyVertexId(std::vector<int> collection, VertexCollection vertices, std::map<int, bool> included, std::vector<int> identifiers, DistMap distMap)
{
    double maxDist, maxId, dist;

    maxDist = NEGATIVE_INFINITY;
    for(int i : identifiers)//Percorre todos os identificadores (todos os vértices)
    {
        dist = 0;
        if(!included[i])//Se o vértice em questão não estiver incluído na coleçao considerada
        {
            for(int idx : collection)
            {
                dist += distMap[i][idx];
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

double minCutPhase(int &last, int &penultimate, CutSetPool &bestCut, VertexCollection vertices, std::vector<int> &identifiers, const int initVertexIdx, DistMap distMap)
{
    double cut_of_the_phase = 0, greaterCost=NEGATIVE_INFINITY, dist;
    int tightVertexId;
    const int N = vertices.size();

    std::vector<int> A = {initVertexIdx};
    std::map<int, bool> included;

    for(auto k : identifiers)included[k]=false;

    included[initVertexIdx] = true;

    while(A.size() < N)
    {
        tightVertexId = getTightlyVertexId(A, vertices, included, identifiers, distMap);
        A.push_back(tightVertexId);
        std::cout << "tightVertexId: " << tightVertexId << std::endl;
        included[tightVertexId] = true;
        std::cout << "Include vertices: ";
        for(auto k : included)std::cout << k.first << ": " << k.second << ", ";
        std::cout << std::endl;
    }

    last = A[A.size() - 1];
    penultimate = A[A.size() - 2];

    

    for(int i : identifiers)
    {
        if(i != last)
        {
            cut_of_the_phase += distMap[i][last];
        }
    }

    A.pop_back(); /*Removendo último elemento incluído*/
    
    /*Obtendo o corte do grafo*/
    bestCut = CutSetPool();
    bestCut.push_back({});
    for(auto element : A)
    {
        bestCut[0].insert(bestCut[0].begin(), vertices[element].begin(), vertices[element].end());
    }
    bestCut.push_back(vertices[last]);


    return cut_of_the_phase;

}


CutSetPool minCut(int n, double ** matrix)
{
    DistMap distMap;
    distMap = getDistMap(matrix, n);

    /*Identificadores dos vértices que serão unidos*/
    int last, penultimate;

    CutSetPool bestCut, cutFound;

    /*Identificadores ainda ativos

      Ao  fazer  a  junção  entre  dois  vértices,  o  maior
      identificador dos dois deve ser desativado e, portanto,
      removido da lista de identificadores.
    */
    std::vector<int> identifiers; 

    /*Coleção de vértices
    
      A    utilização    de    um   map  permite-nos atribuir um identificador 
      a   cada   vértice.   Inicialmente    temos somente vértices "solitários"
      e   os   identificadores  são  iguais  ao vértices únicos aos quais estão 
      associados.  Por exemplo,  se  tivermos  n=3, teremos a seguinte coleção:

      {0: {0}, 1: {1}, 2: {2}}

      Em  algum momento do programa, alguns vértices são unidos. Em um cenário
      como este, por exemplo, podemos unir os vértices 1 e 2. Nessas situações, 
      uniremos  os dois vértices mantendo o menor identificador, resultando em:

      {0: {0}, 1: {1, 2}}

      Logo  em  seguida,  devemos atualizar a distMap para que as distâncias do 
      vértice  de  identificador  2 sejam somadas ao vértice de identificador 1
      e,  logo  após  isso,  devemos  fazer com que as distâncias do vértice de 
      identificador  2  até  todos  os  outros  vértices seja zerada, pois este 
      identificador não existe mais.
    */
    VertexCollection vertexCollection;

    /*Inicializando a coleção de vértices - inicialmente "solitários"*/
    for(size_t i = 0; i < n; i++){
        vertexCollection.insert(std::make_pair(i, std::vector<int>({i})));
        identifiers.push_back(i);
    }

    double minimum = POSITIVE_INFINITY, cut_of_the_phase;


    for(auto k : distMap)
    {
        for(auto u: k.second)
        {
            std::cout << k.first << "---( " << u.second << " )--> " << u.first << ";";
        }
        std::cout<< std::endl;
    }


    showVertexCollection(vertexCollection);
    while(vertexCollection.size() > 2)
    {
        cut_of_the_phase = minCutPhase(last, penultimate, cutFound, vertexCollection, identifiers, identifiers[0], distMap);

        std::cout << "cut-of-the-phase: " << cut_of_the_phase << std::endl;
        std::cout << "CutSetPool: \n";
        showCutSetPool(bestCut);
        if(cut_of_the_phase < minimum)
        {
            bestCut = cutFound;
            minimum = cut_of_the_phase;
        }

        std::cout << "Fazendo merge dos vértices " << last << " e " << penultimate << std::endl;
        mergeVertices(vertexCollection, identifiers, last, penultimate, distMap);
        std::cout << "Identificadores: ";
        for(auto k : identifiers) std::cout << k << " ";
        std::cout << std::endl;
        for(auto k : distMap)
        {
            for(auto u: k.second)
            {
                std::cout << k.first << "---( " << u.second << " )--> " << u.first << ";";
            }
            std::cout<< std::endl;
        }
        showVertexCollection(vertexCollection);

    }

    std::cout << "Minimum: " << minimum << std::endl;
    std::cout << "CutSetPool: \n";
    showCutSetPool(bestCut);

    return bestCut;
}