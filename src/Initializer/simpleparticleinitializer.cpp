#include <fstream>

#include "Initializer/simpleparticleinitializer.h"


SimpleParticleInitializer::SimpleParticleInitializer(GridLayout const& layout)
    : ParticleInitializer {},
      layout_{layout}
{

}



void SimpleParticleInitializer::loadParticles(std::vector<Particle>& particArray) const
{
    std::string file{"simpleParticleInit.txt"};

    std::cout << file << std::endl ;

    std::ifstream ifs{file};
    if (!ifs )
    {
        std::cout << "Could not open file : " << file << std::endl ;
        exit(-1);
    }

    uint32 nbrOfParticles ;

    ifs >> nbrOfParticles ;

    for(uint32 ik=0 ; ik<nbrOfParticles ; ++ik)
    {
        double weight ;
        double charge ;
        std::array<uint32, 3> icell ;
        std::array<float, 3> delta ;
        std::array<double, 3> v ;

        ifs >> weight ;
        ifs >> charge ;
        ifs >> icell[0] >> icell[1] >> icell[2] ;
        ifs >> delta[0] >> delta[1] >> delta[2] ;
        ifs >> v[0]     >> v[1]     >> v[2]     ;

        particArray.push_back( Particle(weight, charge, icell, delta, v) ) ;
    }


}

