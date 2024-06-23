#ifndef __DATA_HPP__
#define __DATA_HPP__
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;


void readData(const char * instance_name, double *** costMatrix, int *dim);
void showMatrix(double** costMatrix, int n);

#endif