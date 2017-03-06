#ifndef PERIODICDOMAINBOUNDARY_H
#define PERIODICDOMAINBOUNDARY_H


#include "BoundaryConditions/boundary.h"
#include "vecfield/vecfield.h"


/**
 * @brief The PeriodicDomainBoundary class is a kind of Boundary for DomainBoundaryCondition
 *
 * The class defines pure virtual methods of the interface Boundary so that fields
 * moments and particles are periodic at the domain boundary.
 */
class PeriodicDomainBoundary : public Boundary
{
private:
    Edge edge_;

    void makeFieldPeriodic_(VecField& vecField  , GridLayout const& layout) const;
    void makeFieldPeriodic1D_(VecField& vecField, GridLayout const& layout) const;
    void makeFieldPeriodic2D_(VecField& vecField, GridLayout const& layout) const;
    void makeFieldPeriodic3D_(VecField& vecField, GridLayout const& layout) const;

    void makeMomentPeriodic_(Field& Ni, GridLayout const& layout) const;
    void makeMomentPeriodic1D_(Field& Ni, GridLayout const& layout) const;
    void makeMomentPeriodic2D_(Field& Ni, GridLayout const& layout) const;
    void makeMomentPeriodic3D_(Field& Ni, GridLayout const& layout) const;


public:

    PeriodicDomainBoundary(Edge edge):edge_{edge}{std::cout << "creating periodic boundary\n";}

    virtual ~PeriodicDomainBoundary() = default ;

    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const override ;
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const override ;
    virtual void applyCurrentBC( VecField& J, GridLayout const& layout) const override ;
    virtual void applyDensityBC( Field& J,    GridLayout const& layout) const override ;
    virtual void applyBulkBC(    VecField& Vi,GridLayout const& layout) const override ;
    virtual void applyParticleBC(std::vector<Particle>& particleArray,
                                 LeavingParticles const& leavingParticles)  const override ;

};




#endif // PERIODICDOMAINBOUNDARY_H
