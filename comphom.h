/*!
	@file 	comphom.h
	@brief	Contains prototypes for the main program.
*/


#ifndef COMPHOM_H
#define COMPHOM_H

#include <vector>

using namespace std;

#include "matrix.h"
#include "simplex.h"

vector< vector<simplex> > process_file(const char* filename);
vector<simplex> find_generators(vector<chain> chains);
size_t generator_position(vector<simplex> generators, simplex generator);
matrix create_matrix(vector<simplex> generators, vector<chain> boundaries);

#endif
