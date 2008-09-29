/*!
	@file 	CT.cpp
	@brief 	Several tests for computing homology groups.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

#include "ct.h"
#include "simplex.h"
#include "chain.h"
#include "matrix.h"

int main(int argc, char* argv[])
{
	// At the moment, this is only a simple demonstration: A file with triangulations
	// for several manifolds is read and processed afterwards. For each triangulation,
	// the Smith normal form of the first boundary matrix is calculated and the result
	// is printed out.

	//
	//vector< vector<simplex> > data = process_file("3_manifolds_all.txt.ct");
	//vector< vector<simplex> > data = process_file("graph.ct");
	//
	
	vector< vector<simplex> > data = process_file("2_manifolds_all.txt.ct");
	for(size_t i = 0; i < data.size(); i++)
	{
		size_t w_prev = 0;
		size_t b_prev = 0;
		size_t z_cur  = 0;

		vector<chain> boundaries;
		for(size_t j = 0; j < data[i].size(); j++)
			boundaries.push_back(data[i][j].boundary());

		cout << "(";
		for(size_t dim = 0; dim < 3; dim++)
		{
			vector<simplex> generators 	= find_generators(boundaries);
			matrix matrix_boundaries 	= create_matrix(generators, boundaries);
			matrix matrix_snf		= matrix_boundaries.nf_smith();


			// DEBUG

			/*
			
			matrix_boundaries.print();
			matrix_snf.print();	
			
			*/

			// Output/store the values for the next dimension
	
			z_cur = matrix_snf.get_num_zero_cols();
			cout << z_cur - w_prev;
			if(b_prev != 0)
				cout << "+" << b_prev;
			cout << ",";
			w_prev = matrix_snf.get_num_non_zero_rows();
			b_prev = matrix_snf.get_torsion();

			// Prepare the generators for the next run

			boundaries.clear();
			for(size_t num_gens = 0; num_gens < generators.size(); num_gens++)
				boundaries.push_back(generators[num_gens].boundary());
		}
		
		cout << "1)\n";
	}

	return(0);
}

/// Given a set of boundaries, this algorithm determines the generators
/// of the chain group.
///
/// @param chains 	The set of chains for which the generators are
///			determined.
///
/// @return A set of simplices that generate the chain group.

vector<simplex> find_generators(vector<chain> chains)
{
	vector<simplex> generators;
	for(size_t i = 0; i < chains.size(); i++)
	{
		for(size_t j = 0; j < chains[i].elements.size(); j++)
		{
			// Look for the element

			bool found = false;
			for(size_t k = 0; k < generators.size(); k++)
			{
				if(generators[k] == chains[i].elements[j].s)
				{
					found = true;
					break;
				}
			}

			if(!found)
				generators.push_back(chains[i].elements[j].s);
		}
	}

	return(generators);
}

/// Returns the position of the appropriate generator in an
/// array of generators.
///
/// @param generators	Array of all generators
/// @param generator	Generator that is currently searched
///
/// @return The position in the array or -1.

size_t generator_position(vector<simplex> generators, simplex generator)
{
	for(size_t i = 0; i < generators.size(); i++)
		if(generators[i] == generator)
			return(i);

	return(-1);		
}

/// Given an array of generators and an array of boundaries, the
/// boundary matrix is created.
///
/// @param generators	Array of all generators
/// @param boundaries	Array of all boundaries
///
/// @return The boundary matrix.

matrix create_matrix(vector<simplex> generators, vector<chain> boundaries)
{

	/****

	OLD CODE

	// Contains all rows that are generated during the process. This vector
	// is then going to be assigned to the actual boundary matrix.
	vector<long> rows;

	for(size_t i = 0; i < boundaries.size(); i++)
	{
		long* row = new long[generators.size()];
		memset(row, 0, generators.size()*sizeof(long));

		for(size_t j = 0; j < boundaries[i].elements.size(); j++)
		{
			size_t pos = generator_position(generators, boundaries[i].elements[j].s);
			row[pos] = boundaries[i].elements[j].c;
		}

		// Store the row

		for(size_t j = 0; j < generators.size(); j++)
			rows.push_back(row[j]);

		delete[] row;
	}
	
	****/

	// Contains all columns that are generated during the process. This vector
	// is then going to be assigned to the actual boundary matrix.
	vector<long> cols;
	for(size_t i = 0; i < boundaries.size(); i++)
	{
		long* col = new long[generators.size()];
		memset(col, 0, generators.size()*sizeof(long));

		for(size_t j = 0; j < boundaries[i].elements.size(); j++)
		{
			size_t pos = generator_position(generators, boundaries[i].elements[j].s);

			// This ensures that only non-empty simplices get assigned non-zero
			// values within the boundary matrix.
			if(boundaries[i].elements[j].s.vertices.size() > 0)
				col[pos] = boundaries[i].elements[j].c;
			else
				col[pos] = 0;	
		}

		// Store the column

		for(size_t j = 0; j < generators.size(); j++)
			cols.push_back(col[j]);

		delete[] col;
	}

	matrix boundary_matrix;

	/************

	boundary_matrix.assign(rows, boundaries.size(), generators.size());

	*************/
	
	boundary_matrix.assign(cols, boundaries.size(), generators.size());
	boundary_matrix.transpose();
	return(boundary_matrix);
}

/// Reads data about vertices from a file and allows the program to process
/// the file on a per line basis.

vector< vector<simplex> > process_file(const char* filename)
{
	size_t lines = 0;

	vector< vector<simplex> > result;
	vector<simplex> cur_complex;

	ifstream input(filename);
	string line;

	cout << "Processing \"" << filename << "\"...\n";

	while(input >> line)
	{
		lines++;

		// Whole set of simplices has been read
		if(line == "EOL")
		{
			result.push_back(cur_complex);
			cur_complex.clear();

			//continue;
			break;
		}

		// Convert the input data into to use the simplex
		// class. 

		stringstream converter;
		simplex cur_simplex;

		converter.str(line);
		unsigned long v;
		char delimiter;
		while(converter.good())
		{
			converter >> v;
			converter >> delimiter; // "," is ignored

			cur_simplex.vertices.push_back(v);
		}

		cur_complex.push_back(cur_simplex);
	}
	
	input.close();
	cout << "Successfully processed " << lines << " lines.\n";

	return(result);
}
