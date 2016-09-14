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
    Field bulkVel_;


public:

    Ions(GridLayout const& layout, IonsInitializer const& ionInitializer);
    //Ions(GridLayout&& layout);

    Ions(Ions&& source) = default;
    Ions& operator=(Ions&& source) = default;

    Ions(Ions const& source) = delete;
    Ions& operator=(Ions const& source) = delete;



    void resetBulkMoments(){rho_.zero(); bulkVel_.zero();}

    void resetSpeciesMoments(){for (Species& spe : speciesArray_) spe.resetMoments();}

    uint32 nbrSpecies() const {return static_cast<uint32>(speciesArray_.size()) ;}




    Species& species(uint32 index);

    Species const& species(uint32 index) const;

    void loadParticles();

    void computeChargeDensity();



#if 0
    Field* getChargeDensity();
    Field* getBulkVelocity();

    void computeChargeDensity();
    void computeBulkVelocity();

    void load();

    unsigned int getNbSpecies() const;



private:
    std::vector<Species> _speciesArr;
    Field* _rho;
    Field* _v;


    void _resetFields();
#endif

};

#endif // IONS_H
