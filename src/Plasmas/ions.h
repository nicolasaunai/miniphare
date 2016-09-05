#ifndef IONS_H
#define IONS_H

#include <vector>

#include "species.h"
#include "Field/field.h"
#include "grid/gridlayout.h"
#include "types.h"

class Ions
{

private:
    std::vector <Species> speciesArray_;
    GridLayout layout_;
    Field rho_;
    Field bulkVel_;

public:

    explicit Ions(GridLayout const& layout);
    explicit Ions(GridLayout&& layout);

    Ions(Ions const& source) = delete;
    Ions& operator=(Ions const& source) = delete;


    //void addSpecies(Species species);

    uint32 nbrSpecies() {return static_cast<uint32>(speciesArray_.size()) ;}


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
