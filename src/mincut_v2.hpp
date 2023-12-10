#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

#ifndef __MINCUTV2_H__
#define __MINCUTV2_H__

typedef std::vector< std::vector<int> > CutSetPool;
typedef std::vector< std::vector<int> > Matrix;
typedef std::map<int, std::map<int, double > > DistMap;
typedef std::map<int, std::vector<int> > VertexCollection;


void showCutSetPool(CutSetPool cutSetPool);
DistMap getDistMap(double ** matrix, const int n);
int getTightlyVertexId(std::vector<int> collection, VertexCollection vertices, std::vector<bool> included, std::vector<int> identifiers, DistMap distMap);
double minCutPhase(int &last, int &penultimate, CutSetPool &bestCut, VertexCollection vertices, std::vector<int> &identifiers, const int initVertexIdx, DistMap distMap);


CutSetPool minCut(int n, double ** matrix);

#endif