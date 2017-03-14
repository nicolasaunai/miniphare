#include "distribgenerator.h"



DistribGenerator::DistribGenerator()
{

}


void DistribGenerator::setStrategy(std::shared_ptr<DistribStrategy> strategy)
{
   this->p_strategy = strategy;
}
