#include "uniformstrategy.h"


UniformStrategy::UniformStrategy()
    :DistribStrategy("Uniform")
{
}

UniformStrategy::~UniformStrategy()
{
//    std::cout << "destruction of Uniform strategy" << std::endl;
}

void UniformStrategy::draw(std::vector<double> &array,
                             uint64 nbElemts,
                             double bmin,
                             double bmax ) const
{
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    std::uniform_real_distribution<> unif(bmin, bmax);

   for (unsigned int i=0; i < nbElemts; ++i)
   {
       array.push_back(unif(generator));
   }
}


void UniformStrategy::draw(double & value,
                        double bmin,
                        double bmax ) const
{
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());

    std::uniform_real_distribution<> unif(bmin, bmax);

    value = unif(generator) ;
}


