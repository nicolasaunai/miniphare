#include "maxwellianstrategy.h"


MaxwellianStrategy::MaxwellianStrategy()
    :DistribStrategy("Maxwellian")
{
}




MaxwellianStrategy::~MaxwellianStrategy()
{
//    std::cout << "destruction of Maxwellian strategy" << std::endl;
}





void MaxwellianStrategy::draw(std::vector<double> &array,
                             uint64 nbElemts,
                             double mean,
                             double std) const
{
 //   std::cout<< "Applying Box-Muller Strategy to draw "
 //            << nbElemts << " elements" << std::endl;

    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());
    std::normal_distribution<> maxwell(mean,std);

   for (unsigned int i=0; i < nbElemts; ++i)
   {
       array.push_back(maxwell(generator));
   }
}


void MaxwellianStrategy::draw(double & value,
                           double mean,
                           double std) const
{
    std::random_device randSeed;
    std::mt19937_64 generator(randSeed());
    std::normal_distribution<> maxwell(mean,std);

    value = maxwell(generator) ;
}


//! ----------------------------------------------------------------------------
//! aleat_gauss generates a Gaussian distribution with the Box-Muller method  :
//!   $y_1 = sqrt(-2.log(x_1)) cos(2.\pi x_2)$
//!   $y_2 = sqrt(-2.log(x_1)) sin(2.\pi x_2)$
//! where $x_1$ and $x_2$ are two uniform deviates.
//!
//! Then $y_1$ and $y_2$ are two centered and reduced Gaussian deviates.
//! $\mathcal{N}(\mu,\sigma^2) = \mathcal{N}(0, 1)$
//!
//! Note that we only use y_1
//! ----------------------------------------------------------------------------
void MaxwellianStrategy::aleat_gaussBox( std::vector<double> &array,
                                         unsigned int nbElemts )
{
    std::vector<double> x1, x2, r, alpha ;

    unsigned int bunch_size = nbElemts ;

    array.assign(bunch_size, 0.);

    x1.assign(bunch_size, 0.);
    x2.assign(bunch_size, 0.);
    r.assign(bunch_size, 0.);
    alpha.assign(bunch_size, 0.);

    srand(time(NULL));
    for(unsigned int i=0 ; i<bunch_size ; i++)
    {
        x1[i] = (double)rand()/(double)RAND_MAX ;
        x2[i] = (double)rand()/(double)RAND_MAX ;
    }

    //   note : we prevent log(0) by using (1-r) instead of r.
    //   Since 0 <= r < 1. So 0 < (1-r) <= 1
    for(unsigned int i=0 ; i<bunch_size ; i++)
    {
        r[i]     = std::sqrt(-2.*std::log( 1.-x1[i] )) ;
        alpha[i] = 2. * M_PI * x2[i] ;
    }

    for(unsigned int i=0 ; i<bunch_size ; i++)
    {
        array[i] = r[i]*std::cos( alpha[i] ) ;
    }

//    std::cout << "Gaussian : " << std::endl ;
//    for(unsigned int i=0 ; i<bunch_size ; i++)
//    {
//        std::cout << array[i] << " " << std::endl ;
//    }
//    std::cout << std::endl ;

}

//! ----------------------------------------------------------------------------
//! We apply the Central Limit Theorem
//!
//! The multiplicative term is equal to sqrt(12/N)
//! with N the bunch size
//!
//!
//! ----------------------------------------------------------------------------


void MaxwellianStrategy::aleat_gauss2( std::vector<double> &array,
                                       unsigned int nbElemts,
                                       unsigned int Nmix  )
{
    double ** random_array = nullptr ;

    double mean  = 0.5 ;
    double sigma = 1./std::sqrt(12.);

    double mval1=0., mval2=0. ;

    size_t bunch_size = nbElemts ;

    random_array = new double* [Nmix] ;
    for( unsigned int i=0 ; i<Nmix ; i++)
    {
        random_array[i] = new double[bunch_size] ;
    }

    srand(time(NULL));
    for( unsigned int imix=0 ; imix<Nmix ; imix++)
    {
        for( unsigned int ik=0 ; ik<bunch_size ; ik++)
        {
            random_array[imix][ik] = (double)rand()/(double)RAND_MAX ;
        }
    }

    for( unsigned int imix=0 ; imix<Nmix ; imix++)
    {
        for( unsigned int ik=0 ; ik<bunch_size ; ik++)
        {
            array[ik] = array[ik] + random_array[imix][ik] ;
        }
    }

    mval1 = Nmix*mean ;
    mval2 = sigma*std::sqrt((double)Nmix) ;
    for( unsigned int ik=0 ; ik<bunch_size ; ik++)
    {
        array[ik] = ( array[ik] - mval1 )/mval2 ;
    }

    for( unsigned int imix=0 ; imix<Nmix ; imix++)
    {
        delete random_array[imix] ;
    }

    delete[] random_array;

}


