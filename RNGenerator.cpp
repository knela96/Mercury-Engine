#include "RNGenerator.h"






RNGenerator::RNGenerator()
{
}

RNGenerator::~RNGenerator()
{
}

int RNGenerator::GetIntRN()
{
	return UIntDistribution(DRNEngine);
}

int RNGenerator::GetIntRNInRange(int MIN, int MAX)
{

	std::uniform_int_distribution<int> tmp_IntDistribution(MIN, MAX);
	return tmp_IntDistribution(DRNEngine);

}
