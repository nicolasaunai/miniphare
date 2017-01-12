#ifndef PERIODICFIELDBC_H
#define PERIODICFIELDBC_H


#include "Field/fieldbctype.h"



class PeriodicFieldBC : public FieldBCType
{
private:


public:

    PeriodicFieldBC( std::string const & BCType,
                     GridLayout const & layout )
        : FieldBCType(BCType, layout) {}

    virtual ~PeriodicFieldBC() {}

    virtual void applyElectricBC( VecField & E ) override ;

    virtual void applyMagneticBC( VecField & B ) override ;

};




#endif // PERIODICFIELDBC_H
