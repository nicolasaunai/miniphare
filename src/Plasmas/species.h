#ifndef SPECIES_H
#define SPECIES_H


class Field;
class Particles;

class Species
{
public:
    Species();
    //~Species();

    Field* getChargeDensity();
    Field* getBulkVelocity();
    Field* getFLux();

    void computeChargeDensity();
    void computeFlux();
    void computeBulkVelocity();

    void dynamics();

private:
    Field* _rho;
    Field* _v;
    Field* _flux;
    Particles* _particle;
};

#endif // SPECIES_H
