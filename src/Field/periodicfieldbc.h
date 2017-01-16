#ifndef PERIODICFIELDBC_H
#define PERIODICFIELDBC_H


#include "Field/fieldbc.h"



class PeriodicFieldBC : public FieldBC
{
private:


public:

    PeriodicFieldBC( std::string const & condition,
                     GridLayout const & layout,
                     Edge const &  edge  )
        : FieldBC(condition, layout, edge) {}

    virtual ~PeriodicFieldBC() = default ;

    virtual void applyElectricBC( VecField & E ) override ;

    virtual void applyMagneticBC( VecField & B ) override ;

};




#endif // PERIODICFIELDBC_H
