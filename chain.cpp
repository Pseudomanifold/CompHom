/*!
	@file	chain.cpp
	@brief	Functions for simplicial chains.
*/

#include <iostream>
#include "chain.h"

using namespace std;

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

