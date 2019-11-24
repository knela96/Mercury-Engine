#ifndef RNGENERATOR
#define RNGENERATOR
#include <random>
#include "Globals.h"

class RNGenerator
{
public:
	RNGenerator();
	~RNGenerator();

	UID GetIntRN(); //Shouldn't give negative numbers
	UID GetIntRNInRange(UID MIN = 0, UID MAX = 999999999);

private:
	
	std::default_random_engine DRNEngine;
	std::random_device RandomDevice;
	std::uniform_int_distribution<UID>UIntDistribution;
};

#endif