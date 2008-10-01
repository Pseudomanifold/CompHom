/*!
	@file 	matrix.cpp
	@brief	Contains functions for arbitrary (n,n)-matrices
*/


#include <iostream>
#include <cstdlib>

#include "matrix.h"

/// Assigns an array of longs to the matrix. The current data is discarded.
/// This operation changes the dimensions of the matrix, of course. The array
/// will be embedded like this: 0,1,2,..,m for the first row, m+1,...,2m for
/// the second row and so on.
///
/// @param a	The array that is to be assigned to the matrix.
/// @param n	The number of rows in the array.
/// @param m	The number of columns in the array.
///
/// @return	The new matrix.

matrix matrix::assign(long* a, unsigned long n, unsigned long m)
{
	data.clear();
	
	vector<long> row;
	for(unsigned long i = 0; i < n*m; i += m)
	{
		row.clear();
		
		for(unsigned long j = 0; j < m; j++)
			row.push_back(a[i+j]);
		
		data.push_back(row);	
	}

	return(*this);
}

/// Assigns a vector of longs to the matrix. This function is an overloaded
/// version of the assign() funtions that assigns an array of longs. 
///
/// @param a	The vector that is to be assigned to the matrix.
/// @param n	The number of rows in the vector.
/// @param m	The number of columns in the vector.
///
/// @return	The new matrix.

matrix matrix::assign(vector<long> a, unsigned long n, unsigned long m)
{
	data.clear();
	
	vector<long> row;
	for(unsigned long i = 0; i < n*m; i += m)
	{
		row.clear();
		
		for(unsigned long j = 0; j < m; j++)
			row.push_back(a[i+j]);
		
		data.push_back(row);	
	}

	return(*this);
}


/// Transforms the current matrix into its Smith Normal Form (abbreviated SNF).
/// Data that is currently held by the matrix will not be changed.
///
/// @return A copy of the matrix in SNF.

matrix matrix::nf_smith(void)
{
	matrix res = *this;
	res.nf_smith_rec(0,0);
	return(res);
}

/// This is the recursive helper function that is called by nf_smith() in order
/// to actually transform the matrix. In each call, the dimensions of the matrix
/// that is processed are reduced. Data of the current matrix is changed, obviously.
///
/// @param row	Starting row
/// @param col	Starting column
 
void matrix::nf_smith_rec(size_t row, size_t col)
{
	if(	(get_num_rows() - row) == 0 || 
		(get_num_cols() - col) == 0)
		return;

	// If the matrix is the zero matrix, we can also quit
	
	bool zero = true;
	for(size_t i = row; i < data.size(); i++)
	{
		for(size_t j = col; j < data[0].size(); j++)
		{
			if(data[i][j] != 0)
				zero = false;
		}
	}

	if(zero)
		return;
	
	ldiv_t res;	
	size_t min_i 	= row;
	size_t min_j 	= col;

	// Search for the smallest element in the matrix.

	for(size_t i = row; i < data.size(); i++)
	{
		for(size_t j = col; j < data[i].size(); j++)
		{
			// The minimal element is changed if...
			//
			// 1) 	A smaller element has been found.
			// 2) 	It is zero, in which case it is assigned the next non-zero
			//	element.

			if(	(abs(data[i][j]) <= abs(data[min_i][min_j]) && data[i][j] != 0) ||
				(data[min_i][min_j] == 0 && data[i][j] != 0))
			{
				min_i 	= i;
				min_j 	= j;
			}
		}
	}

	// Swap the first row with the row that contains the minimum and swap
	// the first column with the column that contains the minimum.

	data[row].swap(data[min_i]);
	for(size_t i = row; i < data.size(); i++)
	{
		long tmp 		= data[i][min_j];
		data[i][min_j] 		= data[i][col];
		data[i][row]		= tmp;
	}

	// Eliminate a row. If not all remainders are zero, a new minimal
	// element must be found.

	for(size_t j = col+1; j < data[0].size(); j++)
	{
		res = div(data[row][j], data[row][col]);

		// Subtract q*(first column) from the current column. In
		// effect, after this transformation, the first row consists of
		// the minimal element and the remainders of the division

		for(size_t i = row; i < data.size(); i++)
			data[i][j] -= res.quot*data[i][col];
	}

	for(size_t j = col+1; j < data[0].size(); j++)
	{
		if(data[row][j] != 0)
			nf_smith_rec(row, col);
	}
	
	// Eliminate a column. If not all remainders are zero, a new minimal
	// element must be found.

	for(size_t i = row+1; i < data.size(); i++)
	{
		res = div(data[i][col], data[row][col]);

		// Subtract q*(first row) from the current row. In effect,
		// after this transformation, the first row consists of the
		// minimal element and the remainders of the division

		for(size_t j = col; j < data[i].size(); j++)
			data[i][j] -= res.quot*data[row][j];
	}

	for(size_t i = row+1; i < data.size(); i++)
	{
		if(data[i][col] != 0)
			nf_smith_rec(row, col);
	}

	// Now the first row and the first column are eliminated (excluding one
	// element). Check if the minimal element divides every entry of the
	// remaining matrix.

	for(size_t i = row+1; i < data.size(); i++)
	{
		for(size_t j = col+1; j < data[i].size(); j++)
		{
			res = div(data[i][j], data[row][col]);
			if(res.rem != 0)
			{
				// Add the i-th row to the first row and divide
				// the current element by the minimal element
				// (already stored in res).

				for(size_t k = col+1; k < data[i].size(); k++)
					data[row][k] += data[i][k];

				// Substract q*(first column) from the j-th column and
				// repeat this whole function.

				for(size_t k = row+1; k < data.size(); k++)
					data[k][j] -= res.rem*data[k][col];

				nf_smith_rec(row, col);
				return;
			}
		}
	}

	// Reduce the dimensions and proceed with the reduced 
	// matrix. After finitely many steps, the matrix will
	// be in SNF.

	nf_smith_rec(row + 1, col + 1);
}

