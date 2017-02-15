#ifndef PERIODICDOMAINBOUNDARY_H
#define PERIODICDOMAINBOUNDARY_H

#include "BoundaryConditions/periodicdomainboundary.h"
#include "BoundaryConditions/boundary.h"
#include "vecfield/vecfield.h"



class PeriodicDomainBoundary : public Boundary
{
private:
    Edge edge_;

    void makeFieldPeriodic_(Field&   field, GridLayout const& layout) const;
    void makeFieldPeriodic1D_(Field& field, GridLayout const& layout) const;
    void makeFieldPeriodic2D_(Field& field, GridLayout const& layout) const;
    void makeFieldPeriodic3D_(Field& field, GridLayout const& layout) const;


public:

    PeriodicDomainBoundary(Edge edge):edge_{edge}{}
#if 0
    PeriodicDomainBoundary( std::string const & condition,
                     GridLayout const & layout,
                     Edge const &  edge  )
        : FieldBC(condition, layout, edge) {}
#endif


    virtual ~PeriodicDomainBoundary() = default ;

    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const override ;
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const override ;
    virtual void applyCurrentBC( VecField& J, GridLayout const& layout) const override ;
    virtual void applyDensityBC( Field& J,    GridLayout const& layout) const override ;
    virtual void applyBulkBC( VecField& Vi,   GridLayout const& layout) const override ;

    // TODO: this is not a FieldBC method, move to Class MomentBC !
    // virtual void applyMomentsBC( VecField & moment ) override ;

};




#endif // PERIODICDOMAINBOUNDARY_H
