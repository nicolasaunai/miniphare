#ifndef OHM_H
#define OHM_H

#include <memory>


#include "vecfield/vecfield.h"
#include "Field/field.h"
#include "grid/gridlayout.h"




// comment here
class OhmImpl
{
    public:
    // ohm(Bpred, Ve, Pe, Epred);
        virtual void operator()(VecField const& B, Field const& Ne,
                                VecField const& Ve, Field const& Pe, VecField& Enew) = 0;

    //        virtual void operator()(VecField const& B, VecField const& Ve, TensorField const& Pe, VecField& Enew) = 0;

        virtual ~OhmImpl();
};





class OhmImplInternals
{
public:
    OhmImplInternals(GridLayout const& layout);

protected:
    VecField idealTerm_;
    VecField pressureTerm_;
    //VecField resistivityTerm_; // eta*J ; eta = scalar OR field

    // resistivity
    // hyper-rest.
    GridLayout layout_;
};






// comment here
class Ohm
{
private:

    std::unique_ptr<OhmImpl> implPtr_;


public:

    // TODO : faraday object should take start and end "iterators"
    // those are given by SOlver which takes them from GridLayout
    explicit Ohm(GridLayout const& layout);


#if 0


    Faraday(Faraday const& source) = delete;
    Faraday& operator=(Faraday const& source) = delete;

    Faraday(Faraday&& source)            = default;
    Faraday& operator=(Faraday&& source) = default;
#endif


     //ohm(Bpred, Ve, Pe, Epred);//
    void operator()(VecField const& B, Field const& Ne, VecField const& Ve,
                         Field const& Pe, VecField& Enew);

};




#endif // OHM_H
