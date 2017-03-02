#ifndef ELECTRONS_H
#define ELECTRONS_H


#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"


class Electrons
{
private:

    VecField   Ve_;
    Field      Pe_;
    double Te_;
    GridLayout layout_;

public:
    Electrons(GridLayout const& layout, double Te);
    const VecField& bulkVel(VecField const& Vi, Field const& Ni, VecField const& J);
    const Field& pressure(Field const& Ni);



};

#endif // ELECTRONS_H
