#ifndef IONS_H
#define IONS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Field/field.h"
#include "Initializer/ionsinitializer.h"
#include "grid/gridlayout.h"
#include "species.h"
#include "types.h"


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
    std::vector<Species> speciesArray_;
    std::unordered_map<std::string, uint32> name2ID_;
    GridLayout layout_;
    Field rho_;
    VecField bulkVel_;


public:
    Ions(GridLayout const& layout, std::unique_ptr<IonsInitializer> ionInitializer);

    Ions(Ions&& source) = default;
    Ions& operator=(Ions&& source) = default;

    Ions(Ions const& source) = delete;
    Ions& operator=(Ions const& source) = delete;


    void resetBulkMoments()
    {
        rho_.zero();
        bulkVel_.zero();
    }
    void resetSpeciesMoments()
    {
        for (Species& spe : speciesArray_)
            spe.resetMoments();
    }

    void resetParticles()
    {
        for (Species& spe : speciesArray_)
            spe.resetParticles();
    }

    uint32 nbrSpecies() const { return static_cast<uint32>(speciesArray_.size()); }

    uint32 population() const;

    uint32 speciesID(std::string name) const
    {
        auto search = name2ID_.find(name);
        if (search != name2ID_.end())
            return search->second;
        else
            throw std::runtime_error("No Such Species");
    }

    Species& species(uint32 index);
    Species const& species(uint32 index) const;
    Species const& species(std::string name) const;
    Species& species(std::string name);

    // std::vector<Species>& species() {return speciesArray_;}

    Field& rho() { return rho_; }
    Field const& rho() const { return rho_; }

    Field& bulkVel(uint32 iComponent) { return bulkVel_.component(iComponent); }
    Field const& bulkVel(uint32 iComponent) const { return bulkVel_.component(iComponent); }

    VecField& bulkVel() { return bulkVel_; }
    VecField const& bulkVel() const { return bulkVel_; }

    void loadParticles();

    void computeChargeDensity();
    void computeBulkVelocity();
};




#endif // IONS_H
