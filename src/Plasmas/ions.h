#ifndef IONS_H
#define IONS_H

#include <vector>

#include "species.h"
#include "Field/field.h"
#include "grid/gridlayout.h"
#include "types.h"
#include "Initializer/ionsinitializer.h"


/**
 * @brief The Ions class holds data dealing with ions. It holds ion Species
 *        themselves holding Particle data.
 *
 * Because it generally holds a **lot** of data, Ions object are move-only.
 * Ion objects give access to total ion quantities, such as:
 *
 *  - the total ion charge density
 *  - the total ion bulk velocity, etc.
 *
 * Ions can also be used to access Species.
 *
 */
class Ions
{    

private:

    std::vector <Species> speciesArray_;
    GridLayout layout_;
    Field rho_;
    VecField bulkVel_;


public:

    Ions(GridLayout const& layout, std::unique_ptr<IonsInitializer> ionInitializer);

    Ions(Ions&& source) = default;
    Ions& operator=(Ions&& source) = default;

    Ions(Ions const& source) = delete;
    Ions& operator=(Ions const& source) = delete;


    void resetBulkMoments(){rho_.zero(); bulkVel_.zero();}
    void resetSpeciesMoments(){for (Species& spe : speciesArray_) spe.resetMoments();}


    uint32 nbrSpecies() const {return static_cast<uint32>(speciesArray_.size()) ;}
    Species& species(uint32 index);
    Species const& species(uint32 index) const;


    Field& rho() {return rho_;}
    Field const& rho() const {return rho_;}

    Field& bulkVel(uint32 iComponent) {return bulkVel_.component(iComponent);}
    Field const& bulkVel(uint32 iComponent) const {return bulkVel_.component(iComponent);}

    void loadParticles();


    void computeChargeDensity();
    void computeBulkVelocity();

};




#endif // IONS_H




