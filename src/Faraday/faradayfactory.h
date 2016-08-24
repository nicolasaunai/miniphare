#ifndef FARADAYFACTORY_H
#define FARADAYFACTORY_H


#include <memory>
#include <vector>

#include "Faraday/faraday.h"
#include "Faraday/faraday1d.h"
#include "types.h"



class FaradayFactory
{
public:
    static std::unique_ptr<Faraday> createFaradaySolver(double dt,
                                                        std::vector<double> meshSize,
                                                        uint32 nbDims)
    {
        switch (nbDims)
        {
            case 1:
                return std::unique_ptr<Faraday> ( new Faraday1D(dt, meshSize[0]) );

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




#endif // FARADAYFACTORY_H
