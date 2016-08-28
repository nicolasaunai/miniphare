#ifndef FARADAYFACTORY_H
#define FARADAYFACTORY_H


#include <memory>
#include <vector>

#include "Faraday/faraday.h"
#include "faradayimpl1d.h"
#include "types.h"
#include "grid/gridlayout.h"



class FaradayImplFactory
{
public:
    static std::unique_ptr<FaradayImpl> createFaradayImpl(double dt, GridLayout const& layout)
    {
        switch (layout.nbDimensions())
        {
            case 1:
                return std::unique_ptr<FaradayImpl> ( new FaradayImpl1D(dt, layout));

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
