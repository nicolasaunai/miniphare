#ifndef FIELDBC_H
#define FIELDBC_H

#include <memory>

#include "types.h"

#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"



class FieldBC
{
protected:

    std::string  condition_ ;
    GridLayout  layout_ ;

    Edge edge_ ;

    double dx_ ;
    uint32 nbrCellx_ ;


public:
    FieldBC( std::string const & condition, GridLayout const & layout,
             Edge const & edge  )
        : condition_{condition}, layout_{layout}, edge_{edge} {}

    FieldBC(FieldBC const& source) = delete;
    FieldBC& operator=(FieldBC const& source) = delete;

    FieldBC(FieldBC&& toMove)      = default;
    FieldBC& operator=(FieldBC&& source) = default;

    virtual ~FieldBC() = default ;

    virtual void applyElectricBC( VecField & E ) = 0 ;

    virtual void applyMagneticBC( VecField & B ) = 0 ;

};


#endif // FIELDBC_H
