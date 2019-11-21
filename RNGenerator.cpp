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

int RNGenerator::GetIntRNInRange(uint MIN, uint MAX)
{

	std::uniform_int_distribution<uint> tmp_IntDistribution(MIN, MAX);
	return tmp_IntDistribution(DRNEngine);

}
