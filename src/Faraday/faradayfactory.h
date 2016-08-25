#ifndef FARADAYFACTORY_H
#define FARADAYFACTORY_H


#include <memory>
#include <vector>

#include "Faraday/faraday.h"
#include "types.h"



class FaradayImplFactory
{
public:
    static std::unique_ptr<FaradayImpl> createFaradayImpl(std::vector<double> meshSize,
                                                          uint32 nbDims)
    {
        switch (nbDims)
        {
            case 1:
                return std::unique_ptr<FaradayImpl> ( new FaradayImpl1D );

#if 0
            case 2:
                std::cout << "not yet implemented\n";
                break;
#endif

            default:
                throw std::runtime_error("Error : FaradayFactory - dimension must be either 1D, 2D or 3D");
        }
    }
};




#if 0


class FaradayPolyImpl
{
    public:
        virtual double operator()(double a) = 0;
};




class FaradayPolyImplFactory
{
    public:
    static std::unique_ptr<FaradayPolyImpl>createFaradayPolyImpl(int dim)
    {
        switch(dim)
        {
            case 1:
              return std::unique_ptr<FaradayPolyImpl> (new Faraday1D);

             case 2:
                return std::unique_ptr<FaradayPolyImpl> (new Faraday2D);

            default:
                std::cout << "nope...\n";
                return nullptr;
        }
    }
};



#endif



#endif // FARADAYFACTORY_H
