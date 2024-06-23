#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <cmath>

#ifndef __MINCUT_H__
#define __MINCUT_H__

using std::cout;
using std::vector;
using std::endl;

typedef std::vector< std::vector<int> > CutSetPool;
typedef std::vector< std::vector<int> > Matrix;
typedef std::map<int, std::map<int, double > > DistMap;
typedef std::map<int, std::vector<int> > VertexCollection;


unsigned int getNVertices(vector<vector<int> > vertices);
void showCutSetPool(CutSetPool cutSetPool);
int getTightlyVertexId(std::vector<int> collection, std::vector<std::vector<int> > vertices, std::map<int, bool> included, std::vector<int> identifiers, double** distMap);
double minCutPhase(int n, int &last, int &penultimate, std::vector<std::vector<int> > &bestCut, std::vector<std::vector<int> > vertices, std::vector<int> &identifiers, const int initVertexIdx, double ** matrix);
void mergeVertices(vector<vector<int> > &collection, vector<int> &identifiers, int i, int j, double **matrix);

CutSetPool minCut(int n, double ** matrix);

#endif
