#ifndef RNGENERATOR
#define RNGENERATOR
#include <random>


class RNGenerator
{
public:
	RNGenerator();
	~RNGenerator();


	int GetIntRN(); //Shouldn't give negative numbers
	int GetIntRNInRange(int MIN = 0, int MAX = 999999999);

private:
	
	std::default_random_engine DRNEngine;
	std::random_device RandomDevice;
	std::uniform_int_distribution<int>UIntDistribution;
};







#endif