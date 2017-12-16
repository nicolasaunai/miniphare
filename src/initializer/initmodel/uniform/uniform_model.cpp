

#include "uniform_model.h"
#include <cmath>
#include <initializer/fluidparticleinitializer.h>
#include <numeric>


void UniformModel::setNbrSpecies(uint32 nbrSpecies)
{
    n0_.resize(nbrSpecies);
    vx_.resize(nbrSpecies);
    vy_.resize(nbrSpecies);
    vz_.resize(nbrSpecies);
    betas_.resize(nbrSpecies);
    speciesCharges_.resize(nbrSpecies);
    speciesMasses_.resize(nbrSpecies);
    speciesAnisotropy_.resize(nbrSpecies);
    nbrParticlesPerCell_.resize(nbrSpecies);
    speciesBasis_.resize(nbrSpecies);
}

void UniformModel::setB0(double bx, double by, double bz)
{
    bx_ = bx;
    by_ = by;
    bz_ = bz;
}


void UniformModel::setE0(double ex, double ey, double ez)
{
    ex_ = ex;
    ey_ = ey;
    ez_ = ez;
}

void UniformModel::setDensity(double n, uint32 speciesIndex)
{
    n0_[static_cast<std::size_t>(speciesIndex)] = n;
}


void UniformModel::setV0(double vx, double vy, double vz, uint32 speciesIndex)
{
    vx_[static_cast<std::size_t>(speciesIndex)] = vx;
    vy_[static_cast<std::size_t>(speciesIndex)] = vy;
    vz_[static_cast<std::size_t>(speciesIndex)] = vz;
}


void UniformModel::setBeta(double beta, uint32 speciesIndex)
{
    betas_[static_cast<std::size_t>(speciesIndex)] = beta;
}



void UniformModel::setAnisotropy(double aniso, uint32 speciesIndex)
{
    speciesAnisotropy_[static_cast<std::size_t>(speciesIndex)] = aniso;
}




void UniformModel::setMass(double mass, uint32 speciesIndex)
{
    speciesMasses_[static_cast<std::size_t>(speciesIndex)] = mass;
}

void UniformModel::setCharges(double charge, uint32 speciesIndex)
{
    speciesCharges_[static_cast<std::size_t>(speciesIndex)] = charge;
}

void UniformModel::setNbrParticlesPerCell(uint32 nbr, uint32 speciesIndex)
{
    nbrParticlesPerCell_[static_cast<std::size_t>(speciesIndex)] = nbr;
}


void UniformModel::setBasis(Basis basis, uint32 speciesIndex)
{
    speciesBasis_[static_cast<std::size_t>(speciesIndex)] = basis;
}




std::unique_ptr<ScalarFunction> UniformModel::density(uint32 speciesIndex) const
{
    return std::unique_ptr<ScalarFunction>{new UniformScalarFunction{n0_[speciesIndex]}};
}



std::unique_ptr<VectorFunction> UniformModel::thermalSpeed(uint32 speciesIndex) const
{
    auto beta = betas_[speciesIndex];

    auto aniso   = speciesAnisotropy_[speciesIndex];
    auto mass    = speciesMasses_[speciesIndex];
    auto density = n0_[speciesIndex];
    auto basis   = speciesBasis_[speciesIndex];

    auto rho    = density * mass;
    auto b2     = bx_ * bx_ + by_ * by_ + bz_ * bz_;
    auto traceP = 0.5 * 3 * b2 * beta;

    if (basis == Basis::Magnetic)
    {
        // find Pperp and Ppara knowing that Trace(P) = Ppara + 2*Pperp
        // and Pperp/Ppara = aniso
        auto Ppara = traceP / (1. + 2 * aniso);
        auto Pperp = Ppara * aniso;

        auto VthPara = std::sqrt(Ppara / rho);
        auto VthPerp = std::sqrt(Pperp / rho);
        return std::unique_ptr<VectorFunction>{
            new UniformVectorFunction{VthPara, VthPerp, VthPerp}};
    }
    else
    {
        auto P   = traceP / 3.;
        auto Vth = std::sqrt(P / rho);
        return std::unique_ptr<VectorFunction>{new UniformVectorFunction{Vth, Vth, Vth}};
    }
}


