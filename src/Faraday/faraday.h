#ifndef FARADAY_H
#define FARADAY_H

#include <memory>


#include "Vecfield/vecfield.h"

class Faraday
{
private:
    double dt_;


public:

    // TODO : faraday object should take start and end "iterators"
    // those are given by SOlver which takes them from GridLayout
    explicit Faraday(double dt):dt_{dt}{}
    Faraday(Faraday const& source) = default;

    virtual void operator()(VecField const& E,
                            VecField const& B,
                            VecField& Bnew) = 0;


    virtual Faraday* clone() = 0;

};

#endif // FARADAY_H
