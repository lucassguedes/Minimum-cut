#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

#ifndef __MINCUT_H__
#define __MINCUT_H__

#define NEGATIVE_INFINITY -std::numeric_limits<double>::infinity()

typedef std::vector< std::vector<char> > Matrixc;
typedef std::vector< std::vector<int> > Matrixd;


void mergeVertices(Matrixd &A, int pos1, int pos2);
double calculateDist(std::vector<int> v1, std::vector<int> v2, double ** connections);
double calculateDistMat(Matrixd A, std::vector<int> v2, double ** connections);
double calculateMaxDistMat(Matrixd A, std::vector<int> v2, double ** connections);
void showVertices(Matrixd vertices);
double minCutPhase(Matrixd &vertices, int aIndex, double ** connections);
Matrixd minCut(int n, double ** connections);
void insertElement(Matrixd &vertices, int element);

#endif