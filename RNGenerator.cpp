#include "RNGenerator.h"

RNGenerator::RNGenerator()
{
}

RNGenerator::~RNGenerator()
{
}

UID RNGenerator::GetIntRN()
{
	return UIntDistribution(DRNEngine);
}

UID RNGenerator::GetIntRNInRange(UID MIN, UID MAX)
{

	std::uniform_int_distribution<UID> tmp_IntDistribution(MIN, MAX);
	return tmp_IntDistribution(DRNEngine);

}
