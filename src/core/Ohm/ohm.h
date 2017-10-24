#ifndef OHM_H
#define OHM_H

#include <memory>


#include "data/Field/field.h"
#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"




// comment here
class OhmImpl
{
protected:
    virtual void ideal_(VecField const& Ve, VecField const& B) = 0;
    virtual void resistive_(VecField const& J) = 0;
    virtual void pressure_(Field const& Pe, Field const& Ne) = 0;

public:
    VecField idealTerm_;
    VecField pressureTerm_;
    VecField resistivityTerm_;
    VecField hyperResistivityTerm_;
    GridLayout layout_;
    double eta_;
    double nu_;


    OhmImpl(GridLayout const& layout, double eta, double nu);


    virtual void computeTerms(VecField const& B, Field const& Ne, VecField const& Ve,
                              Field const& Pe, VecField const& J)
        = 0;


    virtual ~OhmImpl();
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


    // ohm(Bpred, Ve, Pe, Epred);//
    void operator()(VecField const& B, Field const& Ne, VecField const& Ve, Field const& Pe,
                    VecField const& J, VecField& Enew);
};




#endif // OHM_H
