/*!
	@file	simplex.h
	@brief	Contains classes describing affine n-simplices, simplicial i-chains
		and more.
*/

#ifndef CT_SIMPLEX_H
#define CT_SIMPLEX_H

#include <vector>

class simplex;
class chain;

/*!
	@class 	simplex
	@brief	Describes an affine n-simplex and several operations.
*/

class simplex
{
	public:
	
	bool operator==(simplex a);

	chain boundary(void);

	void print(void);
	
	std::vector<unsigned long> vertices;
};

/*!
	@class 	chain_element
	@brief	Stores elements of a chain.
*/

class chain_element
{
	public:

	long c;		///< chain coefficient
	simplex s;	///< corresponding simplex
};



/*!
	@class	chain
	@brief	Describes an i-chain (i.e. a chain of i-simplices with coefficients in Z).

*/

class chain
{
	public:

	void print(void);
	std::vector<chain_element> elements;
};

#endif
