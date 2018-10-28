/*!
	@file 	comphom.cpp
	@brief 	A small demo program for computing homology groups.
	@author Bastian Rieck
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include <string.h>
#include <getopt.h>

#include "comphom.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Read command-line options

	string file_in = "comphom.in";
	string file_out = "comphom.out";

	static struct option cmd_line_opts[] = 
	{
		{"input",	required_argument,	NULL, 	'i'},
		{"output",	required_argument,	NULL,	'o'},
		{"help",	no_argument,		NULL,	'h'},
		{NULL,		0,			NULL,	 0 }
	};

	int opt = 0;
	while((opt = getopt_long(argc, argv, "i:o:h", cmd_line_opts, NULL)) != -1)
	{
		switch(opt)
		{
			case 'h':
				cout << "Usage: comphom [options]\n"
					"Options:\n"
					"-h --help\t\tDisplay this information\n"
					"-i --input <file>\tRead <file> (default: comphom.in)\n"
					"-o --output <file>\tWrite to <file> (default: comphom.out)\n"
					"\n\n\n"
					"Please note that this is only a DEMO program. Send all comments to\n"
					"bastian[AT]annwfn[DOT]net.\n"
					"\n\n";

				return(0);	

			case 'i':
				file_in = optarg;
				break;
			
			case 'o':
				file_out = optarg;
				break;
					
			default:
				break;	
		}
	}

	cout << "comphom is ready. Current options:\n"
		"\tInput:  \"" << file_in << "\"\n"
		"\tOutput: \"" << file_out << "\"\n\n";
	
	vector< vector<simplex> > data = process_file(file_in.c_str());

	ofstream out(file_out.c_str());
	if(!out.good())
	{
		cerr << "Error: Could not write to \"" << file_out << "\". Aborting...\n";
		return(-1);
	}

	cout << "\n"
		"The following computations might take a while -- especially if\n"
		"large triangulations are present. Small dots will serve as a\n"
		"progress indicator.\n\n"
		"Computing...\n";

	short dots = 0;
	for(size_t i = 0; i < data.size(); i++)
	{
		if(data[i].size() == 0)
		{
			cout << "Warning: Detected empty triangulation. Ignoring...\n";
			continue;
		}

		size_t w_prev 	= 0; 		// Previous rank of the group of boundaries
		size_t z_cur  	= 0; 		// Current rank of the group of cycles
		vector<unsigned long> b_prev; 	// Torsion coefficients from the previous dimension

		size_t max_dim 	= data[i][0].vertices.size();

		vector<chain> boundaries;
		for(size_t j = 0; j < data[i].size(); j++)
			boundaries.push_back(data[i][j].boundary());

		out << "(";
		for(size_t dim = 0; dim < max_dim; dim++)
		{
			vector<simplex> generators 	= find_generators(boundaries);
			matrix matrix_boundaries 	= create_matrix(generators, boundaries);
			matrix matrix_snf		= matrix_boundaries.nf_smith();

			// Output/store the values for the next dimension
	
			z_cur = matrix_snf.get_num_zero_cols();
			out << z_cur - w_prev;
			if(b_prev.size() > 0)
			{
				for(size_t j = 0; j < b_prev.size(); j++)
					out << "+Z_" << b_prev[j]; // It is assumed that the coefficients are integers
			}
			
			if(dim < max_dim - 1)
				out << ",";

			w_prev = matrix_snf.get_num_non_zero_rows();
			b_prev = matrix_snf.get_torsion();

			// Prepare the generators for the next run

			boundaries.clear();
			for(size_t num_gens = 0; num_gens < generators.size(); num_gens++)
				boundaries.push_back(generators[num_gens].boundary());
		}
		
		out << ")\n";
		cout << "." << flush;

		dots++;
		if(dots >= 80)
		{
			cout << "\n";
			dots = 0;
		}
	}

	cout << "\n...finished.\n";
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

	if(!input.good())
	{
		cout << "Error: Could not process \"" << filename << "\".\n";
		return(result);
	}

	cout << "Processing \"" << filename << "\"...\n";

	while(input >> line)
	{
		lines++;

		// Whole set of simplices has been read
		if(line == "EOT")
		{
			result.push_back(cur_complex);
			cur_complex.clear();

			continue;
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
	cout << "Processed " << lines << " lines.\n";

	return(result);
}
