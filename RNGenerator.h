#ifndef RNGENERATOR
#define RNGENERATOR
#include <random>
#include "Globals.h"


class RNGenerator
{
public:
	RNGenerator();
	~RNGenerator();


	uint GetIntRN(); //Shouldn't give negative numbers
	uint GetIntRNInRange(uint MIN = 0, uint MAX = 999999999);

private:
	
	std::default_random_engine DRNEngine;
	std::random_device RandomDevice;
	std::uniform_int_distribution<uint>UIntDistribution;
};







#endif