std::unique_ptr<VectorFunction> UniformModel::bulkVelocity(uint32 speciesIndex) const
{
    return std::unique_ptr<VectorFunction>{
        new UniformVectorFunction{vx_[speciesIndex], vy_[speciesIndex], vz_[speciesIndex]}};
}



uint32 UniformModel::nbParticlesPerCell(uint32 speciesIndex) const
{
    return nbrParticlesPerCell_[speciesIndex];
}


std::vector<uint32> UniformModel::nbrsParticlesPerCell() const
{
    return nbrParticlesPerCell_;
}



std::vector<std::unique_ptr<ScalarFunction>> UniformModel::densities_() const
{
    std::vector<std::unique_ptr<ScalarFunction>> densityFunctions;
    uint32 nbrSpecies = static_cast<uint32>(speciesMasses_.size());

    for (uint32 speciesIndex = 0; speciesIndex < nbrSpecies; ++speciesIndex)
    {
        densityFunctions.push_back(density(speciesIndex));
    }
    return densityFunctions;
}



std::vector<std::unique_ptr<VectorFunction>> UniformModel::thermalSpeeds_() const
{
    std::vector<std::unique_ptr<VectorFunction>> thermalSpeedFunctions;
    uint32 nbrSpecies = static_cast<uint32>(speciesMasses_.size());

    for (uint32 speciesIndex = 0; speciesIndex < nbrSpecies; ++speciesIndex)
    {
        thermalSpeedFunctions.push_back(thermalSpeed(speciesIndex));
    }
    return thermalSpeedFunctions;
}



std::vector<std::unique_ptr<VectorFunction>> UniformModel::bulkVelocities_() const
{
    std::vector<std::unique_ptr<VectorFunction>> bulkVelocityFunctions;
    uint32 nbrSpecies = static_cast<uint32>(speciesMasses_.size());

    for (uint32 speciesIndex = 0; speciesIndex < nbrSpecies; ++speciesIndex)
    {
        bulkVelocityFunctions.push_back(bulkVelocity(speciesIndex));
    }
    return bulkVelocityFunctions;
}



std::vector<std::unique_ptr<ParticleInitializer>> UniformModel::particleInitializers() const
{
    std::vector<std::unique_ptr<ScalarFunction>> densities      = densities_();
    std::vector<std::unique_ptr<VectorFunction>> thermalSpeeds  = thermalSpeeds_();
    std::vector<std::unique_ptr<VectorFunction>> bulkVelocities = bulkVelocities_();

    std::size_t nbrSpecies = densities.size();

    std::vector<std::unique_ptr<ParticleInitializer>> partInits;

    for (std::size_t speciesIndex = 0; speciesIndex < nbrSpecies; ++speciesIndex)
    {
        std::unique_ptr<ParticleInitializer> pinit{new FluidParticleInitializer{
            layout_, std::move(densities[speciesIndex]), std::move(bulkVelocities[speciesIndex]),
            std::move(thermalSpeeds[speciesIndex]), nbrParticlesPerCell_[speciesIndex],
            speciesCharges_[speciesIndex], speciesBasis_[speciesIndex], magneticFunction()}};

        partInits.push_back(std::move(pinit));
    }
    return partInits;
}



std::unique_ptr<VectorFunction> UniformModel::electricFunction() const
{
    return std::unique_ptr<VectorFunction>{new UniformVectorFunction{ex_, ey_, ez_}};
}


std::unique_ptr<VectorFunction> UniformModel::magneticFunction() const
{
    return std::unique_ptr<VectorFunction>{new UniformVectorFunction{bx_, by_, bz_}};
}
