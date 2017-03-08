#ifndef OHMIMPL1D_H
#define OHMIMPL1D_H


#include "Ohm/ohm.h"



class OhmImpl1D : public OhmImpl
{
private:
    // Field derivatives
    // Field idealTerm
    // pressureTerm
    // inertiaTerm

    void ideal_(VecField const& Ve, VecField const& B);
    void resistive_(VecField const& J);
    void pressure_(Field const& Pe);

public:

    explicit OhmImpl1D(GridLayout const& layout, double eta, double nu);


    virtual void operator()(VecField const& B, Field const& Ne,
                            VecField const& Ve, Field const& Pe,
                            VecField const&J, VecField& Enew);

     ~OhmImpl1D();
};




#endif // OHMIMPL1D_H
