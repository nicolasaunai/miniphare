#ifndef UNIFORM_MODEL_H
#define UNIFORM_MODEL_H


#include <array>

#include <data/grid/gridlayout.h>
#include <initializer/initmodel/initmodel.h>
#include <utilities/types.h>


class UniformScalarFunction : public ScalarFunction
{
private:
    double cst_;

public:
    UniformScalarFunction(double cstValue)
        : cst_{cstValue}
    {
    }

    virtual double operator()(double x, double y, double z) override
    {
        (void)x;
        (void)y;
        (void)z;
        return cst_;
    }

    virtual ~UniformScalarFunction() = default;
};




class UniformVectorFunction : public VectorFunction
{
private:
    double vx_;
    double vy_;
    double vz_;

public:
    UniformVectorFunction(double vx, double vy, double vz)
        : vx_{vx}
        , vy_{vy}
        , vz_{vz}
    {
    }

    virtual std::array<double, 3> operator()(double x, double y, double z) override
    {
        (void)x;
        (void)y;
        (void)z;
        return {{vx_, vy_, vz_}};
    }

    virtual ~UniformVectorFunction() = default;
};




class UniformModel : public InitModel
{
public:
    UniformModel(GridLayout const& layout)
        : layout_{layout}
    {
    }

    // InitModel Interface
    virtual uint32 nbParticlesPerCell(uint32 speciesIndex) const override;
    virtual std::vector<std::unique_ptr<ParticleInitializer>> particleInitializers() const override;

    virtual std::unique_ptr<VectorFunction> electricFunction() const override;
    virtual std::unique_ptr<VectorFunction> magneticFunction() const override;


    virtual std::vector<uint32> nbrsParticlesPerCell() const override;



    // setters: functions used to set the parameters of the model
    // the setters are called by a uniformExtractor, e.g. UniformINIExtractor
    void setB0(double bx, double by, double bz);
    void setE0(double ex, double ey, double ez);
    void setNbrSpecies(uint32 nbrSpecies);
    void setDensity(double n, uint32 speciesIndex);
    void setV0(double vx, double vy, double vz, uint32 speciesIndex);
    void setVth(double Vth, uint32 speciesIndex);
    void setMass(double mass, uint32 speciesIndex);
    void setCharges(double charge, uint32 speciesIndex);
    void setNbrParticlesPerCell(uint32 nbr, uint32 speciesIndex);


    virtual ~UniformModel() = default;


private:
    std::unique_ptr<ScalarFunction> density(uint32 speciesIndex) const;
    std::unique_ptr<ScalarFunction> thermalSpeed(uint32 speciesIndex) const;
    std::unique_ptr<VectorFunction> bulkVelocity(uint32 speciesIndex) const;

    std::vector<std::unique_ptr<ScalarFunction>> densities_() const;
    std::vector<std::unique_ptr<ScalarFunction>> thermalSpeeds_() const;
    std::vector<std::unique_ptr<VectorFunction>> bulkVelocities_() const;

    GridLayout layout_;
    double bx_, by_, bz_;
    double ex_, ey_, ez_;
    std::vector<double> n0_;
    std::vector<double> vx_;
    std::vector<double> vy_;
    std::vector<double> vz_;
    std::vector<double> Vth_;

    std::vector<double> speciesMasses_;
    std::vector<double> speciesCharges_;
    std::vector<uint32> nbrParticlesPerCell_;
};

#endif // UNIFORM_MODEL_H
