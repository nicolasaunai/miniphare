#ifndef FARADAY1D_H
#define FARADAY1D_H


#include "faraday.h"
#include "Vecfield/vecfield.h"


class Faraday1D :  public Faraday
{
public:
    Faraday1D(double dt, double dx);
    Faraday1D(Faraday1D const& source) = default;

    virtual void operator()(VecField const& E,
                            VecField const& B,
                            VecField& Bpred) override;


    virtual Faraday1D* clone() override {return new Faraday1D(*this);}


private:
    double dx_;
    double dtodx_;
};

#endif // FARADAY1D_H
