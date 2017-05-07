#ifndef OHMIMPL1D_H
#define OHMIMPL1D_H


#include "Ohm/ohm.h"



class OhmImpl1D : public OhmImpl
{
private:
    virtual void ideal_(VecField const& Ve, VecField const& B) override;
    virtual void resistive_(VecField const& J) override;
    virtual void pressure_(Field const& Pe, Field const& Ne) override;

public:
    explicit OhmImpl1D(GridLayout const& layout, double eta, double nu);


    virtual void computeTerms(VecField const& B, Field const& Ne, VecField const& Ve,
                              Field const& Pe, VecField const& J) override;

    ~OhmImpl1D();
};




#endif // OHMIMPL1D_H