/// Transposes the matrix.
void matrix::transpose(void)
{
	matrix transposed;
	long tmp;
	for(size_t j = 0; j < data[0].size(); j++)
	{
		vector<long> row;
		for(size_t i = 0; i < data.size(); i++)
			row.push_back(data[i][j]);
		
		transposed.data.push_back(row);	
	}

	*this = transposed;
}

/// Prints the matrix to STDOUT. This is used for debugging purposes and
/// for displaying intermediate results.

void matrix::print(void)
{
	cout << "*********************************************\n";
	for(size_t i = 0; i < data.size(); i++)
	{
		for(size_t j = 0; j < data[i].size(); j++)
			cout << data[i][j] << "\t";
		
		cout << "\n";	
	}
	cout << "*********************************************\n";
}

/// Returns the number of rows in the matrix.
/// @return The number of rows.

size_t matrix::get_num_rows(void)
{
	return(data.size());
}

/// Returns the number of columns in the matrix.
/// @return The number of columns.

size_t matrix::get_num_cols(void)
{
	if(data.size() == 0)
		return(0);
	else
		return(data[0].size());	
}

/// Returns the number of zero columns in the matrix. This is used to compute
/// the order of Z_n. It is assumed that the matrix is already in SNF.
///
/// @return The number of zero columns.

size_t matrix::get_num_zero_cols(void)
{
	size_t i = 0;
	for(; i < get_num_cols() && i < get_num_rows(); i++)
	{
		if(data[i][i] == 0)
		{
			return(get_num_cols() - i);
		}
	}

	return(get_num_cols() - i);
}

/// Returns the number of non-zero rows in the matrix. This is used to compute
/// the order of W_(n-1). It is assumed that the matrix is already in SNF.
///
/// @return The number of non-zero rows.

size_t matrix::get_num_non_zero_rows(void)
{
	size_t rows = 0;
	for(size_t i = 0; i < get_num_cols() && i < get_num_rows(); i++)
	{
		if(data[i][i] != 0)
			rows++;
	}

	return(rows);
}

/// Given a matrix in Smith Normal Form (which is assumed throughout the
/// function), return all elements that have an absolute value > 1. These
/// numbers constitute the torsion coefficients.
///
/// @return 	A vector that contains the absolute values of 
///		the torsion coefficients.
vector<unsigned long> matrix::get_torsion(void)
{
	vector<unsigned long> result;
	for(size_t i = 0; i < get_num_cols() && i < get_num_rows(); i++)
	{
		if(labs(data[i][i]) > 1)
			result.push_back(labs(data[i][i]));

	}

	return(result);
}
