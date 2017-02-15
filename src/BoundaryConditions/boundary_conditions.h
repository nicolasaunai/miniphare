#ifndef FIELDBC3DCOLLECTION_H
#define FIELDBC3DCOLLECTION_H

#include <vector>
#include <memory>

#include "vecfield/vecfield.h"


class BoundaryCondition
{
public:
    virtual void applyMagneticBC(VecField& B) const = 0;
    virtual void applyElectricBC(VecField& E) const = 0;
    virtual void applyCurrentBC(VecField& J)  const = 0;

    virtual ~BoundaryCondition() = 0;
};



/*
class BoundaryConditions
{

private:
    std::vector< std::unique_ptr<FieldBC> > fieldBoundaryConditions_ ;
    std::vector< std::unique_ptr<ParticlesBC> > particlesBoundaryConditions_ ;


public:
    BoundaryConditions() {}

    BoundaryConditions( std::vector<std::unique_ptr<FieldBC>> && fieldsBCs )
        : fieldBoundaryConditions_{std::move(fieldsBCs)} {}

    BoundaryConditions(BoundaryConditions&& toMove)      = default;
    BoundaryConditions& operator=(BoundaryConditions&& source) = default;

    ~BoundaryConditions() = default ;

    std::vector< std::unique_ptr<FieldBC> > &
    fieldBoundaryConditions() { return fieldBoundaryConditions_ ; }

    void applyElectricBC( VecField & E ) ;
    void applyMagneticBC( VecField & B ) ;
    void applyCurrentBC ( VecField & J ) ;

};*/



#endif // FIELDBC3DCOLLECTION_H
