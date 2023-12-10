#include "mincut_v2.hpp"

#define NEGATIVE_INFINITY -std::numeric_limits<double>::infinity()
#define POSITIVE_INFINITY std::numeric_limits<double>::infinity()
#define IDENTIFIER 0

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

    for(size_t i = 0; i < n; i++)
    {
        dists[i] = std::map<int, double> ();
        for(size_t j = 0; j < n; j++)
        {
            if(matrix[i][j] < threshold)
            {
                matrix[i][j] = 0.0;
                dists[i][j] = matrix[i][j];
            }
        }
    }

    return dists;
}


int getTightlyVertex(Matrix vertices, std::vector<bool> included, const int n, DistMap distMap)
{

}

double minCutPhase(CutSetPool &bestCut, Matrix &vertices, const int initialVertex, DistMap &distMap)
{
    double cut_of_the_phase = POSITIVE_INFINITY, greaterCost=NEGATIVE_INFINITY, dist;
    int greaterIdx;
    const int N = vertices.size();

    std::vector<int> A = {vertices[initialVertex][IDENTIFIER]};
    std::vector<bool> included = std::vector<bool> (N, false);
    included[vertices[initialVertex][IDENTIFIER]] = true;

    while(A.size() < N)
    {
        
    }

}


CutSetPool minCut(int n, double ** matrix)
{
    DistMap distMap;
    distMap = getDistMap(matrix, n);

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
    std::map<int, std::vector<int> > vertexCollection;

    /*Inicializando a coleção de vértices - inicialmente "solitários"*/
    for(size_t i = 0; i < n; i++){
        vertexCollection.insert(std::make_pair(i, std::vector<int>({i})));
        identifiers.push_back(i);
    }

    double minimum = POSITIVE_INFINITY, cut_of_the_phase;

    while(vertexCollection.size() > 1)
    {
        //Chamar o minCutPhase aqui

        if(cut_of_the_phase < minimum)
        {
            bestCut = cutFound;
            minimum = cut_of_the_phase;
        }
    }

    return bestCut;
}