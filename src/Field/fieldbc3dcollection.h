#ifndef FIELDBC3DCOLLECTION_H
#define FIELDBC3DCOLLECTION_H

#include <vector>
#include <memory>

#include "Field/fieldbc.h"
#include "vecfield/vecfield.h"


class CollectionOfBoundaryConditions
{
private:
    std::vector< std::unique_ptr<FieldBC> > allFieldsBoundaryConditions_ ;

public:
    CollectionOfBoundaryConditions() {}

    CollectionOfBoundaryConditions( std::vector<std::unique_ptr<FieldBC>> && collectionOfFieldsBC )
        : allFieldsBoundaryConditions_{std::move(collectionOfFieldsBC)} {}

    CollectionOfBoundaryConditions(CollectionOfBoundaryConditions&& toMove)      = default;
    CollectionOfBoundaryConditions& operator=(CollectionOfBoundaryConditions&& source) = default;

    ~CollectionOfBoundaryConditions() = default ;

    void applyElectricBC( VecField & E ) ;
    void applyMagneticBC( VecField & B ) ;

};



#endif // FIELDBC3DCOLLECTION_H
