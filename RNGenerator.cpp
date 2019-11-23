#include "RNGenerator.h"

RNGenerator::RNGenerator()
{
}

RNGenerator::~RNGenerator()
{
}

uint RNGenerator::GetIntRN()
{
	return UIntDistribution(DRNEngine);
}

uint RNGenerator::GetIntRNInRange(uint MIN, uint MAX)
{

	std::uniform_int_distribution<uint> tmp_IntDistribution(MIN, MAX);
	return tmp_IntDistribution(DRNEngine);

}
