#ifndef IONS_H
#define IONS_H

#include <vector>

class Species;
class Field;

class Ions
{
public:
    Ions();
    ~Ions();

    Field* getChargeDensity();
    Field* getBulkVelocity();

    void computeChargeDensity();
    void computeBulkVelocity();

    void load();

    unsigned int getNbSpecies() const;



private:
    std::vector<Species*> _speciesArr;
    Field* _rho;
    Field* _v;


    void _resetFields();
};

#endif // IONS_H
