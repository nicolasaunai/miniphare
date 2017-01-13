#ifndef FIELDBC_H
#define FIELDBC_H

#include <memory>

#include "types.h"

#include "vecfield/vecfield.h"
#include "Field/fieldbctype.h"




class FieldBC
{
protected:

    // Bridge pattern for FieldBC
    // FieldBCType might be periodic ...
    std::unique_ptr<FieldBCType> impl_ ;

    Edge edge_ ;

public:
    FieldBC( std::unique_ptr<FieldBCType> && impl, Edge edge  )
        : impl_{std::move(impl) }, edge_{edge} {}

    FieldBC(FieldBC const& source) = delete;
    FieldBC& operator=(FieldBC const& source) = delete;

    FieldBC(FieldBC&& toMove)      = default;
    FieldBC& operator=(FieldBC&& source) = default;

    ~FieldBC() = default ;

    void applyElectricBC( VecField & E )
    {
        impl_->applyElectricBC( E ) ;
    }

    void applyMagneticBC( VecField & B )
    {
        impl_->applyMagneticBC( B ) ;
    }

};


#endif // FIELDBC_H
