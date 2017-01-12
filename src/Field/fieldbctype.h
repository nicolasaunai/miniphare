#ifndef FIELDBCTYPE_H
#define FIELDBCTYPE_H

#include "types.h"

#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"


class FieldBCType
{
protected:
    std::string  boundaryCondition_ ;

    double dx_ ;
    uint32 nbrCellx_ ;

public:
    FieldBCType( std::string const & BCType,
                 GridLayout const & layout)
        : boundaryCondition_{BCType}
    {
        dx_ = layout.dx() ;
        nbrCellx_ = layout.nbrCellx() ;
    }

    FieldBCType(FieldBCType && toMove)      = default;
    FieldBCType& operator=(FieldBCType && source) = default;

    // Dont't forget =default HERE
    // or move operations won't be generated
    virtual ~FieldBCType() = default ;


    virtual void applyElectricBC( VecField & E ) = 0 ;

    virtual void applyMagneticBC( VecField & B ) = 0 ;

};


#endif // FIELDBCTYPE_H
