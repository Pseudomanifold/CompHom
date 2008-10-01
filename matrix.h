/*!
	@file	matrix.h
	@brief	A class to handle (n,n)-matrices
*/

#ifndef COMPTOP_MATRIX_H
#define COMPTOP_MATRIX_H

#include <vector>

using namespace std;

/*!
	@class matrix
	@brief A class to handle (n,n)-matrices
*/

class matrix
{
	public:

	matrix assign(long* a, unsigned long n, unsigned long m);
	matrix assign(vector<long> a, unsigned long n, unsigned long m);

	matrix nf_smith(void);
	matrix nf_frobenius(void);
	matrix nf_jordan(void);

	void transpose(void);	
	void print(void);

	size_t get_num_rows(void);
	size_t get_num_cols(void);
	size_t get_num_zero_cols(void);
	size_t get_num_non_zero_rows(void);
	
	vector<unsigned long> get_torsion(void);
	
	private:

	vector< vector<long> > data;

	void nf_smith_rec(size_t row, size_t col);
	void nf_frobenius_rec(size_t row, size_t col);
	void nf_jordan_rec(size_t row, size_t col);
};

#endif
