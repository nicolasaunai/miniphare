#ifndef OHMIMPL1D_H
#define OHMIMPL1D_H


#include "Ohm/ohm.h"



class OhmImpl1D : public OhmImpl , private OhmImplInternals
{
private:
    // Field derivatives
    // Field idealTerm
    // pressureTerm
    // inertiaTerm

public:

    explicit OhmImpl1D(GridLayout const& layout);


    virtual void operator()(VecField const& B, Field const& Ne,
                            VecField const& Ve, Field const& Pe, VecField& Enew);

     ~OhmImpl1D();
};




#endif // OHMIMPL1D_H