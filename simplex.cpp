/*!
	@file	simplex.cpp
	@brief	Contains implementations for the functions of the simplex class and
		the chain class.
*/

#include <iostream>
#include "simplex.h"

using namespace std; 

/// Determines whether two n-simplices are equal. They are considered to be equal
/// iff their vertices are equal.
///
/// @param 	a	The other simplex.

bool simplex::operator==(simplex a)
{
	for(size_t i = 0; i < vertices.size(); i++)
		if(vertices.at(i) != a.vertices.at(i))
			return(false);

	return(true);		
}

/// Prints an affine n-simplex to STDOUT. This is useful for displaying intermediate 
/// results and debugging the program.

void simplex::print(void)
{
	cout << "[";
	for(size_t i = 0; i < vertices.size(); i++)
	{
		cout << vertices[i];
		if(i < vertices.size() - 1)
			cout << ",";
	}
	cout << "]";
}

/// Computes the boundary of the current affine n-simplex. The boundary is
/// an (n-1)-chain. Thus, it can be described as a vector of
/// simplices.
///
/// @return The boundary as a (n-1)-chain.

chain simplex::boundary(void)
{
	chain res;
	chain_element e;

	short sign = 1;
	
	vector<unsigned int>::iterator it;
	for(size_t i = 0; i < vertices.size(); i++)
	{
		simplex tmp = *this;

		tmp.vertices.erase(tmp.vertices.begin() + i);	// erase the i-th vertex...
		e.s = tmp;					// ...assign it to the chain element...
		e.c = sign;					// ...and set the sign.

		sign *= (-1);
		res.elements.push_back(e);
	}
	
	return(res);
}

/// Prints an i-chain to STDOUT. This is useful for displaying intermediate
/// results and for debugging purposes.

void chain::print(void)
{
	for(size_t i = 0; i < elements.size(); i++)
	{
		cout << (elements[i].c > 0 ? "+" : "");
		cout << elements[i].c << "*";
		
		elements[i].s.print();
	}

	cout << "\n";
}
